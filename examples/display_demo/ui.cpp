#include "ui.h"
#include "src/assets.h"
#include "scr_mrg.h"
#include "Arduino.h"

#define EPD_REFRESH_TIME 200

//************************************[ Other fun ]******************************************
void scr_back_btn_create(lv_obj_t *parent, const char *text, lv_event_cb_t cb)
{
    lv_obj_t * btn = lv_btn_create(parent);
    lv_obj_set_style_pad_all(btn, 0, 0);
    lv_obj_set_height(btn, 50);
    lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 15, 15);
    lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, lv_color_hex(EPD_COLOR_BG), LV_PART_MAIN);
    lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *label2 = lv_label_create(btn);
    lv_obj_align(label2, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_text_color(label2, lv_color_hex(EPD_COLOR_TEXT), LV_PART_MAIN);
    lv_label_set_text(label2, " " LV_SYMBOL_LEFT);

    lv_obj_t *label = lv_label_create(parent);
    lv_obj_align_to(label, btn, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    lv_obj_set_style_text_font(label, &Font_Mono_Bold_30, LV_PART_MAIN);
    lv_obj_set_style_text_color(label, lv_color_hex(EPD_COLOR_TEXT), LV_PART_MAIN);
    lv_label_set_text(label, text);
    lv_obj_add_flag(label, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(label, cb, LV_EVENT_CLICKED, NULL);
    lv_obj_set_ext_click_area(label, 30);
}

void scr_middle_line(lv_obj_t *parent)
{
    static lv_point_t line_points[] = { {LCD_HOR_SIZE/2, 0}, {LCD_HOR_SIZE/2, LCD_VER_SIZE-150}};
    /*Create style*/
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 2);
    lv_style_set_line_color(&style_line, lv_color_black());
    lv_style_set_line_rounded(&style_line, true);
    /*Create a line and apply the new style*/
    lv_obj_t * line1;
    line1 = lv_line_create(parent);
    lv_line_set_points(line1, line_points, 2);     /*Set the points*/
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_set_align(line1, LV_ALIGN_LEFT_MID);
}
//************************************[ screen 0 ]****************************************** menu
#if 1
#define MENU_ICON_NUM  (8)
#define MENU_CONT_HIGH (LCD_VER_SIZE * 0.84)

/*** UI interfavce ***/
void __attribute__((weak)) ui_if_epd_refr(uint16_t time) {}
// end

lv_obj_t *scr0_cont;
struct menu_icon {
    const void *icon_src;
    const char *icon_str;
};

const struct menu_icon icon_buf[MENU_ICON_NUM] = {
    {&img_clock, "clock"}, 
    {&img_lora, "lora"},
    {&img_sd_card, "sd card"},
    {&img_setting, "setting"},
    {&img_test, "test"},
    {&img_wifi, "wifi"},
    {&img_battery, "battery"},
    {&img_shutdown, "shutdown"},
};

static void menu_btn_event(lv_event_t *e)
{
    int data = (int)e->user_data;
    if(e->code == LV_EVENT_CLICKED) {
        // printf("%s is clicked.\n", icon_buf[data].icon_str);
        
        switch (data) {
            case 0: scr_mgr_push(SCREEN1_ID, false); ui_if_epd_refr(EPD_REFRESH_TIME); break;
            case 1: scr_mgr_push(SCREEN2_ID, false); ui_if_epd_refr(EPD_REFRESH_TIME); break;
            case 2: scr_mgr_push(SCREEN3_ID, false); ui_if_epd_refr(500); break;
            case 3: scr_mgr_push(SCREEN4_ID, false); ui_if_epd_refr(EPD_REFRESH_TIME); break;
            case 4: scr_mgr_push(SCREEN5_ID, false); ui_if_epd_refr(EPD_REFRESH_TIME); break;
            case 5: scr_mgr_push(SCREEN6_ID, false); ui_if_epd_refr(EPD_REFRESH_TIME); break;
            case 6: scr_mgr_push(SCREEN7_ID, false); ui_if_epd_refr(EPD_REFRESH_TIME); break;
            case 7: scr_mgr_push(SCREEN8_ID, false); ui_if_epd_refr(EPD_REFRESH_TIME); break;
            default: break;
        }
    }
}

static void create0(lv_obj_t *parent) 
{
    lv_obj_t *scr0_cont = lv_obj_create(parent);
    lv_obj_set_size(scr0_cont, lv_pct(100), MENU_CONT_HIGH);
    lv_obj_set_style_bg_color(scr0_cont, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(scr0_cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_width(scr0_cont, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(scr0_cont, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr0_cont, 0, LV_PART_MAIN);
    lv_obj_align(scr0_cont, LV_ALIGN_BOTTOM_MID, 0, 0);

    int row_n = 2;
    int col_n = 5;
    lv_obj_t *img_buf[MENU_ICON_NUM] = {0};
    lv_obj_t *lab_buf[MENU_ICON_NUM] = {0};
    
    for(int i = 0; i < MENU_ICON_NUM; i++) {
        lv_obj_t *img = lv_img_create(scr0_cont);
        lv_img_set_src(img, icon_buf[i].icon_src);
        lv_obj_add_flag(img, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(img, menu_btn_event, LV_EVENT_CLICKED, (void *)i);
        lv_obj_t *label = lv_label_create(scr0_cont);
        lv_label_set_text(label, icon_buf[i].icon_str);
        img_buf[i] = img;
        lab_buf[i] = label;
    }

    lv_coord_t x_gap = (LCD_HOR_SIZE-(img_clock.header.w * col_n)) / (col_n+1);
    lv_coord_t y_gap = (MENU_CONT_HIGH / 5);

    // printf("x=%d, y=%d\n", x_gap, y_gap);

    // row 1
    lv_obj_align_to(img_buf[0], scr0_cont, LV_ALIGN_TOP_LEFT, x_gap, 0);
    for(int i = 1; i < col_n; i++) {
        lv_obj_align_to(img_buf[i], img_buf[i-1], LV_ALIGN_OUT_RIGHT_MID, x_gap, 0);
    }

    if(MENU_ICON_NUM > col_n) {
        // row 2
        lv_obj_align_to(img_buf[col_n], img_buf[0], LV_ALIGN_OUT_BOTTOM_MID, 0, y_gap);
        for(int i = col_n+1; i < MENU_ICON_NUM; i++) {
            lv_obj_align_to(img_buf[i], img_buf[i-1], LV_ALIGN_OUT_RIGHT_MID, x_gap, 0);
        }
    }

    for(int i = 0; i < MENU_ICON_NUM; i++) {
        lv_obj_align_to(lab_buf[i], img_buf[i], LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    }
}
static void entry0(void) { }
static void exit0(void) { }
static void destroy0(void) { }

static scr_lifecycle_t screen0 = {
    .create = create0,
    .entry =   entry0,
    .exit  =   exit0,
    .destroy = destroy0,
};
#endif
//************************************[ screen 1 ]****************************************** clock
#if 1
/*** UI interfavce ***/
void __attribute__((weak)) ui_if_epd_get_time(uint8_t *h, uint8_t *m, uint8_t *s) {
    *h = 0;
    *m = 0;
    *s = 0;
}
void __attribute__((weak)) ui_if_epd_get_data(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *week) {
    *year = 0;
    *month = 0;
    *day = 0;
    *week = 0;
}
// end

static lv_obj_t  * calendar;
static lv_timer_t *get_timer = NULL;
lv_meter_indicator_t * indic_min;
lv_meter_indicator_t * indic_hour;
static lv_obj_t *clock_time;
static lv_obj_t *clock_data;
static lv_obj_t *clock_ap;
static lv_obj_t *clock_month;
static const char *week_list_en[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
static const char * month_names_def[12] = LV_CALENDAR_DEFAULT_MONTH_NAMES;
static bool get_refresh_data(void)
{
    uint8_t h, m, s;
    uint8_t year, mont, day, week;

    ui_if_epd_get_time(&h, &m, &s);
    ui_if_epd_get_data(&year, &mont, &day, &week);

    if(h > 12) {
        lv_label_set_text_fmt(clock_ap, "%s", "P.M.");
    }  else {
        lv_label_set_text_fmt(clock_ap, "%s", "A.M.");
    }

    lv_calendar_set_today_date(calendar, 2000+year, mont, day);
    lv_calendar_set_showed_date(calendar, 2000+year, mont);
    lv_label_set_text_fmt(clock_month, "%s", month_names_def[mont-1]);

    lv_label_set_text_fmt(clock_time, "%02d:%02d", h%12, m);
    lv_label_set_text_fmt(clock_data, "20%02d-%02d-%02d  %s", year, mont, day, week_list_en[week]);

    printf("%2d:%2d:%02d-%d/%d/%d\n", h, m, s, year, mont, day);

    return year;
}

static void get_timer_event(lv_timer_t *t) 
{
    // refresh time per 65s
    bool is_ref = get_refresh_data();
    if(is_ref) {
        lv_timer_set_period(get_timer, 65*1000);
        ui_if_epd_refr(EPD_REFRESH_TIME);
    }
}

static void scr1_btn_event_cb(lv_event_t * e)
{
    if(e->code == LV_EVENT_CLICKED){
        ui_if_epd_refr(EPD_REFRESH_TIME);
        scr_mgr_switch(SCREEN0_ID, false);
    }
}

static void create1(lv_obj_t *parent) {
    clock_time = lv_label_create(parent);
    clock_data = lv_label_create(parent);
    clock_ap = lv_label_create(parent);
    clock_month = lv_label_create(parent);

    lv_obj_set_style_border_width(clock_data, 2, 0);
    lv_obj_set_style_pad_top(clock_data, 30, 0);
    lv_obj_set_style_border_side(clock_data, LV_BORDER_SIDE_TOP, LV_PART_MAIN);

    lv_obj_set_style_text_font(clock_time, &Font_Mono_Bold_90, LV_PART_MAIN);
    lv_obj_set_style_text_font(clock_data, &Font_Mono_Bold_30, LV_PART_MAIN);
    lv_obj_set_style_text_font(clock_ap, &Font_Mono_Bold_30, LV_PART_MAIN);
    lv_obj_set_style_text_font(clock_month, &Font_Mono_Bold_30, LV_PART_MAIN);

    //---------------------
    calendar = lv_calendar_create(parent);
    lv_obj_set_size(calendar, 380, 350);
    lv_obj_align(calendar, LV_ALIGN_RIGHT_MID, -50, 27);
    lv_obj_set_style_text_font(calendar, &Font_Geist_Bold_20, LV_PART_MAIN);

    lv_obj_set_style_border_width(calendar, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(lv_calendar_get_btnmatrix(calendar), 0, LV_PART_ITEMS);
    lv_obj_set_style_border_side(lv_calendar_get_btnmatrix(calendar), LV_BORDER_SIDE_TOP, LV_PART_MAIN);

    //---------------------
    scr_middle_line(parent);

    // back
    scr_back_btn_create(parent, "Clock", scr1_btn_event_cb); 
}
static void entry1(void) {
    // refresh time
    bool is_ref = get_refresh_data();
    if(is_ref) {
        get_timer = lv_timer_create(get_timer_event, 60*1000, NULL);
    } else {
        get_timer = lv_timer_create(get_timer_event, 6000, NULL);
    }
    
    // layout
    lv_obj_set_style_text_align(clock_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_text_align(clock_data, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_text_align(clock_ap, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_align(clock_time, LV_ALIGN_LEFT_MID, 70, -50);
    lv_obj_align_to(clock_data, clock_time, LV_ALIGN_OUT_BOTTOM_MID, 20, 30);
    lv_obj_align_to(clock_ap, clock_time, LV_ALIGN_OUT_RIGHT_MID, 5, 20);
    lv_obj_align_to(clock_month, calendar, LV_ALIGN_OUT_TOP_RIGHT, 0, -5);
}
static void exit1(void) {
    if(get_timer) {
        lv_timer_del(get_timer);
        get_timer = NULL;
    }
}
static void destroy1(void) { }

static scr_lifecycle_t screen1 = {
    .create = create1,
    .entry = entry1,
    .exit  = exit1,
    .destroy = destroy1,
};
#endif
//************************************[ screen 2 ]****************************************** lora
#if 1
static lv_obj_t *scr2_cont;
static lv_obj_t *scr2_cont_info;
static lv_obj_t *lora_mode_sw;
// static lv_obj_t *lora_open_sw;
static lv_obj_t *lora_lab_buf[11] = {0};
static lv_obj_t *lora_lab_mode;
static lv_obj_t *lora_lab_time;
static lv_timer_t *lora_send_timer = NULL;

int lora_sr_cnt = 0;
int lora_lab_cnt = 1;

static lv_obj_t * scr2_create_label(lv_obj_t *parent)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_obj_set_width(label, LCD_HOR_SIZE/2-50);
    lv_obj_set_style_text_font(label, &Font_Mono_Bold_25, LV_PART_MAIN);   
    // lv_obj_set_style_border_width(label, 1, LV_PART_MAIN);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    return label;
}

static void lora_send_timer_event(lv_timer_t *t)
{
    if(ui_if_epd_get_LORA() == false) return;
    
    int mode = ui_if_epd_get_LORA_mode();

    if(mode == 0) // send
    {
        String str = "Lora Send #" + String(lora_sr_cnt++);
        ui_lora_transmit(str.c_str());

        if(lora_lab_buf[lora_lab_cnt] == NULL) {
            lora_lab_buf[lora_lab_cnt] = scr2_create_label(scr2_cont_info);
            lv_label_set_text(lora_lab_buf[lora_lab_cnt], str.c_str());
        } else {
            lv_label_set_text(lora_lab_buf[lora_lab_cnt], str.c_str());
        }

        lora_lab_cnt++;
        if(lora_lab_cnt >= 10) {
            lora_lab_cnt = 1;
        }
        ui_if_epd_refr(EPD_REFRESH_TIME);
    } 
    else if(mode == 1)
    {
        String str = "";

        if(lora_recv_success) {
            lora_recv_success = false;

            str += lora_recv_data;

            lv_label_set_text_fmt(lora_lab_buf[0], "RECV: %ddm", lora_recv_rssi);

            if(lora_lab_buf[lora_lab_cnt] == NULL) {
                lora_lab_buf[lora_lab_cnt] = scr2_create_label(scr2_cont_info);
                lv_label_set_text(lora_lab_buf[lora_lab_cnt], str.c_str());
            } else {
                lv_label_set_text(lora_lab_buf[lora_lab_cnt], str.c_str());
            }

            lora_lab_cnt++;
            if(lora_lab_cnt >= 10) {
                lora_lab_cnt = 1;
            }
            ui_if_epd_refr(EPD_REFRESH_TIME);
        }
    }
}

static void lora_mode_sw_event(lv_event_t * e)
{
    if(e->code == LV_EVENT_CLICKED){
        int mode = ui_if_epd_get_LORA_mode();

        // if(e->target == lora_open_sw) {
        //     static int open = 0;
        //     lv_obj_t *lab = (lv_obj_t *)e->user_data;
            
        //     if(mode == 0 && open == 0) {
        //         lv_timer_resume(lora_send_timer);
        //         lv_label_set_text(lab, "SEND CLOSE");
        //     }
        //     if(mode == 0 && open == 1) {
        //         lv_timer_pause(lora_send_timer);
        //         lv_label_set_text(lab, "SEND OPEN");
        //         ui_if_epd_refr(EPD_REFRESH_TIME);
        //     }
        //     open = !open;

        // } else 
        {
            mode = !mode;
            if(mode == LORA_MODE_SEND) { // send
                lv_obj_clear_flag(lora_lab_time, LV_OBJ_FLAG_HIDDEN);
                lv_label_set_text_fmt(lora_lab_mode, "%s : %s", "Mode", "send");
                lv_label_set_text(lora_lab_buf[0], "SEND:");
                lora_sr_cnt = 1;
                // lv_timer_resume(lora_send_timer);
            } else if(mode == LORA_MODE_RECV) { // recv
                lv_obj_add_flag(lora_lab_time, LV_OBJ_FLAG_HIDDEN);
                lv_label_set_text_fmt(lora_lab_mode, "%s : %s", "Mode", "recv");
                lv_label_set_text(lora_lab_buf[0], "RECV:");
                for(int i = 1; i < 11; i++) {
                    if(lora_lab_buf[i])
                        lv_label_set_text(lora_lab_buf[i], " ");
                }
                // lv_timer_pause(lora_send_timer);
            }
            lora_lab_cnt = 1;
            ui_if_epd_set_LORA_mode(mode);
            ui_if_epd_refr(EPD_REFRESH_TIME);
        }
    }
}

static void scr2_btn_event_cb(lv_event_t * e)
{
    if(e->code == LV_EVENT_CLICKED){
        ui_if_epd_refr(EPD_REFRESH_TIME);
        scr_mgr_switch(SCREEN0_ID, false);
    }
}

static void create2(lv_obj_t *parent) {

    scr2_cont = lv_obj_create(parent);
    lv_obj_set_size(scr2_cont, lv_pct(49), lv_pct(85));
    lv_obj_set_style_bg_color(scr2_cont, lv_color_hex(EPD_COLOR_BG), LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(scr2_cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(scr2_cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(scr2_cont, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr2_cont, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_left(scr2_cont, 50, LV_PART_MAIN);
    lv_obj_set_flex_flow(scr2_cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(scr2_cont, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_column(scr2_cont, 5, LV_PART_MAIN);
    lv_obj_set_align(scr2_cont, LV_ALIGN_BOTTOM_LEFT);

    scr2_cont_info = lv_obj_create(parent);
    lv_obj_set_size(scr2_cont_info, lv_pct(49), lv_pct(85));
    lv_obj_set_style_bg_color(scr2_cont_info, lv_color_hex(EPD_COLOR_BG), LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(scr2_cont_info, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(scr2_cont_info, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(scr2_cont_info, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr2_cont_info, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_left(scr2_cont_info, 50, LV_PART_MAIN);
    lv_obj_set_flex_flow(scr2_cont_info, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(scr2_cont_info, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_column(scr2_cont_info, 5, LV_PART_MAIN);
    lv_obj_set_align(scr2_cont_info, LV_ALIGN_BOTTOM_RIGHT);

    lv_obj_t *label;
    //
    lora_mode_sw = lv_btn_create(parent);
    lv_obj_set_style_radius(lora_mode_sw, 5, LV_PART_MAIN);
    lv_obj_set_style_border_width(lora_mode_sw, 2, LV_PART_MAIN);
    label = lv_label_create(lora_mode_sw);
    lv_obj_set_style_text_font(label, &Font_Mono_Bold_25, LV_PART_MAIN);   
    lv_label_set_text(label, "MODE SW");
    lv_obj_add_event_cb(lora_mode_sw, lora_mode_sw_event, LV_EVENT_CLICKED, NULL);

    // lora_open_sw = lv_btn_create(parent);
    // lv_obj_set_style_radius(lora_open_sw, 5, LV_PART_MAIN);
    // lv_obj_set_style_border_width(lora_open_sw, 2, LV_PART_MAIN);
    // label = lv_label_create(lora_open_sw);
    // lv_obj_set_style_text_font(label, &Font_Mono_Bold_25, LV_PART_MAIN);   
    // lv_label_set_text(label, "SEND OPEN");
    // lv_obj_add_event_cb(lora_open_sw, lora_mode_sw_event, LV_EVENT_CLICKED, label);

    // --------------- LEFT ---------------
    scr_middle_line(parent);

    if(ui_if_epd_get_LORA() == true) 
    {
        lora_lab_mode = scr2_create_label(scr2_cont);
        int mode = ui_if_epd_get_LORA_mode();
        if(mode == LORA_MODE_SEND ){
            lv_label_set_text_fmt(lora_lab_mode, "%s : %s", "Mode", "send");
        } else {
            lv_label_set_text_fmt(lora_lab_mode, "%s : %s", "Mode", "recv");
        }

        lora_lab_time = scr2_create_label(scr2_cont);
        lv_label_set_text_fmt(lora_lab_time, "%s : %ds", "SendTimeInterval", 5);

        label = scr2_create_label(scr2_cont);
        lv_label_set_text_fmt(label, "%s : %0.1fMHz", "Frequency", 850.0);

        label = scr2_create_label(scr2_cont);
        lv_label_set_text_fmt(label, "%s : %0.1fKHz", "Bandwidth", 125.0);

        label = scr2_create_label(scr2_cont);
        lv_label_set_text_fmt(label, "%s : %d", "SpreadingFactor", 10);

        label = scr2_create_label(scr2_cont);
        lv_label_set_text_fmt(label, "%s : %d", "CodingRate", 6);

        label = scr2_create_label(scr2_cont);
        lv_label_set_text_fmt(label, "%s : 0x%X", "SyncWord", 0xAB);

        label = scr2_create_label(scr2_cont);
        lv_label_set_text_fmt(label, "%s : %d", "OutputPower", 10);

        label = scr2_create_label(scr2_cont);
        lv_label_set_text_fmt(label, "%s : %d", "CurrentLimit", 140);

        label = scr2_create_label(scr2_cont);
        lv_label_set_text_fmt(label, "%s : %d", "PreambleLength", 15);
        
        // --------------- RIGHT ---------------
        lora_lab_buf[0] = scr2_create_label(scr2_cont_info);
        // lv_obj_set_style_text_align(lora_lab_buf[0], LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
        if(mode == LORA_MODE_SEND ){
            ui_if_epd_set_LORA_mode(LORA_MODE_SEND);
        } else {
            lv_label_set_text(lora_lab_buf[0], "RECV:");
        }
    }

    // back
    scr_back_btn_create(parent, "Lora", scr2_btn_event_cb);
}
static void entry2(void) {
    // ui_lora_standby();

    lora_lab_cnt = 1;

    lv_obj_align(lora_mode_sw, LV_ALIGN_TOP_MID, 0, 22);
    
    if(ui_if_epd_get_LORA() == true)
    {
        lora_send_timer = lv_timer_create(lora_send_timer_event, 5000, NULL);
    }
}
static void exit2(void) {
    if(ui_if_epd_get_LORA() == true)
        ui_lora_sleep();
}
static void destroy2(void) {
    if(lora_send_timer) {
        lv_timer_del(lora_send_timer);
        lora_send_timer = NULL;
    }

    for(int i = 0; i < 11; i++) {
        lora_lab_buf[i] = NULL;
    }
}

static scr_lifecycle_t screen2 = {
    .create = create2,
    .entry = entry2,
    .exit  = exit2,
    .destroy = destroy2,
};

/*** UI interfavce ***/
int __attribute__((weak)) ui_if_epd_get_LORA_mode(void) {
    return false;
}
void __attribute__((weak)) ui_if_epd_set_LORA_mode(int mode) {
}
void __attribute__((weak)) ui_if_epd_LORA_send(const char *str) {
}

// end

#endif
//************************************[ screen 3 ]****************************************** sd_card
#if 1

lv_obj_t *scr3_cont_file;
lv_obj_t *scr3_cont_img;
lv_obj_t *sd_info;
lv_obj_t *ui_photos_img;

static void read_img_btn_event(lv_event_t * e)
{
    char *file_name = lv_label_get_text((lv_obj_t *)e->user_data);

    if(e->code = LV_EVENT_CLICKED) {
        
        static char path[32];
        lv_snprintf(path, 32, "S:/%s", file_name);
        lv_img_set_src(ui_photos_img, path);
        printf("event [%s]\n", path);

        ui_if_epd_refr(500);
    }
}

static void scr3_btn_event_cb(lv_event_t * e)
{
    if(e->code == LV_EVENT_CLICKED){
        ui_if_epd_refr(EPD_REFRESH_TIME);
        scr_mgr_switch(SCREEN0_ID, false);
    }
}

static void scr3_add_img_btn(const char *text, int text_len, int type)
{
    char buf[16] = {0};
    strncpy(buf, text, 16);
    char *suffix = (char *)text + text_len - 4;
    buf[text_len - 4] = '\0';

    printf("imgbtn [%s][%d][%s]\n", text, text_len, suffix);

    lv_obj_t *obj = lv_obj_create(scr3_cont_file);
    lv_obj_set_size(obj, LCD_HOR_SIZE/11, LCD_HOR_SIZE/11);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_all(obj, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN);

    lv_obj_t *img = lv_img_create(obj);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, -10);
    lv_obj_add_flag(img, LV_OBJ_FLAG_CLICKABLE);

    switch (type) {
        case 1: lv_img_set_src(img, &img_JPG); break;
        case 2: lv_img_set_src(img, &img_PNG); break;
        case 3: lv_img_set_src(img, &img_BMP); break;
        default:
            break;
    }

    lv_obj_t *lab = lv_label_create(obj);
    lv_obj_set_style_text_font(lab, &Font_Mono_Bold_20, LV_PART_MAIN);
    lv_label_set_text(lab, buf); // File suffixes are not displayed
    lv_obj_align_to(lab, img, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    lv_obj_t *lab1 = lv_label_create(obj);
    lv_label_set_text(lab1, text); 
    lv_obj_add_flag(lab1, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_event_cb(img, read_img_btn_event, LV_EVENT_CLICKED, lab1);
}


static void create3(lv_obj_t *parent) {
    scr3_cont_file = lv_obj_create(parent);
    lv_obj_set_size(scr3_cont_file, lv_pct(49), lv_pct(85));
    lv_obj_set_style_bg_color(scr3_cont_file, lv_color_hex(EPD_COLOR_BG), LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(scr3_cont_file, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_width(scr3_cont_file, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr3_cont_file, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_hor(scr3_cont_file, 5, LV_PART_MAIN);
    lv_obj_set_flex_flow(scr3_cont_file, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_style_pad_row(scr3_cont_file, 5, LV_PART_MAIN);
    lv_obj_set_style_pad_column(scr3_cont_file, 5, LV_PART_MAIN);
    lv_obj_set_align(scr3_cont_file, LV_ALIGN_BOTTOM_LEFT);

    scr3_cont_img = lv_obj_create(parent);
    lv_obj_set_size(scr3_cont_img, lv_pct(49), lv_pct(85));
    lv_obj_set_style_bg_color(scr3_cont_img, lv_color_hex(EPD_COLOR_BG), LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(scr3_cont_img, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_width(scr3_cont_img, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr3_cont_img, 0, LV_PART_MAIN);
    lv_obj_set_align(scr3_cont_img, LV_ALIGN_BOTTOM_RIGHT);

    //---------------------
    ui_photos_img = lv_img_create(scr3_cont_img);
    lv_obj_align(ui_photos_img, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *lab1;
    if(ui_if_epd_get_SD()) {
        ui_if_epd_read_from_SD();

        //---------------------
        scr_middle_line(parent);

        sd_info = lv_label_create(parent);
        lv_obj_set_style_text_font(sd_info, &Font_Mono_Bold_30, LV_PART_MAIN);
        lv_label_set_text(sd_info, "SD GALLERY"); 
    } else {
        sd_info = lv_label_create(parent);
        lv_obj_set_style_text_font(sd_info, &Font_Mono_Bold_30, LV_PART_MAIN);
        lv_label_set_text(sd_info, "NO FIND SD CARD!"); 
    }

    // back
    scr_back_btn_create(parent, "SD", scr3_btn_event_cb);
}
static void entry3(void) 
{
    // lv_obj_align(scr3_cont, LV_ALIGN_BOTTOM_MID, 0, 0);

    if(ui_if_epd_get_SD()) {
        lv_obj_align(sd_info, LV_ALIGN_TOP_MID, 0, 22);
    } else {
        lv_obj_center(sd_info);
    }
}
static void exit3(void) { }
static void destroy3(void) {
}

static scr_lifecycle_t screen3 = {
    .create = create3,
    .entry = entry3,
    .exit  = exit3,
    .destroy = destroy3,
};

/*** UI interfavce ***/
void __attribute__((weak)) ui_if_epd_set_imgbtn(const char *text, int text_len, int type) 
{
    scr3_add_img_btn(text, text_len, type);
}
void __attribute__((weak)) ui_if_epd_read_from_SD(void) 
{

}
// end
#endif
//************************************[ screen 4 ]****************************************** setting
#if 1
lv_obj_t * ui_Slider1;
lv_obj_t * ui_Slider2;
lv_obj_t * ui_Slider3;
lv_obj_t * ui_label1;
lv_obj_t * ui_Label2;
lv_obj_t * ui_Label3;
lv_obj_t * ui_backlight;
lv_obj_t * ui_refr_cycle;
lv_obj_t * ui_refr_time;

static void scr4_btn_event_cb(lv_event_t * e)
{
    if(e->code == LV_EVENT_CLICKED) {
        ui_if_epd_refr(EPD_REFRESH_TIME);
        scr_mgr_switch(SCREEN0_ID, false);
    }
}

static void slider_event_cb(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target(e);
    int slider_val = (int)lv_slider_get_value(slider);

    if(e->code == LV_EVENT_VALUE_CHANGED) {
        if(slider == ui_Slider1)  {
            lv_label_set_text_fmt(ui_backlight, "%d", slider_val);

            // slider_val = lv_map(slider_val, 0, 10, 0, 255);
            ui_if_epd_set_light(slider_val * 25);
        } else if(slider == ui_Slider2) {
            lv_label_set_text_fmt(ui_refr_cycle, "%d", slider_val);
            ui_port_set_refr_cycle(slider_val);
        } else if(slider == ui_Slider3) {
            lv_label_set_text_fmt(ui_refr_time, "%d", slider_val);
            ui_port_set_refr_times(slider_val);
        }
    } else if(e->code == LV_EVENT_RELEASED) {
        ui_if_epd_refr(EPD_REFRESH_TIME);
    }
    
}

///////////////////// FUNCTIONS ////////////////////
void ui_event_Slider1(lv_event_t * e)
{
    slider_event_cb(e);
}
void ui_event_Slider2(lv_event_t * e)
{
    slider_event_cb(e);
}
void ui_event_Slider3(lv_event_t * e)
{
    slider_event_cb(e);
}

static void create4(lv_obj_t *parent) {
    ui_Slider1 = lv_slider_create(parent);
    lv_slider_set_range(ui_Slider1, 0, 10);
    lv_slider_set_value(ui_Slider1, ui_port_get_light() / 25, LV_ANIM_OFF);
    if(lv_slider_get_mode(ui_Slider1) == LV_SLIDER_MODE_RANGE) lv_slider_set_left_value(ui_Slider1, 0, LV_ANIM_OFF);
    lv_obj_set_width(ui_Slider1, 500);
    lv_obj_set_height(ui_Slider1, 50);
    lv_obj_set_x(ui_Slider1, 333);
    lv_obj_set_y(ui_Slider1, 165);
    lv_obj_set_style_radius(ui_Slider1, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);



    ui_Slider2 = lv_slider_create(parent);
    lv_slider_set_range(ui_Slider2, 1, 10);
    lv_slider_set_value(ui_Slider2, ui_port_get_refr_cycle(), LV_ANIM_OFF);
    if(lv_slider_get_mode(ui_Slider2) == LV_SLIDER_MODE_RANGE) lv_slider_set_left_value(ui_Slider2, 0, LV_ANIM_OFF);
    lv_obj_set_width(ui_Slider2, 500);
    lv_obj_set_height(ui_Slider2, 50);
    lv_obj_set_x(ui_Slider2, 333);
    lv_obj_set_y(ui_Slider2, 288);
    lv_obj_set_style_radius(ui_Slider2, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);


    ui_Slider3 = lv_slider_create(parent);
    lv_slider_set_range(ui_Slider3, 1, 5);
    lv_slider_set_value(ui_Slider3, ui_port_get_refr_times(), LV_ANIM_OFF);
    if(lv_slider_get_mode(ui_Slider3) == LV_SLIDER_MODE_RANGE) lv_slider_set_left_value(ui_Slider3, 0, LV_ANIM_OFF);
    lv_obj_set_width(ui_Slider3, 500);
    lv_obj_set_height(ui_Slider3, 50);
    lv_obj_set_x(ui_Slider3, 335);
    lv_obj_set_y(ui_Slider3, 414);
    lv_obj_set_style_radius(ui_Slider3, 32767, LV_PART_MAIN | LV_STATE_DEFAULT);


    ui_label1 = lv_label_create(parent);
    lv_obj_set_width(ui_label1, LV_SIZE_CONTENT);   /// 9
    lv_obj_set_height(ui_label1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_label1, 58);
    lv_obj_set_y(ui_label1, 172);
    lv_label_set_text(ui_label1, "Backlight");
    lv_obj_set_style_text_font(ui_label1, &Font_Mono_Bold_30, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label2 = lv_label_create(parent);
    lv_obj_set_width(ui_Label2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label2, -332);
    lv_obj_set_y(ui_Label2, 39);
    lv_obj_set_align(ui_Label2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label2, "Refresh Cycle");
    lv_obj_set_style_text_font(ui_Label2, &Font_Mono_Bold_30, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label3 = lv_label_create(parent);
    lv_obj_set_width(ui_Label3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label3, -321);
    lv_obj_set_y(ui_Label3, 161);
    lv_obj_set_align(ui_Label3, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label3, "Refresh Times");
    lv_obj_set_style_text_font(ui_Label3, &Font_Mono_Bold_30, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_backlight = lv_label_create(parent);
    lv_obj_set_width(ui_backlight, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_backlight, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_backlight, 870);
    lv_obj_set_y(ui_backlight, 170);
    lv_label_set_text_fmt(ui_backlight, "%d", ui_port_get_light() / 25);
    lv_obj_set_style_text_font(ui_backlight, &Font_Mono_Bold_30, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_refr_cycle = lv_label_create(parent);
    lv_obj_set_width(ui_refr_cycle, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_refr_cycle, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_refr_cycle, 870);
    lv_obj_set_y(ui_refr_cycle, 296);
    lv_label_set_text_fmt(ui_refr_cycle, "%d", ui_port_get_refr_cycle());
    lv_obj_set_style_text_font(ui_refr_cycle, &Font_Mono_Bold_30, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_refr_time = lv_label_create(parent);
    lv_obj_set_width(ui_refr_time, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_refr_time, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_refr_time, 870);
    lv_obj_set_y(ui_refr_time, 425);
    lv_label_set_text_fmt(ui_refr_time, "%d", ui_port_get_refr_times());
    lv_obj_set_style_text_font(ui_refr_time, &Font_Mono_Bold_30, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_Slider1, ui_event_Slider1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Slider2, ui_event_Slider2, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Slider3, ui_event_Slider3, LV_EVENT_ALL, NULL);

    // back
    scr_back_btn_create(parent, "Setting", scr4_btn_event_cb);
}
static void entry4(void) { }
static void exit4(void) { }
static void destroy4(void) { }

static scr_lifecycle_t screen4 = {
    .create = create4,
    .entry = entry4,
    .exit  = exit4,
    .destroy = destroy4,
};
/*** UI interfavce ***/
void __attribute__((weak)) ui_if_epd_set_light(uint8_t light) {
}
// end

#endif
//************************************[ screen 5 ]****************************************** test
#if 1
lv_obj_t *scr5_cont_PASS;
lv_obj_t *scr5_cont_FAIL;

static void scr5_btn_event_cb(lv_event_t * e)
{
    if(e->code == LV_EVENT_CLICKED){
        ui_if_epd_refr(EPD_REFRESH_TIME);
        scr_mgr_switch(SCREEN0_ID, false);
    }
}

lv_obj_t * scr5_imgbtn_create(lv_obj_t *parent, const void *src, const char *text)
{
    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_set_size(obj, 120, 150);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(obj, 0, LV_PART_MAIN);

    lv_obj_t *img = lv_img_create(obj);
    lv_img_set_src(img, src);
    lv_obj_align(img, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t *lab = lv_label_create(obj);
    lv_obj_set_style_text_font(lab, &Font_Mono_Bold_25, LV_PART_MAIN);
    lv_label_set_text(lab, text);
    lv_obj_align_to(lab, img, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    return obj;
}


static void create5(lv_obj_t *parent) {

    scr5_cont_PASS = lv_obj_create(parent);
    lv_obj_set_size(scr5_cont_PASS, lv_pct(49), lv_pct(80));
    lv_obj_set_style_bg_color(scr5_cont_PASS, lv_color_hex(EPD_COLOR_BG), LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(scr5_cont_PASS, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_width(scr5_cont_PASS, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr5_cont_PASS, 26, LV_PART_MAIN);
    lv_obj_set_flex_flow(scr5_cont_PASS, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_style_pad_row(scr5_cont_PASS, 27, LV_PART_MAIN);
    lv_obj_set_style_pad_column(scr5_cont_PASS, 27, LV_PART_MAIN);
    lv_obj_set_align(scr5_cont_PASS, LV_ALIGN_BOTTOM_LEFT);

    scr5_cont_FAIL = lv_obj_create(parent);
    lv_obj_set_size(scr5_cont_FAIL, lv_pct(49), lv_pct(80));
    lv_obj_set_style_bg_color(scr5_cont_FAIL, lv_color_hex(EPD_COLOR_BG), LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(scr5_cont_FAIL, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_width(scr5_cont_FAIL, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr5_cont_FAIL, 26, LV_PART_MAIN);
    lv_obj_set_flex_flow(scr5_cont_FAIL, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_style_pad_row(scr5_cont_FAIL, 27, LV_PART_MAIN);
    lv_obj_set_style_pad_column(scr5_cont_FAIL, 27, LV_PART_MAIN);
    lv_obj_set_align(scr5_cont_FAIL, LV_ALIGN_BOTTOM_RIGHT);


    lv_obj_t *pass = lv_label_create(parent);
    lv_obj_t *fail = lv_label_create(parent);

    lv_obj_set_style_text_font(pass, &Font_Mono_Bold_25, LV_PART_MAIN);
    lv_obj_set_style_text_font(fail, &Font_Mono_Bold_25, LV_PART_MAIN);

    lv_label_set_text(pass, "INIT PASS");
    lv_label_set_text(fail, "INIT FAIL");

    lv_obj_align_to(pass, scr5_cont_PASS, LV_ALIGN_OUT_TOP_MID, 0, 0);
    lv_obj_align_to(fail, scr5_cont_FAIL, LV_ALIGN_OUT_TOP_MID, 0, 0);

    
    //---------------------
    ui_if_epd_get_SD() ? scr5_imgbtn_create(scr5_cont_PASS, &img_sd_card, "sd card") : scr5_imgbtn_create(scr5_cont_FAIL, &img_sd_card, "sd card");
    ui_if_epd_get_RTC() ? scr5_imgbtn_create(scr5_cont_PASS, &img_rtc, "RTC") : scr5_imgbtn_create(scr5_cont_FAIL, &img_rtc, "RTC");
    ui_if_epd_get_TOUCH() ? scr5_imgbtn_create(scr5_cont_PASS, &img_touch, "touch") : scr5_imgbtn_create(scr5_cont_FAIL, &img_touch, "touch");
    ui_if_epd_get_LORA() ? scr5_imgbtn_create(scr5_cont_PASS, &img_lora, "lora") : scr5_imgbtn_create(scr5_cont_FAIL, &img_lora, "lora");
    ui_if_epd_get_WIFI() ? scr5_imgbtn_create(scr5_cont_PASS, &img_wifi, "connect") : scr5_imgbtn_create(scr5_cont_FAIL, &img_on_wifi, "No WIFI");
    //---------------------
    scr_middle_line(parent);

    // back
    scr_back_btn_create(parent, "Test", scr5_btn_event_cb);
}
static void entry5(void) { }
static void exit5(void) { }
static void destroy5(void) { }

static scr_lifecycle_t screen5 = {
    .create = create5,
    .entry = entry5,
    .exit  = exit5,
    .destroy = destroy5,
};

/*** UI interfavce ***/
bool __attribute__((weak)) ui_if_epd_get_SD(void) {
    return false;
}
bool __attribute__((weak)) ui_if_epd_get_RTC(void) {
    return true;
}
bool __attribute__((weak)) ui_if_epd_get_TOUCH(void) {
    return false;
}
bool __attribute__((weak)) ui_if_epd_get_LORA(void) {
    return false;
}
bool __attribute__((weak)) ui_if_epd_get_WIFI(void){
    return false;
}
// end

#endif
//************************************[ screen 6 ]****************************************** wifi
#if 1
lv_obj_t *scr6_root;
lv_obj_t *wifi_st_lab = NULL;
lv_obj_t *ip_lab;
lv_obj_t *ssid_lab;
lv_obj_t *pwd_lab;

static volatile bool smartConfigStart      = false;
static lv_timer_t   *wifi_timer            = NULL;
static uint32_t      wifi_timer_counter    = 0;
static uint32_t      wifi_connnect_timeout = 60;

static void wifi_info_label_create(lv_obj_t *parent)
{
    ip_lab = lv_label_create(parent);
    // lv_obj_set_style_text_color(ip_lab, lv_color_hex(COLOR_TEXT), LV_PART_MAIN);
    lv_obj_set_style_text_font(ip_lab, &Font_Mono_Bold_25, LV_PART_MAIN);
    lv_label_set_text_fmt(ip_lab, "ip: %s", ui_if_epd_get_WIFI_ip());
    lv_obj_align_to(ip_lab, wifi_st_lab, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);

    ssid_lab = lv_label_create(parent);
    // lv_obj_set_style_text_color(ssid_lab, lv_color_hex(COLOR_TEXT), LV_PART_MAIN);
    lv_obj_set_style_text_font(ssid_lab, &Font_Mono_Bold_25, LV_PART_MAIN);
    lv_label_set_text_fmt(ssid_lab, "ssid: %s", ui_if_epd_get_WIFI_ssid());
    lv_obj_align_to(ssid_lab, ip_lab, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);

    pwd_lab = lv_label_create(parent);
    // lv_obj_set_style_text_color(pwd_lab, lv_color_hex(COLOR_TEXT), LV_PART_MAIN);
    lv_obj_set_style_text_font(pwd_lab, &Font_Mono_Bold_25, LV_PART_MAIN);
    lv_label_set_text_fmt(pwd_lab, "pswd: %s", ui_if_epd_get_WIFI_pwd());
    lv_obj_align_to(pwd_lab, ssid_lab, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
}

static void wifi_config_event_handler(lv_event_t *e)
{
    static int step = 0;
    lv_event_code_t code  = lv_event_get_code(e);

    if(code != LV_EVENT_CLICKED) {
        return;
    }

    if(wifi_is_connect){
        Serial.println(" WiFi is connected do not need to configure WiFi.");
        return;
    }

    if (smartConfigStart) {
        Serial.println("[wifi config] Config Stop");
        if (wifi_timer) {
            lv_timer_del(wifi_timer);
            wifi_timer = NULL;
        }
        WiFi.stopSmartConfig();
        Serial.println("return smart Config has Start;");
        smartConfigStart = false;
        return;
    }
    WiFi.disconnect();
    smartConfigStart = true;
    WiFi.beginSmartConfig();
    Serial.println("[wifi config] Config Start");
    lv_label_set_text(wifi_st_lab, "Wifi Config ...");
    ui_if_epd_refr(EPD_REFRESH_TIME);
    
    wifi_timer = lv_timer_create([](lv_timer_t *t) {
        bool      destory = false;
        wifi_timer_counter++;
        if (wifi_timer_counter > wifi_connnect_timeout && !WiFi.isConnected()) {
            Serial.println("Connect timeout!");
            destory = true;
            Serial.println("[wifi config] Time Out");
        }
        if (WiFi.isConnected()) {
            Serial.println("WiFi has connected!");
            Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
            Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());

            if(strcmp(wifi_ssid, WiFi.SSID().c_str()) == 0) {
                Serial.printf("SSID == CURR SSID\r\n");
            }
            if(strcmp(wifi_password, WiFi.psk().c_str()) == 0) {
                Serial.printf("PSW == CURR PSW\r\n");
            }
            
            String ssid = WiFi.SSID();
            String pwsd = WiFi.psk();
            if(strcmp(wifi_ssid, ssid.c_str()) != 0 ||
               strcmp(wifi_password, pwsd.c_str()) != 0) {
                memcpy(wifi_ssid, ssid.c_str(), WIFI_SSID_MAX_LEN);
                memcpy(wifi_password, pwsd.c_str(), WIFI_PSWD_MAX_LEN);
                eeprom_wr_wifi(ssid.c_str(), ssid.length(), pwsd.c_str(), pwsd.length());
            }

            destory   = true;
            String IP = WiFi.localIP().toString();
            wifi_is_connect = true;
            Serial.println("[wifi config] WiFi has connected!");

            lv_label_set_text(wifi_st_lab, (wifi_is_connect == true ? "Wifi Connect" : "Wifi Disconnect"));
            wifi_info_label_create(scr6_root);
            ui_if_epd_refr(EPD_REFRESH_TIME);
        }
        if (destory) {
            WiFi.stopSmartConfig();
            smartConfigStart = false;
            lv_timer_del(wifi_timer);
            wifi_timer         = NULL;
            wifi_timer_counter = 0;
        }
        // Every seconds check conected
    },
    1000, NULL);
}

static void scr6_btn_event_cb(lv_event_t * e)
{
    if(e->code == LV_EVENT_CLICKED){
        ui_if_epd_refr(EPD_REFRESH_TIME);
        scr_mgr_switch(SCREEN0_ID, false);
    }
}

static void create6(lv_obj_t *parent) {
    scr6_root = parent;
    wifi_st_lab = lv_label_create(parent);
    lv_obj_set_width(wifi_st_lab, 360);
    // lv_obj_set_style_text_color(wifi_st_lab, lv_color_hex(COLOR_TEXT), LV_PART_MAIN);
    lv_obj_set_style_text_font(wifi_st_lab, &Font_Mono_Bold_25, LV_PART_MAIN);
    lv_label_set_text(wifi_st_lab, (ui_if_epd_get_WIFI() ? "Wifi Connect" : "Wifi Disconnect"));
    lv_obj_set_style_text_align(wifi_st_lab, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);
    lv_obj_align(wifi_st_lab, LV_ALIGN_BOTTOM_RIGHT, -0, -190);

    if(ui_if_epd_get_WIFI()) {
        wifi_info_label_create(parent);
    }

    lv_obj_t *label, *tips_label;
    tips_label = lv_label_create(parent);
    lv_obj_set_width(tips_label, LV_PCT(100));
    lv_label_set_long_mode(tips_label, LV_LABEL_LONG_SCROLL);
    lv_obj_set_style_text_color(tips_label, lv_color_black(), LV_PART_MAIN);
    lv_label_set_text(tips_label,   "1. Scan the QR code to download `EspTouch`\n"
                                    "2. Install and launch `EspTouch` APP\n"
                                    "3. Make sure your phone is connected to WIFI\n"
                                    "4. Tap the [EspTouch] option of the APP\n"
                                    "5. Enter your WIFI password and click [confirm]\n"
                                    "6. Finally, click [config wifi] on the ink screen\n"
                                    "After that, wait for the network distribution to succeed!"
                                    );
    
    lv_obj_set_style_text_font(tips_label, &Font_Mono_Bold_25, LV_PART_MAIN);
    lv_obj_align(tips_label, LV_ALIGN_LEFT_MID, 50, -100);

    const char *android_url = "https://github.com/EspressifApp/EsptouchForAndroid/releases/tag/v2.0.0/esptouch-v2.0.0.apk";
    const char *ios_url     = "https://apps.apple.com/cn/app/espressif-esptouch/id1071176700";

    lv_coord_t size            = 120;
    lv_obj_t  *android_rq_code = lv_qrcode_create(parent, size, lv_color_black(), lv_color_white());
    lv_qrcode_update(android_rq_code, android_url, strlen(android_url));
    lv_obj_set_pos(android_rq_code, 340, 10);
    lv_obj_align(android_rq_code, LV_ALIGN_LEFT_MID, 50, 100);

    lv_obj_set_style_border_color(android_rq_code, lv_color_white(), 0);
    lv_obj_set_style_border_width(android_rq_code, 5, 0);
    label = lv_label_create(parent);
    lv_label_set_text(label, "Android");
    lv_obj_set_style_text_color(label, lv_color_black(), LV_PART_MAIN);
    lv_obj_align_to(label, android_rq_code, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t *ios_rq_code = lv_qrcode_create(parent, size, lv_color_black(), lv_color_white());
    lv_qrcode_update(ios_rq_code, ios_url, strlen(ios_url));
    lv_obj_align_to(ios_rq_code, android_rq_code, LV_ALIGN_OUT_RIGHT_MID, 20, 0);

    lv_obj_set_style_border_color(ios_rq_code, lv_color_white(), 0);
    lv_obj_set_style_border_width(ios_rq_code, 5, 0);
    label = lv_label_create(parent);
    lv_label_set_text(label, "IOS");
    lv_obj_set_style_text_color(label, lv_color_black(), LV_PART_MAIN);
    lv_obj_align_to(label, ios_rq_code, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    // config btn
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 200, 60);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, -20, -120);
    lv_obj_set_style_radius(btn, 10, LV_PART_MAIN);
    label = lv_label_create(btn);
    lv_label_set_text(label, "Config Wifi");
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_text_font(label, &Font_Mono_Bold_25, LV_PART_MAIN);
    lv_obj_center(label);
    lv_obj_add_event_cb(btn, wifi_config_event_handler, LV_EVENT_CLICKED, NULL);

    //---------------------
    // scr_middle_line(parent);
    // back
    scr_back_btn_create(parent, "Wifi", scr6_btn_event_cb);
}
static void entry6(void) { }
static void exit6(void) { }
static void destroy6(void) { }

static scr_lifecycle_t screen6 = {
    .create = create6,
    .entry = entry6,
    .exit  = exit6,
    .destroy = destroy6,
};

/*** UI interfavce ***/
String  __attribute__((weak)) ui_if_epd_get_WIFI_ip(void) {
    return "WIFI not connected";
}
const char * __attribute__((weak)) ui_if_epd_get_WIFI_ssid(void) {
    return "WIFI not connected";
}
const char * __attribute__((weak)) ui_if_epd_get_WIFI_pwd(void) {
    return "WIFI not connected";
}
// end
#endif
//************************************[ screen 7 ]****************************************** battery
#if 1
static lv_obj_t *scr7_cont_letf;
static lv_obj_t *scr7_cont_right;
static lv_obj_t *batt_right[10] = {0};
static lv_obj_t *batt_left[10] = {0};
static lv_timer_t *batt_refr_timer = NULL;
#define line_max 28

static void battery_set_line(lv_obj_t *label, const char *str1, const char *str2)
{
    int w2 = strlen(str2);
    int w1 = line_max - w2;
    lv_label_set_text_fmt(label, "%-*s%-*s", w1, str1, w2, str2);
}

static void scr7_btn_event_cb(lv_event_t * e)
{
    if(e->code == LV_EVENT_CLICKED){
        ui_if_epd_refr(EPD_REFRESH_TIME);
        scr_mgr_switch(SCREEN0_ID, false);
    }
}

static void battery_data_refr(void)
{
    char buf[line_max];
    // BQ25896
    if(battery_25896_is_vaild()) {
        battery_25896_refr();

        battery_set_line(batt_left[0], "Charge:", (battery_25896_is_chr() == true ? "Charging" : "Not charged"));

        lv_snprintf(buf, line_max, "%.2fV", battery_25896_get_VBUS());
        battery_set_line(batt_left[1], "VBUS:", buf);

        lv_snprintf(buf, line_max, "%.2fV", battery_25896_get_VSYS());
        battery_set_line(batt_left[2], "VSYS:", buf);

        lv_snprintf(buf, line_max, "%.2fV", battery_25896_get_VBAT());
        battery_set_line(batt_left[3], "VBAT:", buf);

        lv_snprintf(buf, line_max, "%.2fv", battery_25896_get_targ_VOLT());
        battery_set_line(batt_left[4], "VOLT Target:", buf);

        lv_snprintf(buf, line_max, "%.2fmA", battery_25896_get_CHG_CURR());
        battery_set_line(batt_left[5], "Charge Curr:", buf);

        lv_snprintf(buf, line_max, "%.2fmA", battery_25896_get_PREC_CURR());
        battery_set_line(batt_left[6], "Precharge Curr:", buf);

        lv_snprintf(buf, line_max, "%s", battery_25896_get_CHG_ST());
        battery_set_line(batt_left[7], "CHG Status:", buf);

        lv_snprintf(buf, line_max, "%s", battery_25896_get_VBUS_ST());
        battery_set_line(batt_left[8], "VBUS Status:", buf);

        lv_snprintf(buf, line_max, "%s", battery_25896_get_NTC_ST());
        battery_set_line(batt_left[9], "NCT:", buf);

    }

    // BQ27220
    if(battery_27220_is_vaild()) {
        battery_set_line(batt_right[0], "Charge:", (battery_27220_is_chr() == true? "Charging" : "Not charged"));

        lv_snprintf(buf, line_max, "%.2fV", battery_27220_get_VOLT()/1000);
        battery_set_line(batt_right[1], "VOLT:", buf);

        lv_snprintf(buf, line_max, "%.2fV", battery_27220_get_VOLT_CHG()/1000);
        battery_set_line(batt_right[2], "VOLT Charge:", buf);

        lv_snprintf(buf, line_max, "%.2fmA", battery_27220_get_CURR_ARG());
        battery_set_line(batt_right[3], "CURR Average:", buf);

        lv_snprintf(buf, line_max, "%.2fmA", battery_27220_get_CURR_INS());
        battery_set_line(batt_right[4], "CURR Instant:", buf);

        lv_snprintf(buf, line_max, "%.2fmA", battery_27220_get_CURR_STD());
        battery_set_line(batt_right[5], "Curr Standby:", buf);

        lv_snprintf(buf, line_max, "%.2fmA", battery_27220_get_CURR_CHG());
        battery_set_line(batt_right[6], "Curr Charging:", buf);

        lv_snprintf(buf, line_max, "%.2f", battery_27220_get_TEMP());
        battery_set_line(batt_right[7], "TEMP:", buf);

        lv_snprintf(buf, line_max, "%.2fmAh", battery_27220_get_BATT_CAP());
        battery_set_line(batt_right[8], "CAP BATT:", buf);

        lv_snprintf(buf, line_max, "%.2fmAh", battery_27220_get_BATT_CAP_FULL());
        battery_set_line(batt_right[9], "CAP BATT Full:", buf);
    }
}

static void batt_refr_timer_event(lv_timer_t *t)
{
    battery_data_refr();
    // ui_if_epd_refr(EPD_REFRESH_TIME);
    ui_epd_refr(EPD_REFRESH_TIME, 2, 2);
}

static lv_obj_t * scr7_create_label(lv_obj_t *parent)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_obj_set_width(label, LCD_HOR_SIZE/2-50);
    lv_obj_set_style_text_font(label, &Font_Mono_Bold_25, LV_PART_MAIN);   
    lv_obj_set_style_border_width(label, 1, LV_PART_MAIN);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_border_side(label, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN);
    return label;
}

static void create7(lv_obj_t *parent)
{
    lv_obj_t *label;

    // left cont
    scr7_cont_letf = lv_obj_create(parent);
    lv_obj_set_size(scr7_cont_letf, lv_pct(49), lv_pct(85));
    lv_obj_set_style_bg_color(scr7_cont_letf, lv_color_hex(EPD_COLOR_BG), LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(scr7_cont_letf, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(scr7_cont_letf, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(scr7_cont_letf, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr7_cont_letf, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_hor(scr7_cont_letf, 20, LV_PART_MAIN);
    lv_obj_set_flex_flow(scr7_cont_letf, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(scr7_cont_letf, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_column(scr7_cont_letf, 5, LV_PART_MAIN);
    lv_obj_set_align(scr7_cont_letf, LV_ALIGN_BOTTOM_LEFT);

    // left
    if(!battery_25896_is_vaild()) {
        label = scr7_create_label(scr7_cont_letf);
        lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
        lv_label_set_text_fmt(label, "%s", "[0x6B] BQ25896 NOT FOUND");
        goto NO_BATTERY_BQ25896;
    }

    label = scr7_create_label(scr7_cont_letf);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_label_set_text_fmt(label, "%s", "[0x6B] BQ25896");

    for(int i = 0; i < sizeof(batt_left) / sizeof(batt_left[0]); i++) {
        batt_left[i] = scr7_create_label(scr7_cont_letf);
    }

    battery_set_line(batt_left[0], "Charge:", "---");
    battery_set_line(batt_left[1], "VBUS:", "---");
    battery_set_line(batt_left[2], "VBUS Status:", "---");
    battery_set_line(batt_left[3], "VSYS:", "---");
    battery_set_line(batt_left[4], "VSYS Status:", "---");
    battery_set_line(batt_left[5], "VBAT:", "---");
    battery_set_line(batt_left[6], "ICHG:", "---");
    battery_set_line(batt_left[7], "TEMP:", "---");
    battery_set_line(batt_left[8], "TSPCT:", "---");
    battery_set_line(batt_left[9], "Charger Err:", "---");

    // right cont
NO_BATTERY_BQ25896:

    scr7_cont_right = lv_obj_create(parent);
    lv_obj_set_size(scr7_cont_right, lv_pct(49), lv_pct(85));
    lv_obj_set_style_bg_color(scr7_cont_right, lv_color_hex(EPD_COLOR_BG), LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(scr7_cont_right, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(scr7_cont_right, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(scr7_cont_right, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(scr7_cont_right, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_hor(scr7_cont_right, 10, LV_PART_MAIN);
    lv_obj_set_flex_flow(scr7_cont_right, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(scr7_cont_right, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_column(scr7_cont_right, 5, LV_PART_MAIN);
    lv_obj_set_align(scr7_cont_right, LV_ALIGN_BOTTOM_RIGHT);

    // right
    if(!battery_27220_is_vaild()) {
        label = scr7_create_label(scr7_cont_right);
        lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
        lv_label_set_text_fmt(label, "%s", "[0x55] BQ27220 NOT FOUND");
        goto NO_BATTERY;
    }
    label = scr7_create_label(scr7_cont_right);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_label_set_text_fmt(label, "%s", "[0x55] BQ27220");

    for(int i = 0; i < sizeof(batt_right) / sizeof(batt_right[0]); i++) {
        batt_right[i] = scr7_create_label(scr7_cont_right);
    }

    battery_set_line(batt_right[0], "Charge:", "---");
    battery_set_line(batt_right[1], "VOLT:", "---");
    battery_set_line(batt_right[2], "VOLT Charge:", "---");
    battery_set_line(batt_right[3], "CURR Average:", "---");
    battery_set_line(batt_right[4], "CURR Instant:", "---");
    battery_set_line(batt_right[5], "Curr Standby:", "---");
    battery_set_line(batt_right[6], "Curr Charging:", "---");
    battery_set_line(batt_right[7], "TEMP:", "---");
    battery_set_line(batt_right[8], "CAP BATT:", "---");
    battery_set_line(batt_right[9], "CAP BATT Full:", "---");

NO_BATTERY:
    //---------------------
    scr_middle_line(parent);
    // back
    scr_back_btn_create(parent, "battery", scr7_btn_event_cb);
    // timer
    batt_refr_timer = lv_timer_create(batt_refr_timer_event, 5000, NULL);
    lv_timer_pause(batt_refr_timer);
}

static void entry7(void) {
    battery_data_refr();
    ui_if_epd_refr(EPD_REFRESH_TIME);
    lv_timer_resume(batt_refr_timer);
}
static void exit7(void) {
    lv_timer_pause(batt_refr_timer);
}
static void destroy7(void) { 
    lv_timer_del(batt_refr_timer);
    if(batt_refr_timer){
        batt_refr_timer = NULL;
    }
}

static scr_lifecycle_t screen7 = {
    .create = create7,
    .entry = entry7,
    .exit  = exit7,
    .destroy = destroy7,
};
#undef line_max
#endif
//************************************[ screen 8 ]****************************************** shutdown
#if 1

static void scr8_btn_event_cb(lv_event_t * e)
{
    if(e->code == LV_EVENT_CLICKED){
        ui_if_epd_refr(EPD_REFRESH_TIME);
        scr_mgr_switch(SCREEN0_ID, false);
    }
}

static void scr8_shutdown_timer_event(lv_timer_t *t)
{
    lv_timer_del(t);
    ui_batt_power_off();
}

static void create8(lv_obj_t *parent)
{

    lv_obj_t * img = lv_img_create(parent);
    lv_img_set_src(img, &img_start);
    lv_obj_center(img);


    const char *str1 = "PWR: Press and hold to power on";

    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, str1);
    lv_obj_set_style_transform_angle(label, -900, 0);
    lv_obj_align(label, LV_ALIGN_RIGHT_MID, 60, 80);

    lv_coord_t w = lv_txt_get_width(str1, strlen(str1), &Font_Mono_Bold_20, 0, false);
    lv_obj_set_style_transform_pivot_x(label, w / 2, 0);

    // back
    scr_back_btn_create(parent, "Shoutdown", scr8_btn_event_cb);

    lv_timer_create(scr8_shutdown_timer_event, EPD_REFRESH_TIME+500, NULL);
}

static void entry8(void) {
    
}
static void exit8(void) {
}
static void destroy8(void) { 

}

static scr_lifecycle_t screen8 = {
    .create = create8,
    .entry = entry8,
    .exit  = exit8,
    .destroy = destroy8,
};
#endif
//************************************[ UI ENTRY ]******************************************

void home_back_timer_event(lv_timer_t *t)
{
    if(ui_get_home_btn_st()) {
        int id = scr_mgr_get_curr_id();
        ui_set_home_btn_st(false);
        
        // printf("id = %d home_back_chk_event\n", id);

        if(id != SCREEN0_ID) {
            ui_if_epd_refr(EPD_REFRESH_TIME);
            scr_mgr_switch(SCREEN0_ID, false);
        }
    }
}

// void home_back_chk_event(void *used_data)
// {
//     scr_mgr_switch(SCREEN0_ID, false);
//     printf("home_back_chk_event\n");
// }

void ui_epd47_entry(void)
{
    lv_disp_t *disp = lv_disp_get_default();
    disp->theme = lv_theme_mono_init(disp, false, LV_FONT_DEFAULT);

    scr_mgr_init();
    scr_mgr_set_bg_color(EPD_COLOR_BG);
    scr_mgr_register(SCREEN0_ID, &screen0); // menu
    scr_mgr_register(SCREEN1_ID, &screen1); // clock
    scr_mgr_register(SCREEN2_ID, &screen2); // lora
    scr_mgr_register(SCREEN3_ID, &screen3); // sd card
    scr_mgr_register(SCREEN4_ID, &screen4); // setting
    scr_mgr_register(SCREEN5_ID, &screen5); // test
    scr_mgr_register(SCREEN6_ID, &screen6); // wifi
    scr_mgr_register(SCREEN7_ID, &screen7); // battery
    scr_mgr_register(SCREEN8_ID, &screen8); // battery

    scr_mgr_switch(SCREEN0_ID, false); // set root screen
    scr_mgr_set_anim(LV_SCR_LOAD_ANIM_NONE, LV_SCR_LOAD_ANIM_NONE, LV_SCR_LOAD_ANIM_NONE);

    lv_timer_create(home_back_timer_event, 50, NULL);
    // touch.setHomeButtonCallback(home_back_chk_event, NULL);
}