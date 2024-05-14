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