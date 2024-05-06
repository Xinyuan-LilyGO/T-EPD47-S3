#ifndef ABOUT_PAGE_H
#define ABOUT_PAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

void create_about_page(lv_obj_t *menu);
lv_obj_t *get_about_page_obj(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif