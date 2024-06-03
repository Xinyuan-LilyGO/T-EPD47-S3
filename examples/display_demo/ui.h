#ifndef __UI_EPD47H__
#define __UI_EPD47H__

/*********************************************************************************
 *                                  INCLUDES
 * *******************************************************************************/
#include "lvgl.h"
#include "display_demo.h"

/*********************************************************************************
 *                                   DEFINES
 * *******************************************************************************/
#define LCD_HOR_SIZE 960
#define LCD_VER_SIZE 540

#define EPD_COLOR_BG          0xffffff
#define EPD_COLOR_FOCUS_ON    0x91B821
#define EPD_COLOR_TEXT        0x000000
#define EPD_COLOR_BORDER      0xBBBBBB
#define EPD_COLOR_PROMPT_BG   0x1e1e00
#define EPD_COLOR_PROMPT_TXT  0xfffee6

/*********************************************************************************
 *                                   MACROS
 * *******************************************************************************/

/*********************************************************************************
 *                                  TYPEDEFS
 * *******************************************************************************/
enum {
    SCREEN0_ID = 0,
    SCREEN1_ID,
    SCREEN2_ID,
    SCREEN3_ID,
    SCREEN4_ID,
    SCREEN5_ID,
    SCREEN6_ID,
    SCREEN7_ID,
    SCREEN8_ID,
};

/*********************************************************************************
 *                              GLOBAL PROTOTYPES
 * *******************************************************************************/
void ui_epd47_entry(void);
// week fun (ui interface)
void ui_if_epd_refr(void);
void ui_epd_refr(uint16_t time, uint16_t cycle, uint16_t times);

void ui_if_epd_get_time(uint8_t *h, uint8_t *m, uint8_t *s);
void ui_if_epd_get_data(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *week);
// LORA
int ui_if_epd_get_LORA_mode(void);
void ui_if_epd_set_LORA_mode(int mode);
void ui_if_epd_LORA_send(const char *str);

// SD
void ui_if_epd_set_imgbtn(const char *text, int text_len, int type);
void ui_if_epd_read_from_SD(void);
// setting
void ui_if_epd_set_light(uint8_t light);
void ui_port_set_refr_cycle(uint16_t cycle);
void ui_port_set_refr_times(uint16_t times);
uint16_t ui_port_get_light(void);
uint16_t ui_port_get_refr_cycle(void);
uint16_t ui_port_get_refr_times(void);
// test
bool ui_if_epd_get_SD(void);
bool ui_if_epd_get_RTC(void);
bool ui_if_epd_get_TOUCH(void);
bool ui_if_epd_get_LORA(void);
bool ui_if_epd_get_WIFI(void);
// wifi
String ui_if_epd_get_WIFI_ip(void);
const char *ui_if_epd_get_WIFI_ssid(void);
const char *ui_if_epd_get_WIFI_pwd(void);
// battery
/* 25896 */
bool battery_25896_is_vaild(void);
bool battery_25896_is_chr(void);
void battery_25896_refr(void);
const char * battery_25896_get_VSYS_ST(void);
const char * battery_25896_get_VBUS_ST(void);
const char * battery_25896_get_CHG_ERR(void);
float battery_25896_get_VBUS(void);
float battery_25896_get_VSYS(void);
float battery_25896_get_VBAT(void);
float battery_25896_get_ICHG(void);
float battery_25896_get_TEMP(void);
float battery_25896_get_TSPCT(void);
/* 27220 */
bool battery_27220_is_vaild(void);
bool battery_27220_is_chr(void);
float battery_27220_get_VOLT(void);
float battery_27220_get_VOLT_CHG(void);
float battery_27220_get_CURR_ARG(void);
float battery_27220_get_CURR_INS(void);
float battery_27220_get_CURR_STD(void);
float battery_27220_get_CURR_CHG(void);
float battery_27220_get_TEMP(void);
float battery_27220_get_BATT_CAP(void);
float battery_27220_get_BATT_CAP_FULL(void);
#endif /* __UI_EPD47H__ */