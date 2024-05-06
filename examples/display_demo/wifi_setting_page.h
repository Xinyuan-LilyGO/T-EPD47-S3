#ifndef WIFI_SETTING_PAGE_H
#define WIFI_SETTING_PAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

void create_setting_page(lv_obj_t *menu);
lv_obj_t * get_setting_page_obj(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif