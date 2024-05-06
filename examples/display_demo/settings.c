
// #include <Arduino.h>

// #if LV_USE_MENU && LV_USE_MSGBOX
#include "settings.h"
#include "utlis.h"
#include "wifi_setting_page.h"
#include "about_page.h"
#include "openweather_page.h"
#include "ntp_page.h"
#include "schedule_power_page.h"


#include <lvgl.h>
#include <stdlib.h>

lv_obj_t * root_page;

extern lv_font_t user_icon;

#define USER_INFO_SYMBOL           "\xEF\x84\xA9"
#define USER_WEATHER_SYMBOL        "\xEF\x83\x82"
#define USER_NTP_SYMBOL            "\xEF\x80\x97"
#define USER_SCHEDULE_POWER_SYMBOL "\xEF\x80\x91"


static void back_event_handler(lv_event_t * e);
// static void switch_handler(lv_event_t * e);


void settings_ui()
{
    lv_obj_t * menu = lv_menu_create(lv_scr_act());
    lv_obj_t * cont = NULL;
    lv_obj_t * section = NULL;

    lv_obj_add_event_cb(menu, back_event_handler, LV_EVENT_CLICKED, menu);
    lv_obj_set_size(menu, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
    lv_obj_center(menu);

    create_setting_page(menu);

    create_about_page(menu);

    /** openweather page */
    create_openweather_page(menu);

    /** ntp page */
    create_ntp_page(menu);

    /** schedule_power_onoff page */
    create_schedule_power_page(menu);

    /*Create a root page*/
    root_page = lv_menu_page_create(menu, (char *)"Settings");
    lv_obj_set_style_pad_hor(root_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_obj_set_style_text_font(menu, &lv_font_montserrat_28, 0);

    /** Create network section */
    section = lv_menu_section_create(root_page);
    cont = create_text(section, LV_SYMBOL_WIFI, "WLAN", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_obj_set_style_text_font(lv_obj_get_child(cont, -1), &lv_font_montserrat_20, 0);
    lv_menu_set_load_page_event(menu, cont, get_setting_page_obj());

    /** Create info section */
    section = lv_menu_section_create(root_page);
    cont = create_text(section, USER_INFO_SYMBOL, "About", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_obj_set_style_text_font(lv_obj_get_child(cont, -1), &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_font(cont, &user_icon, 0);
    lv_menu_set_load_page_event(menu, cont, get_about_page_obj());

    /** Create app section */
    section = lv_menu_section_create(root_page);
    cont = create_text(section, USER_WEATHER_SYMBOL, "OpenWeather", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_obj_set_style_text_font(lv_obj_get_child(cont, -1), &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_font(cont, &user_icon, 0);
    lv_menu_set_load_page_event(menu, cont, get_openweather_obj());

    cont = create_text(section, USER_NTP_SYMBOL, "NTP", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_obj_set_style_text_font(lv_obj_get_child(cont, -1), &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_font(cont, &user_icon, 0);
    lv_menu_set_load_page_event(menu, cont, get_ntp_page_obj());

    cont = create_text(section, USER_SCHEDULE_POWER_SYMBOL, "Schedule power on/off", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_obj_set_style_text_font(lv_obj_get_child(cont, -1), &lv_font_montserrat_20, 0);
    lv_label_set_long_mode(lv_obj_get_child(cont, -1), LV_LABEL_LONG_CLIP);
    lv_obj_set_style_text_font(cont, &user_icon, 0);
    lv_menu_set_load_page_event(menu, cont, get_schedule_power_page_obj());

    /** 在侧边栏中设置菜单页 */
    lv_menu_set_sidebar_page(menu, root_page);
}


static void back_event_handler(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t * menu = lv_event_get_user_data(e);

    if(lv_menu_back_btn_is_root(menu, obj)) {
        lv_obj_t * mbox1 = lv_msgbox_create(NULL, "Hello", "Root back btn click.", NULL, true);
        lv_obj_center(mbox1);
    }
}

#if 0
static void switch_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * menu = lv_event_get_user_data(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        if(lv_obj_has_state(obj, LV_STATE_CHECKED)) {
            lv_menu_set_page(menu, NULL);
            lv_menu_set_sidebar_page(menu, root_page);
            lv_event_send(lv_obj_get_child(lv_obj_get_child(lv_menu_get_cur_sidebar_page(menu), 0), 0), LV_EVENT_CLICKED, NULL);
        }else {
            lv_menu_set_sidebar_page(menu, NULL);
            lv_menu_clear_history(menu); /* Clear history because we will be showing the root page later */
            lv_menu_set_page(menu, root_page);
        }
    }
}
#endif




// #endif