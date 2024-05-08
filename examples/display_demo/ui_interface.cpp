#include "ui.h"
#include "display_demo.h"
#include "Arduino.h"


//************************************[ Other fun ]******************************************
void ui_if_epd_refr(void) 
{
    disp_manual_refr();
}

//************************************[ screen 0 ]****************************************** menu
void ui_if_epd_get_time(uint8_t *h, uint8_t *m, uint8_t *s) 
{
    *h = timeinfo.tm_hour;
    *m = timeinfo.tm_min;
    *s = timeinfo.tm_sec;
}

void ui_if_epd_get_data(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *week) 
{
    *year = timeinfo.tm_year % 100;
    *month = timeinfo.tm_mon;
    *day = timeinfo.tm_mday;
    *week = timeinfo.tm_wday;
}
//************************************[ screen 5 ]****************************************** test
bool ui_if_epd_get_SD(void) { return sd_is_init; }
bool ui_if_epd_get_RTC(void) { return rtc_is_init; }
bool ui_if_epd_get_TOUCH(void) { return touchOnline; }
bool ui_if_epd_get_LORA(void) { return lora_is_init; }