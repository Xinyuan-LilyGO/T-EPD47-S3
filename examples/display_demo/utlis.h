#ifndef UTILS_H_
#define UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <lvgl.h>

enum {
    LV_MENU_ITEM_BUILDER_VARIANT_1,
    LV_MENU_ITEM_BUILDER_VARIANT_2
};
typedef uint8_t lv_menu_builder_variant_t;

lv_obj_t *create_text(lv_obj_t     *parent, const char *icon, const char *txt, lv_menu_builder_variant_t builder_variant);
lv_obj_t *create_slider(lv_obj_t   *parent, const char *icon, const char *txt, int32_t min, int32_t max, int32_t val);
lv_obj_t *create_switch(lv_obj_t *parent, const char *icon, const char *txt, bool chk);
lv_obj_t *create_textarea(lv_obj_t *parent, const char *icon, const char *txt, bool chk);
lv_obj_t *create_dropdown(lv_obj_t *parent, const char *icon, const char *txt, bool chk, const char * options);
lv_obj_t *create_button(lv_obj_t *parent, const char *icon, const char *txt, bool chk);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif