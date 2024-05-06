#include "utlis.h"
#include "about_page.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>

static lv_obj_t * about_page = NULL;


void create_about_page(lv_obj_t *menu)
{
    lv_obj_t * section;
    lv_obj_t * cont;

    lv_obj_t * sub_software_info_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_software_info_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    section = lv_menu_section_create(sub_software_info_page);
    create_text(section, NULL, "Version 1.0", LV_MENU_ITEM_BUILDER_VARIANT_1);

    lv_obj_t * sub_legal_info_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_legal_info_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    section = lv_menu_section_create(sub_legal_info_page);
    for(uint32_t i=0; i<15; i++){
        create_text(section, NULL, "This is a long long long long long long long long long text, if it is long enough it may scroll.", LV_MENU_ITEM_BUILDER_VARIANT_1);
    }

    about_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(about_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(about_page);
    section = lv_menu_section_create(about_page);
    cont = create_text(section, NULL, "Software information", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont, sub_software_info_page);
    cont = create_text(section, NULL, "Legal information", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont, sub_legal_info_page);
}

lv_obj_t *get_about_page_obj(void)
{
    return about_page;
}