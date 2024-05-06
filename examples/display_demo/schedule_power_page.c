#include "utlis.h"
#include "schedule_power_page.h"

#include <stdlib.h>
#include <stdbool.h>

static lv_obj_t * schedule_power_page = NULL;

static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    // lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t * kb = lv_event_get_user_data(e);
    if (code == LV_EVENT_PRESSED)
    {
        // LV_LOG_USER("State: %s\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }
}


static void ta_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    lv_obj_t * kb = lv_event_get_user_data(e);
    if(code == LV_EVENT_CLICKED) {
        lv_keyboard_set_textarea(kb, ta);
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ta, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    }

    if(code == LV_EVENT_LEAVE || code == LV_EVENT_DEFOCUSED) {
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ta, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    }

}


static void kb_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL)
    {
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }
}


void create_schedule_power_page(lv_obj_t *menu)
{
    lv_obj_t *section = NULL;
    lv_obj_t *kb = NULL;
    lv_obj_t *textarea = NULL;

    kb = lv_keyboard_create(lv_scr_act());
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUMBER);
    lv_obj_add_event_cb(kb, kb_event_cb, LV_EVENT_READY, NULL);
    lv_obj_add_event_cb(kb, kb_event_cb, LV_EVENT_CANCEL, NULL);

    schedule_power_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(schedule_power_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(schedule_power_page);
    // lv_menu_set_mode_header(schedule_power_page, LV_MENU_HEADER_TOP_UNFIXED);
    section = lv_menu_section_create(schedule_power_page);
    lv_obj_add_flag(schedule_power_page, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_add_flag(section, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

    lv_obj_t *obj = lv_menu_cont_create(section);

    lv_obj_t *btn = lv_btn_create(obj);
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, LV_SYMBOL_CLOSE);
    lv_obj_center(label);
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, kb);

    label = lv_label_create(obj);
    lv_obj_align(label, LV_ALIGN_CENTER , 0, 0);
    lv_label_set_text(label, "");
    lv_obj_set_flex_grow(label, 1);

    btn = lv_btn_create(obj);
    lv_obj_align(btn, LV_ALIGN_CENTER , 0, 0);
    label = lv_label_create(btn);
    lv_label_set_text(label, LV_SYMBOL_OK);
    lv_obj_center(label);
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, kb);

    lv_obj_set_style_text_font(section, &lv_font_montserrat_20, 0);
    textarea = create_textarea(section, NULL, "Power on time", true);
    lv_obj_add_event_cb(lv_obj_get_child(textarea, -1), ta_event_cb, LV_EVENT_ALL, kb);
    lv_textarea_set_accepted_chars(lv_obj_get_child(textarea, -1), "0123456789");
    lv_textarea_set_max_length(lv_obj_get_child(textarea, -1), 2);
    lv_textarea_set_placeholder_text(lv_obj_get_child(textarea, -1), "0 - 24");

    textarea = create_textarea(section, NULL, "Power off time", true);
    lv_obj_add_event_cb(lv_obj_get_child(textarea, -1), ta_event_cb, LV_EVENT_ALL, kb);
    lv_textarea_set_accepted_chars(lv_obj_get_child(textarea, -1), "0123456789");
    lv_textarea_set_max_length(lv_obj_get_child(textarea, -1), 2);
    lv_textarea_set_placeholder_text(lv_obj_get_child(textarea, -1), "0 - 24");
}

lv_obj_t *get_schedule_power_page_obj(void)
{
    return schedule_power_page;
}
