#ifndef NTP_PAGE_H
#define NTP_PAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

void create_ntp_page(lv_obj_t *menu);
lv_obj_t * get_ntp_page_obj(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif