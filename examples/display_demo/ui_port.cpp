#include "ui.h"
#include "display_demo.h"
#include "Arduino.h"


//************************************[ Other fun ]******************************************
void ui_if_epd_refr(uint16_t time) 
{
    disp_manual_refr(time);
}

//************************************[ screen 1 ]****************************************** clock
void ui_if_epd_get_time(uint8_t *h, uint8_t *m, uint8_t *s) 
{
    *h = timeinfo.tm_hour;
    *m = timeinfo.tm_min;
    *s = timeinfo.tm_sec;
}

void ui_if_epd_get_data(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *week) 
{
    *year = timeinfo.tm_year % 100;
    *month = timeinfo.tm_mon+1;
    *day = timeinfo.tm_mday;
    *week = timeinfo.tm_wday;
}
//************************************[ screen 2 ]****************************************** lora
int ui_if_epd_get_LORA_mode(void)
{
    return lora_mode;
}
void ui_if_epd_set_LORA_mode(int mode)
{
    lora_set_mode(mode);
}

void ui_if_epd_LORA_send(const char *str)
{
    lora_transmit(str);
}

//************************************[ screen 3 ]****************************************** sd_card
// void ui_if_epd_set_imgbtn(const char *text, int text_len, int type)
// {

// }
void ui_if_epd_read_from_SD(void)
{
    File root = SD.open("/");
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }

    File file = root.openNextFile();
    while(file) {
        if(!file.isDirectory()) {
            char *file_name = (char *)file.name();
            uint16_t file_name_len = strlen(file_name);
            char *suffix = file_name + file_name_len - 4;
            int picture_type = 0;

            picture_type = strcmp(suffix, ".jpg") == 0 ? 1 : picture_type;
            picture_type = strcmp(suffix, ".png") == 0 ? 2 : picture_type;
            picture_type = strcmp(suffix, ".bmp") == 0 ? 3 : picture_type;

            if(picture_type) {
                ui_if_epd_set_imgbtn(file_name, file_name_len, picture_type);
            }
        }
        file = root.openNextFile();
    }
}
//************************************[ screen 4 ]****************************************** setting
void ui_if_epd_set_light(uint8_t light)
{
    analogWrite(BL_EN, light);
    refr_backlight = light;
    eeprom_wr(UI_BACKLIGHT_EEPROM_ADDR, light);
}

void ui_port_set_refr_cycle(uint16_t cycle) 
{
    refr_cycle = cycle;
    eeprom_wr(UI_REFR_CYCLE_EEPROM_ADDR, (uint8_t)cycle);
}

void ui_port_set_refr_times(uint16_t times)
{
    refr_times = times;
    eeprom_wr(UI_REFR_TIMES_EEPROM_ADDR, (uint8_t)times);
}

uint16_t ui_port_get_light(void)
{
    return refr_backlight;
}

uint16_t ui_port_get_refr_cycle(void) 
{
    return refr_cycle;
}

uint16_t ui_port_get_refr_times(void)
{
    return refr_times;
}
//************************************[ screen 5 ]****************************************** test
bool ui_if_epd_get_SD(void) { return sd_is_init; }
bool ui_if_epd_get_RTC(void) { return rtc_is_init; }
bool ui_if_epd_get_TOUCH(void) { return touchOnline; }
bool ui_if_epd_get_LORA(void) { return lora_is_init; }
bool ui_if_epd_get_WIFI(void) { return wifi_is_connect; }

//************************************[ screen 5 ]****************************************** wifi
String ui_if_epd_get_WIFI_ip(void)
{
    return WiFi.localIP().toString();
}
const char *ui_if_epd_get_WIFI_ssid(void)
{
    return wifi_ssid;
}
const char *ui_if_epd_get_WIFI_pwd(void)
{
    return wifi_password;
}

//************************************[ screen 5 ]****************************************** battery
/* 25896 */
bool battery_25896_is_vaild(void)
{
    // return bq25896_is_init;
    return true;
}

bool battery_25896_is_chr(void)
{
    return false;
}

char * battery_25896_get_VSYS_ST(void)
{
    return "Nomal";
}
char * battery_25896_get_VBUS_ST(void) 
{
    return "Nomal";
}
char * battery_25896_get_CHG_ERR(void)
{
    return "Nomal";
}
float battery_25896_get_VBUS(void)
{
    static float vsys = 0;
    vsys+=0.1;
    return 4.23+vsys;
}
float battery_25896_get_VSYS(void) 
{
    static float vsys = 0;
    vsys+=0.2;
    return 4.23+vsys;
}
float battery_25896_get_VBAT(void)
{
    static float vsys = 0;
    vsys+=0.3;
    return 4.23+vsys;
}
float battery_25896_get_ICHG(void)
{
    static float vsys = 0;
    vsys+=0.4;
    return 4.23+vsys;
}
float battery_25896_get_TEMP(void)
{
    static float vsys = 0;
    vsys+=0.5;
    return 4.23+vsys;
}
float battery_25896_get_TSPCT(void)
{
    static float vsys = 0;
    vsys=0.6;
    return 4.23+vsys;
}

/* 27220 */
bool battery_27220_is_vaild(void)
{
    // return bq27220_is_init;
    return true;
}

bool battery_27220_is_chr(void)
{
    return false;
}

float battery_27220_get_VOLT(void)
{
    static float vsys = 0;
    vsys+=0.01;
    return 4.23+vsys;
}
float battery_27220_get_VOLT_CHG(void)
{
    static float vsys = 0;
    vsys+=0.02;
    return 4.23+vsys;
}
float battery_27220_get_CURR_ARG(void)
{
    static float vsys = 0;
    vsys+=0.03;
    return 4.23+vsys;
}
float battery_27220_get_CURR_INS(void)
{
    static float vsys = 0;
    vsys+=0.04;
    return 4.23+vsys;
}
float battery_27220_get_CURR_STD(void)
{
    static float vsys = 0;
    vsys+=0.05;
    return 4.23+vsys;
}
float battery_27220_get_CURR_CHG(void)
{
    static float vsys = 0;
    vsys+=0.06;
    return 4.23+vsys;
}
float battery_27220_get_TEMP(void)
{
    static float vsys = 0;
    vsys+=0.07;
    return 4.23+vsys;
}
float battery_27220_get_BATT_CAP(void)
{
    static float vsys = 0;
    vsys+=0.08;
    return 4.23+vsys;
}
float battery_27220_get_BATT_CAP_FULL(void)
{
    static float vsys = 0;
    vsys+=0.09;
    return 4.23+vsys;
}
