#ifndef SCHEDULE_POWER_PAGE_H_
#define SCHEDULE_POWER_PAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

void create_schedule_power_page(lv_obj_t *menu);
lv_obj_t * get_schedule_power_page_obj(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif