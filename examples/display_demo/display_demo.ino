#include <Arduino.h>
#include "epd_driver.h"

#include "firasans.h"
#include "esp_adc_cal.h"
#include <FS.h>
#include <SPI.h>
#include <SD.h>

#include <Wire.h>
#include <TouchDrvGT911.hpp>
#include <SensorPCF8563.hpp>
#include <WiFi.h>
#include <esp_sntp.h>
#include "utilities.h"
#include <RadioLib.h>
#include "ui.h"

#define USED_EPD_LVGL 1

#define WIFI_SSID "Your WiFi SSID"
#define WIFI_PASSWORD "Your WiFi PASSWORD"

#define DISPLAY_WIDTH 960
#define DISPLAY_HEIGHT 576
#define DISPLAY_MAX_PAGE 2

/* clang-format off */
#define AREA_EVENT(a, tx, ty)                                                                         \
    if (touch_press) {                                                                                \
        for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++) {                                          \
            if ((tx > a[i].x && tx < (a[i].x + a[i].w)) && (ty > a[i].y && ty < (a[i].y + a[i].h))) { \
                touch_cnt++;                                                                          \
                if (touch_cnt == 2) {                                                                 \
                    switch (a[i].buttonID)                                                            \
                    { 

#define AREA_CASE(x) case x:

#define AREA_END        \
    default:break;}     \
        touch_cnt = 0;} \
        break; } } }
/* clang-format on */

//
bool sd_is_init = false;
bool rtc_is_init = false;
bool lora_is_init = false;
bool touchOnline = false;

// wifi
// char wifi_ssid[WIFI_SSID_MAX_LEN] = "xinyuandianzi";
// char wifi_password[WIFI_PSWD_MAX_LEN] = "AA15994823428";
const char *wifi_ssid = "xinyuandianzi";
const char *wifi_password = "AA15994823428";
const char *ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
static uint32_t last_tick;
bool wifi_is_connect = false;
struct tm timeinfo = {0};

int touch_press = 0;
int touch_cnt = 0;
int16_t touch_x, touch_y;
int cursor_x, cursor_y;
uint8_t *framebuffer;
uint32_t interval = 0;
int vref = 1100;
char buf[128];

struct _point
{
    uint8_t buttonID;
    int32_t x;
    int32_t y;
    int32_t w;
    int32_t h;
};

SensorPCF8563 rtc;
TouchDrvGT911 touch;

SX1262 radio = new Module(LORA_CS, LORA_IRQ, LORA_RST, LORA_BUSY);
int transmissionState = RADIOLIB_ERR_NONE;

#if USED_EPD_LVGL // EPD_LVGL
#include "lvgl.h"
#include "settings.h"

#define DISP_BUF_SIZE (EPD_WIDTH*EPD_HEIGHT)
uint8_t *decodebuffer = NULL;

volatile bool disp_flush_enabled = true;
bool disp_refr_is_busy = false;

int mapTo1Byte(uint8_t* data) {
    uint8_t byte = 0;
    for (int i = 0; i < 4; i++) {
        byte ^= *(data + i);
    }
    return byte;
}

void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    if(disp_flush_enabled) {
        uint16_t w = lv_area_get_width(area);
        uint16_t h = lv_area_get_height(area);
        // uint32_t gray = 0;
        int i=0, j=0;
        uint8_t *c  = (uint8_t *)color_p;
        lv_color32_t *t = (lv_color32_t *)color_p;

#if LV_COLOR_DEPTH == 8
        for(i = 0; i < (w * h) / 2; i++) {
            uint8_t t1 = 0, t2 = 0, tmp = 0;
            t1 = c[j + 0] / 16;
            t2 = c[j + 1] / 16;
            tmp = ((t1 << 4) & 0xF0) | (t2 & 0x0F);
            c[i] = tmp;
            j += 2;
        }
#elif LV_COLOR_DEPTH == 16
        for(i = 0; i < (w * h) / 2; i++) {
            // uint8_t tmp = 0;
            // tmp |= (c[j + 0] << 6) & 0xC0;
            // tmp |= (c[j + 1] << 4) & 0x30;
            // tmp |= (c[j + 2] << 2) & 0x0C;
            // tmp |= (c[j + 3] << 0) & 0x03;
            // decodebuffer[i] = tmp;
            // j += 4;
            decodebuffer[i] = t->ch.green;
            t++;
        }
#elif LV_COLOR_DEPTH == 32
        for (int y = area->y1; y <= area->y2; y++) {
            for (int x = area->x1; x <= area->x2; x++) {
                uint8_t gray = (color_p->ch.red * 38 + color_p->ch.green * 75 + color_p->ch.blue * 15) >> 7;

                color_p++;
            }
        }
#endif
        // Serial.printf("dips x=%d, y=%d, w=%d, h=%d\n", area->x1, area->y1, w, h);
    }
    /* Inform the graphics library that you are ready with the flushing */
    lv_disp_flush_ready(disp);
}

void disp_refrensh_cb(lv_timer_t *t)
{
    lv_timer_del(t);
    disp_refr_is_busy = false;

    epd_poweron();
    epd_clear();
    epd_draw_image(epd_full_screen(), (uint8_t *)decodebuffer, BLACK_ON_WHITE);
    epd_poweroff();
}

void disp_manual_refr(void)
{
    if(disp_refr_is_busy == false) {
        lv_timer_create(disp_refrensh_cb, 200, NULL);
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

    // lv_color_t *lv_disp_buf_p = (lv_color_t *)ps_calloc(sizeof(lv_color_t), DISP_BUF_SIZE);
    // lv_color_t *lv_disp_buf_p1 = (lv_color_t *)ps_calloc(sizeof(lv_color_t), DISP_BUF_SIZE);
    // lv_disp_draw_buf_init(&draw_buf, lv_disp_buf_p, lv_disp_buf_p1, DISP_BUF_SIZE);

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
#endif

static void get_curr_time(lv_timer_t *t)
{
    if(wifi_is_connect == true){
        if (!getLocalTime(&timeinfo)){
            Serial.println("Failed to obtain time");
            return;
        }
        // Serial.println(&timeinfo, "%F %T %A"); // 格式化输出
        // timeinfo.tm_hour = timeinfo.tm_hour % 12;
        // lv_msg_send(MSG_CLOCK_HOUR, &timeinfo.tm_hour);
        // lv_msg_send(MSG_CLOCK_MINUTE, &timeinfo.tm_min);
        // lv_msg_send(MSG_CLOCK_SECOND, &timeinfo.tm_sec);
    }
}


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


void setup()
{
    Serial.begin(115200);

    pinMode(BL_EN, OUTPUT);
    analogWrite(BL_EN, 0);

    wifi_init();

    epd_init();

    // SD
    SPI.begin(BOARD_SPI_SCLK, BOARD_SPI_MISO, BOARD_SPI_MOSI, -1);
    sd_is_init = SD.begin(SD_CS);

    // LORA
    // Serial.print(F("[SX1262] Initializing ... "));
    // int state= radio.begin();
    // if (state == RADIOLIB_ERR_NONE) {
    //     Serial.println(F("success!"));
    //     lora_is_init = true;
    // } else {
    //     Serial.print(F("failed, code "));
    //     Serial.println(state);
    //     lora_is_init = false;
    // }

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

    // RTC
    Wire.beginTransmission(PCF8563_SLAVE_ADDRESS);
    if (Wire.endTransmission() == 0)
    {
        rtc.begin(Wire, PCF8563_SLAVE_ADDRESS, BOARD_SDA, BOARD_SCL);
        rtc_is_init = true;
    }

    // TOUCH
    uint8_t touchAddress = 0x14;
    Wire.beginTransmission(0x5D);
    if (Wire.endTransmission() == 0)
    {
        touchAddress = 0x5D;
    }

    touch.setPins(TOUCH_RST, TOUCH_INT);
    if (touch.begin(Wire, touchAddress, BOARD_SDA, BOARD_SCL))
    {
        touch.setMaxCoordinates(EPD_WIDTH, EPD_HEIGHT);
        touch.setSwapXY(true);
        touch.setMirrorXY(false, true);
        touchOnline = true;
        Serial.printf("touchOnline \n");
    }

    Serial.printf("Touch init %s\n", touchOnline? "PASS" : "FAIL");
    Serial.printf("SD card init %s\n", sd_is_init? "PASS" : "FAIL");
    Serial.printf("RTC init %s\n", rtc_is_init? "PASS" : "FAIL");
    Serial.printf("LORA init %s\n", lora_is_init? "PASS" : "FAIL");

    lv_port_disp_init();
    // epd_poweron();
    // epd_clear();
    
    ui_epd47_entry();
    disp_manual_refr();

    lv_timer_create(get_curr_time, 5000, NULL);
}

void loop()
{
    lv_task_handler();
    delay(1);
}
