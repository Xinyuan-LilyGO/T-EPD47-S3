
#include "ui.h"
#include "display_demo.h"

//
bool sd_is_init = false;
bool rtc_is_init = false;
bool lora_is_init = false;
bool touchOnline = false;
bool bq25896_is_init = false;
bool bq27220_is_init = false;

// wifi
char wifi_ssid[WIFI_SSID_MAX_LEN] = {0};
char wifi_password[WIFI_PSWD_MAX_LEN] = {0};
// const char *wifi_ssid = WIFI_SSID;
// const char *wifi_password = WIFI_PASSWORD;
const char *ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
static uint32_t last_tick;
bool wifi_is_connect = false;
bool wifi_eeprom_upd = false;
struct tm timeinfo = {0};

BQ25896 battery_25896(Wire);
BQ27220 bq27220;
SensorPCF8563 rtc;
TouchDrvGT911 touch;

SX1262 radio = new Module(LORA_CS, LORA_IRQ, LORA_RST, LORA_BUSY);
int lora_mode = LORA_MODE_SEND;

// transmit 
int transmissionState = RADIOLIB_ERR_NONE;
volatile bool transmittedFlag = false;

void set_transmit_flag(void){
    transmittedFlag = true;
}

// receive
int receivedState = RADIOLIB_ERR_NONE;
volatile bool receivedFlag = false;

void set_receive_flag(void){
    receivedFlag = true;
}

// lvgl
#define DISP_BUF_SIZE (EPD_WIDTH*EPD_HEIGHT)
uint8_t *decodebuffer = NULL;
uint16_t refr_backlight = 0;
uint16_t refr_cycle = 10;
uint16_t refr_times = 4;
lv_timer_t *get_curr_data_timer = NULL;

volatile bool disp_flush_enabled = true;
bool disp_refr_is_busy = false;

// eeprom
void eeprom_default_val(void)
{
    char wifi_ssid[WIFI_SSID_MAX_LEN] = WIFI_SSID;
    char wifi_password[WIFI_PSWD_MAX_LEN] = WIFI_PASSWORD;

    EEPROM.write(0, EEPROM_UPDATA_FLAG_NUM);
    for(int i = WIFI_SSID_EEPROM_ADDR; i < WIFI_SSID_EEPROM_ADDR + WIFI_SSID_MAX_LEN; i++) {
        int k = i - WIFI_SSID_EEPROM_ADDR;
        if(k < WIFI_SSID_MAX_LEN) {
            EEPROM.write(i, wifi_ssid[k]);
        } else {
            EEPROM.write(i, 0x00);
        }
    }
    for(int i = WIFI_PSWD_EEPROM_ADDR; i < WIFI_PSWD_EEPROM_ADDR + WIFI_PSWD_MAX_LEN; i++) {
        int k = i - WIFI_PSWD_EEPROM_ADDR;
        if(k < WIFI_PSWD_MAX_LEN) {
            EEPROM.write(i, wifi_password[k]);
        } else {
            EEPROM.write(i, 0x00);
        }
    }
    EEPROM.commit();
    wifi_eeprom_upd = true;
}

void eeprom_wr(int addr, uint8_t val)
{
    if(wifi_eeprom_upd == false) {
        eeprom_default_val();
    }
    EEPROM.write(addr, val);
    EEPROM.commit();
    Serial.printf("eeprom_wr %d:%d\n", addr, val);
}

void eeprom_wr_wifi(const char *ssid, uint16_t ssid_len, const char *pwsd, uint16_t pwsd_len)
{
    Serial.printf("[eeprom] eeprom_wr_wifi \n%s:%d\n%s:%d\n", ssid, ssid_len, pwsd, pwsd_len);
    if(ssid_len > WIFI_SSID_MAX_LEN) 
        ssid_len = WIFI_SSID_MAX_LEN;
    if(pwsd_len > WIFI_PSWD_MAX_LEN)
        pwsd_len = WIFI_PSWD_MAX_LEN;

    if(wifi_eeprom_upd == false) {
        EEPROM.write(0, EEPROM_UPDATA_FLAG_NUM);
        wifi_eeprom_upd = true;
    }

    for(int i = WIFI_SSID_EEPROM_ADDR; i < WIFI_SSID_EEPROM_ADDR + WIFI_SSID_MAX_LEN; i++) {
        int k = i - WIFI_SSID_EEPROM_ADDR;
        if(k < ssid_len) {
            EEPROM.write(i, ssid[k]);
        } else {
            EEPROM.write(i, 0x00);
        }
    }
    for(int i = WIFI_PSWD_EEPROM_ADDR; i < WIFI_PSWD_EEPROM_ADDR + WIFI_PSWD_MAX_LEN; i++) {
        int k = i - WIFI_PSWD_EEPROM_ADDR;
        if(k < pwsd_len) {
            EEPROM.write(i, pwsd[k]);
        } else {
            EEPROM.write(i, 0x00);
        }
    }
    EEPROM.commit();
}


#define DEBUG_INFO //

void eeprom_init()
{
    if (!EEPROM.begin(EEPROM_SIZE_MAX)) {
        Serial.println("[eeprom] failed to initialise EEPROM"); delay(1000000);
    }
    uint8_t frist_flag = EEPROM.read(0);
    Serial.printf("eeprom flag: %d\n", frist_flag);
    if(frist_flag == EEPROM_UPDATA_FLAG_NUM) {
        for(int i = WIFI_SSID_EEPROM_ADDR; i < WIFI_SSID_EEPROM_ADDR + WIFI_SSID_MAX_LEN; i++) {
            wifi_ssid[i - WIFI_SSID_EEPROM_ADDR] = EEPROM.read(i);
        }
        for(int i = WIFI_PSWD_EEPROM_ADDR; i < WIFI_PSWD_EEPROM_ADDR + WIFI_PSWD_MAX_LEN; i++) {
            wifi_password[i - WIFI_PSWD_EEPROM_ADDR] = EEPROM.read(i);
        }

        wifi_eeprom_upd = true;
        
        Serial.printf("eeprom SSID: %s\n", wifi_ssid);
        Serial.printf("eeprom PWSD: %s\n", wifi_password);
        // uint8_t theme = EEPROM.read(UI_THEME_EEPROM_ADDR);
        uint8_t backlight = EEPROM.read(UI_BACKLIGHT_EEPROM_ADDR);
        uint8_t cycle = EEPROM.read(UI_REFR_CYCLE_EEPROM_ADDR);
        uint8_t times = EEPROM.read(UI_REFR_TIMES_EEPROM_ADDR);

        refr_backlight = backlight;
        refr_cycle = cycle;
        refr_times = times;
        
        // Serial.printf("eeprom theme: %d\n", theme);
        Serial.printf("eeprom backlight: %d\n", refr_backlight);
        Serial.printf("eeprom refr_cycle: %d\n", refr_cycle);
        Serial.printf("eeprom refr_times: %d\n", refr_times);
    }
}

void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    if(disp_flush_enabled) {
        static int refr_cnt = 0;
        uint16_t w = lv_area_get_width(area);
        uint16_t h = lv_area_get_height(area);
        lv_color32_t *t32 = (lv_color32_t *)color_p;

        for(int i = 0; i < (w * h) / 2; i++) {

            lv_color8_t ret;
            LV_COLOR_SET_R8(ret, LV_COLOR_GET_R(*t32) >> 5); /*8 - 3  = 5*/
            LV_COLOR_SET_G8(ret, LV_COLOR_GET_G(*t32) >> 5); /*8 - 3  = 5*/
            LV_COLOR_SET_B8(ret, LV_COLOR_GET_B(*t32) >> 6); /*8 - 2  = 6*/
            decodebuffer[i] = ret.full;
            t32++;
        }
        Serial.printf("[disp_flush]refr_cnt: %d\n", refr_cnt++);
    }
    /* Inform the graphics library that you are ready with the flushing */
    lv_disp_flush_ready(disp);
}

void disp_refrensh_cb(lv_timer_t *t)
{
    lv_timer_del(t);
    disp_refr_is_busy = false;

    epd_poweron();
    // epd_clear();
    epd_clear_area_cycles(epd_full_screen(), refr_times, refr_cycle);
    epd_draw_grayscale_image(epd_full_screen(), (uint8_t *)decodebuffer);
    epd_poweroff();
}

void disp_manual_refr(uint16_t time)
{
    if(disp_refr_is_busy == false) {
        lv_timer_create(disp_refrensh_cb, time, NULL);
        disp_refr_is_busy = true;
    }
}

uint32_t epd_refr_data = 0;

void disp_refrensh_cycle_cb(lv_timer_t *t)
{
    uint16_t cycle = (epd_refr_data >> 16) & 0x0000FFFF;
    uint16_t times = (epd_refr_data & 0x0000FFFF);

    lv_timer_del(t);
    disp_refr_is_busy = false;

    epd_poweron();
    // epd_clear();
    Serial.printf("t=%d, c=%d\n", times, cycle);
    epd_clear_area_cycles(epd_full_screen(), times, cycle);
    epd_draw_grayscale_image(epd_full_screen(), (uint8_t *)decodebuffer);
    epd_poweroff();
}

void disp_manual_refr_cycle(uint16_t time, uint16_t cycle, uint16_t times)
{
    epd_refr_data = ((cycle << 16) & 0xFFFF0000) | times;
    if(disp_refr_is_busy == false) {
        lv_timer_create(disp_refrensh_cycle_cb, time, &epd_refr_data);
        disp_refr_is_busy = true;
    }
}

void my_input_read(lv_indev_drv_t * drv, lv_indev_data_t*data)
{
    int16_t  x, y;
    if(touchOnline) {
        if(touch.getPoint(&x, &y)){
            data->point.x = x;
            data->point.y = y;
            data->state = LV_INDEV_STATE_PRESSED;
            Serial.printf("input X:%d Y:%d\n", data->point.x, data->point.y);
        }else {
            data->state = LV_INDEV_STATE_RELEASED; 
        }
    }
}

void lv_port_disp_init(void)
{
    lv_init();

    static lv_disp_draw_buf_t draw_buf;

    lv_color_t *lv_disp_buf_p = (lv_color_t *)ps_calloc(sizeof(lv_color_t), DISP_BUF_SIZE);
    decodebuffer = (uint8_t *)ps_calloc(sizeof(uint8_t), DISP_BUF_SIZE);
    lv_disp_draw_buf_init(&draw_buf, lv_disp_buf_p, NULL, DISP_BUF_SIZE);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = EPD_WIDTH;
    disp_drv.ver_res = EPD_HEIGHT;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.full_refresh = 1;
    lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);      /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;                 /*See below.*/
    indev_drv.read_cb = my_input_read;              /*See below.*/
    /*Register the driver in LVGL and save the created input device object*/
    // static lv_indev_t * my_indev = lv_indev_drv_register(&indev_drv);
    lv_indev_drv_register(&indev_drv);
}

static void get_curr_time(lv_timer_t *t)
{
    static int cnt = 0;
    if(wifi_is_connect == true){
        if (!getLocalTime(&timeinfo)){
            Serial.println("Failed to obtain time");
            return;
        }
    }
}

// wifi
void wifi_init(void)
{
    Serial.printf("SSID len: %d\n", strlen(wifi_ssid));
    Serial.printf("PWSD len: %d\n", strlen(wifi_password));
    if(strlen(wifi_ssid) == 0 || strlen(wifi_password) == 0) {
        return;
    }

    WiFi.begin(wifi_ssid, wifi_password);
    wl_status_t wifi_state = WiFi.status();
    last_tick = millis();
    while (wifi_state != WL_CONNECTED){
        delay(500);
        Serial.print(".");
        wifi_state = WiFi.status();
        if(wifi_state == WL_CONNECTED){
            wifi_is_connect = true;
            Serial.println("WiFi connected!");
            configTime(8 * 3600, 0, ntpServer1, ntpServer2);
            break;
        }
        if (millis() - last_tick > 5000) {
            Serial.println("WiFi connected falied!");
            last_tick = millis();
            break;
        }
    }
}

// lora
bool lora_init(void)
{
    Serial.print(F("[SX1262] Initializing ... "));
    int state= radio.begin();
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
        return false;
    }

    // set carrier frequency to 433.5 MHz
    if (radio.setFrequency(433.5) == RADIOLIB_ERR_INVALID_FREQUENCY) {
        Serial.println(F("Selected frequency is invalid for this module!"));
        while (true);
    }

    // set bandwidth to 250 kHz
    if (radio.setBandwidth(250.0) == RADIOLIB_ERR_INVALID_BANDWIDTH) {
        Serial.println(F("Selected bandwidth is invalid for this module!"));
        while (true);
    }

    // set spreading factor to 10
    if (radio.setSpreadingFactor(10) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR) {
        Serial.println(F("Selected spreading factor is invalid for this module!"));
        while (true);
    }

    // set coding rate to 6
    if (radio.setCodingRate(6) == RADIOLIB_ERR_INVALID_CODING_RATE) {
        Serial.println(F("Selected coding rate is invalid for this module!"));
        while (true);
    }

    // set LoRa sync word to 0xAB
    if (radio.setSyncWord(0xAB) != RADIOLIB_ERR_NONE) {
        Serial.println(F("Unable to set sync word!"));
        while (true);
    }

    // set output power to 10 dBm (accepted range is -17 - 22 dBm)
    if (radio.setOutputPower(10) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
        Serial.println(F("Selected output power is invalid for this module!"));
        while (true);
    }

    // set over current protection limit to 80 mA (accepted range is 45 - 240 mA)
    // NOTE: set value to 0 to disable overcurrent protection
    if (radio.setCurrentLimit(80) == RADIOLIB_ERR_INVALID_CURRENT_LIMIT) {
        Serial.println(F("Selected current limit is invalid for this module!"));
        while (true);
    }

    // set LoRa preamble length to 15 symbols (accepted range is 0 - 65535)
    if (radio.setPreambleLength(15) == RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH) {
        Serial.println(F("Selected preamble length is invalid for this module!"));
        while (true);
    }

    // disable CRC
    if (radio.setCRC(false) == RADIOLIB_ERR_INVALID_CRC_CONFIGURATION) {
        Serial.println(F("Selected CRC is invalid for this module!"));
        while (true);
    }

    // Some SX126x modules have TCXO (temperature compensated crystal
    // oscillator). To configure TCXO reference voltage,
    // the following method can be used.
    if (radio.setTCXO(2.4) == RADIOLIB_ERR_INVALID_TCXO_VOLTAGE) {
        Serial.println(F("Selected TCXO voltage is invalid for this module!"));
        while (true);
    }

    // Some SX126x modules use DIO2 as RF switch. To enable
    // this feature, the following method can be used.
    // NOTE: As long as DIO2 is configured to control RF switch,
    //       it can't be used as interrupt pin!
    if (radio.setDio2AsRfSwitch() != RADIOLIB_ERR_NONE) {
        Serial.println(F("Failed to set DIO2 as RF switch!"));
        while (true);
    }

    Serial.println(F("All settings succesfully changed!"));

    radio.setPacketSentAction(set_transmit_flag);
    Serial.println(F("[SX1262] Sending first packet ... "));
    transmissionState = radio.startTransmit("Hello World!");
    radio.sleep();

    return true;
}

void lora_set_mode(int mode) 
{
    if(mode == LORA_MODE_SEND){
        radio.setPacketSentAction(set_transmit_flag);
        Serial.println(F("[LORA] Sending first packet ... "));
        transmissionState = radio.startTransmit("Hello World!");
    } else if(mode == LORA_MODE_RECV){
        radio.setPacketReceivedAction(set_receive_flag);
        Serial.println(F("[LORA] Starting to listen ... "));
        receivedState = radio.startReceive();
    }
    lora_mode = mode;
}

bool lora_receive(String *str)
{
    bool ret = false;
    if(receivedFlag){
        receivedFlag = false;
        // String str;
        receivedState = radio.readData(*str);
        if(receivedState == RADIOLIB_ERR_NONE){
            Serial.print(F("[SX1262] Data:\t\t"));
            // Serial.println(str);
            ret = true;
        }else{
            Serial.print(F("failed, code "));
            Serial.println(receivedState);
        }
    }
    return ret;
}

void lora_transmit(const char *str)
{
    if(transmittedFlag){
        transmittedFlag = false;
        if(transmissionState == RADIOLIB_ERR_NONE){
            Serial.println(F("transmission finished!"));
        } else {
            Serial.print(F("failed, code "));
            Serial.println(transmissionState);
        }

        radio.finishTransmit();
        Serial.print(F("[Lora] Sending another packet ... "));
        transmissionState = radio.startTransmit(str);
    }
}

void setup()
{
    Serial.begin(115200);

    eeprom_init();

    wifi_init();
    configTime(8 * 3600, 0, ntpServer1, ntpServer2);

    epd_init();

    // SD
    SPI.begin(BOARD_SPI_SCLK, BOARD_SPI_MISO, BOARD_SPI_MOSI);
    sd_is_init = SD.begin(SD_CS);

    // LORA
    lora_is_init = lora_init();
    
    // I2C Scan
    /**
     * 0x51 --- RTC
     * 0x5D --- Touch
     * 0x6B --- BQ25896
     * 0x55 --- BQ27220
    */
    byte error, address;
    int nDevices = 0;
    Wire.begin(BOARD_SDA, BOARD_SCL);
    for(address = 0x01; address < 0x7F; address++){
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if(error == 0){ // 0: success.
            nDevices++;
            Serial.printf("I2C device found at address 0x%x\n", address);
        }
    }

    // Touch --- 0x5D
    touch.setPins(TOUCH_RST, TOUCH_INT);
    if (touch.begin(Wire, 0x5D, BOARD_SDA, BOARD_SCL))
    {
        touch.setMaxCoordinates(EPD_WIDTH, EPD_HEIGHT);
        touch.setSwapXY(true);
        touch.setMirrorXY(false, true);
        touchOnline = true;
        Serial.printf("touchOnline \n");
    }

    // RTC --- 0x51
    Wire.beginTransmission(PCF8563_SLAVE_ADDRESS);
    if (Wire.endTransmission() == 0)
    {
        rtc.begin(Wire, PCF8563_SLAVE_ADDRESS, BOARD_SDA, BOARD_SCL);
        rtc.setDateTime(2022, 6, 30, 0, 0, 0);
        rtc_is_init = true;
    }

    // BQ25896 --- 0x6B
    Wire.beginTransmission(0x6B);
    if (Wire.endTransmission() == 0)
    {
        bq25896_is_init = true;
        battery_25896.begin();
    }

    // BQ27220 --- 0x55
    Wire.beginTransmission(0x55);
    if (Wire.endTransmission() == 0)
    {
        bq27220_is_init = true;
    }

    epd_poweroff();

    Serial.printf("Touch init %s\n", touchOnline? "PASS" : "FAIL");
    Serial.printf("SD card init %s\n", sd_is_init? "PASS" : "FAIL");
    Serial.printf("RTC init %s\n", rtc_is_init? "PASS" : "FAIL");
    Serial.printf("LORA init %s\n", lora_is_init? "PASS" : "FAIL");

    lv_port_disp_init();

    ui_epd47_entry();
    disp_manual_refr(500);

    pinMode(BL_EN, OUTPUT);
    analogWrite(BL_EN, refr_backlight);

    // get_curr_time(NULL);
    get_curr_data_timer = lv_timer_create(get_curr_time, 5000, NULL);
}

void loop()
{
    lv_task_handler();
    delay(1);
}
