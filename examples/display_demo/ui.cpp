
#include "ui.h"

LV_IMG_DECLARE(img_battery_32);
LV_IMG_DECLARE(img_light_32);
LV_IMG_DECLARE(img_lora_32);
LV_IMG_DECLARE(img_nfc_32);
LV_IMG_DECLARE(img_setting_32);
LV_IMG_DECLARE(img_wifi_32);
LV_IMG_DECLARE(img_dev_32);
LV_IMG_DECLARE(img_info_72);
LV_IMG_DECLARE(img_info_96);
LV_IMG_DECLARE(img_info_128);

// font
LV_FONT_DECLARE(Font_Mono_Bold_14);
LV_FONT_DECLARE(Font_Mono_Bold_16);
LV_FONT_DECLARE(Font_Mono_Bold_18);
LV_FONT_DECLARE(Font_Mono_Bold_20);
LV_FONT_DECLARE(Font_Mono_Light_14);


void ui_entry(void)
{
    lv_obj_t *scr5_cont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(scr5_cont, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(scr5_cont, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(scr5_cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_width(scr5_cont, 0, LV_PART_MAIN);
    // lv_obj_set_style_pad_all(scr5_cont, 0, LV_PART_MAIN);
    lv_obj_set_flex_flow(scr5_cont, LV_FLEX_FLOW_ROW_WRAP);

    lv_obj_t * img1 = lv_img_create(scr5_cont);
    lv_obj_t * img2 = lv_img_create(scr5_cont);
    lv_obj_t * img3 = lv_img_create(scr5_cont);
    lv_obj_t * img4 = lv_img_create(scr5_cont);
    lv_obj_t * img5 = lv_img_create(scr5_cont);

    lv_img_set_src(img1, &img_nfc_32);
    lv_img_set_src(img2, &img_nfc_32);
    lv_img_set_src(img3, &img_info_96);
    lv_img_set_src(img4, &img_info_72);
    lv_img_set_src(img5, &img_info_128);

    // lv_obj_add_event_cb(img1, tabview_clk_event, LV_EVENT_CLICKED, (void *)1);
    // lv_obj_add_event_cb(img2, tabview_clk_event, LV_EVENT_CLICKED, (void *)2);
    // lv_obj_add_event_cb(img3, tabview_clk_event, LV_EVENT_CLICKED, (void *)3);
    // lv_obj_add_event_cb(img4, tabview_clk_event, LV_EVENT_CLICKED, (void *)4);
    // lv_obj_add_event_cb(img5, tabview_clk_event, LV_EVENT_CLICKED, (void *)5);
}