#ifndef MY_ASSETS_H
#define MY_ASSETS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

// image
LV_IMG_DECLARE(img_clock)
LV_IMG_DECLARE(img_lora)
LV_IMG_DECLARE(img_sd_card)
LV_IMG_DECLARE(img_setting)
LV_IMG_DECLARE(img_test)
LV_IMG_DECLARE(img_wifi)
LV_IMG_DECLARE(img_on_wifi)
LV_IMG_DECLARE(img_hand)
LV_IMG_DECLARE(img_hand_sec)
LV_IMG_DECLARE(img_rtc)
LV_IMG_DECLARE(img_touch)
LV_IMG_DECLARE(img_PNG)
LV_IMG_DECLARE(img_JPG)
LV_IMG_DECLARE(img_BMP)


// font
// LV_FONT_DECLARE(Font_Mono_Bold_14);
// LV_FONT_DECLARE(Font_Mono_Bold_16);
// LV_FONT_DECLARE(Font_Mono_Bold_18);
LV_FONT_DECLARE(Font_Mono_Bold_20);
LV_FONT_DECLARE(Font_Mono_Bold_25);
// LV_FONT_DECLARE(Font_Mono_Light_14);
LV_FONT_DECLARE(Font_Mono_Bold_30)
LV_FONT_DECLARE(Font_Mono_Bold_90)

LV_FONT_DECLARE(Font_Geist_Light_20)
LV_FONT_DECLARE(Font_Geist_Bold_20)

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif