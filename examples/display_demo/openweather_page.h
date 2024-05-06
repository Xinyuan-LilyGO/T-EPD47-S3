#ifndef OPENWEATHER_PAGE_H
#define OPENWEATHER_PAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

void create_openweather_page(lv_obj_t *menu);
lv_obj_t *get_openweather_obj(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif