#include "utlis.h"
#include "openweather_page.h"

#include <stdlib.h>
#include <stdbool.h>

static lv_obj_t * sub_openweather_page = NULL;

static void ta_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    lv_obj_t * kb = lv_event_get_user_data(e);

    if(code == LV_EVENT_CLICKED) {
        lv_keyboard_set_textarea(kb, ta);
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }

    if( code == LV_EVENT_LEAVE || code == LV_EVENT_DEFOCUSED ) {
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }
}


static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    // lv_obj_t * obj = lv_event_get_target(e);

    if (code == LV_EVENT_PRESSED)
    {
        // LV_LOG_USER("State: %s\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
    }
}


static void kb_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if (code == LV_EVENT_READY || code == LV_EVENT_DEFOCUSED)
    {
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }
}


void create_openweather_page(lv_obj_t *menu)
{
    lv_obj_t *section = NULL;
    lv_obj_t *kb = NULL;
    lv_obj_t *textarea = NULL;
    lv_obj_t *btn = NULL;

    kb = lv_keyboard_create(lv_scr_act());
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_event_cb(kb, kb_event_cb, LV_EVENT_READY, NULL);

    sub_openweather_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_openweather_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_openweather_page);
    section = lv_menu_section_create(sub_openweather_page);


    lv_obj_t *obj = lv_menu_cont_create(section);

    btn = lv_btn_create(obj);
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, LV_SYMBOL_CLOSE);
    lv_obj_center(label);
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);

    label = lv_label_create(obj);
    lv_obj_align(label, LV_ALIGN_CENTER , 0, 0);
    lv_label_set_text(label, "");
    lv_obj_set_flex_grow(label, 1);

    btn = lv_btn_create(obj);
    lv_obj_align(btn, LV_ALIGN_CENTER , 0, 0);
    label = lv_label_create(btn);
    lv_label_set_text(label, LV_SYMBOL_OK);
    lv_obj_center(label);
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);


    lv_obj_set_style_text_font(section, &lv_font_montserrat_20, 0);
    textarea = create_textarea(section, NULL, "apikey", true);
    lv_obj_add_event_cb(lv_obj_get_child(textarea, -1), ta_event_cb, LV_EVENT_ALL, kb);
    lv_textarea_set_placeholder_text(lv_obj_get_child(textarea, -1), "apikey");

    textarea = create_textarea(section, NULL, "Country", true);
    lv_obj_add_event_cb(lv_obj_get_child(textarea, -1), ta_event_cb, LV_EVENT_ALL, kb);
    lv_textarea_set_placeholder_text(lv_obj_get_child(textarea, -1), "e.g. CN");

    textarea = create_textarea(section, NULL, "City", true);
    lv_obj_add_event_cb(lv_obj_get_child(textarea, -1), ta_event_cb, LV_EVENT_ALL, kb);
    lv_textarea_set_placeholder_text(lv_obj_get_child(textarea, -1), "e.g. Shenzhen");

    create_dropdown(section, NULL, "Hemisphere", true, "north\n""south\n");
    create_dropdown(section, NULL, "Units", true, "Metric\n""Imperial\n");

    lv_keyboard_set_textarea(kb, NULL);
}

lv_obj_t *get_openweather_obj(void)
{
    return sub_openweather_page;
}