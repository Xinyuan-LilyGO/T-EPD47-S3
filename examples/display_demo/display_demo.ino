#include <Arduino.h>
#include "epd_driver.h"

#include "firasans.h"
#include "esp_adc_cal.h"
#include <FS.h>
#include <SPI.h>
#include <SD.h>

#include <Wire.h>
#include <TouchDrvGT911.hpp>
#include <SensorPCF8563.hpp>
#include <WiFi.h>
#include <esp_sntp.h>
#include "utilities.h"
#include <RadioLib.h>
#include "ui.h"

#define USED_EPD_LVGL 1



#define WIFI_SSID "Your WiFi SSID"
#define WIFI_PASSWORD "Your WiFi PASSWORD"

#define DISPLAY_WIDTH 960
#define DISPLAY_HEIGHT 576
#define DISPLAY_MAX_PAGE 2

/* clang-format off */
#define AREA_EVENT(a, tx, ty)                                                                         \
    if (touch_press) {                                                                                \
        for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++) {                                          \
            if ((tx > a[i].x && tx < (a[i].x + a[i].w)) && (ty > a[i].y && ty < (a[i].y + a[i].h))) { \
                touch_cnt++;                                                                          \
                if (touch_cnt == 2) {                                                                 \
                    switch (a[i].buttonID)                                                            \
                    { 

#define AREA_CASE(x) case x:

#define AREA_END        \
    default:break;}     \
        touch_cnt = 0;} \
        break; } } }
/* clang-format on */

//
bool sd_is_init = false;
bool rtc_is_init = false;
bool lora_is_init = false;
bool touchOnline = false;

int curr_page = 0;
int touch_press = 0;
int touch_cnt = 0;
int16_t touch_x, touch_y;
int cursor_x, cursor_y;
uint8_t *framebuffer;
uint32_t interval = 0;
int vref = 1100;
char buf[128];

struct _point
{
    uint8_t buttonID;
    int32_t x;
    int32_t y;
    int32_t w;
    int32_t h;
};

SensorPCF8563 rtc;
TouchDrvGT911 touch;

SX1262 radio = new Module(LORA_CS, LORA_IRQ, LORA_RST, LORA_BUSY);
int transmissionState = RADIOLIB_ERR_NONE;

#if USED_EPD_LVGL // EPD_LVGL
#include "lvgl.h"
#include "settings.h"

#define DISP_BUF_SIZE (EPD_WIDTH*EPD_HEIGHT)

double gamma_value = 0.9;
uint8_t gamme_curve[256] = { 0 };
uint8_t *decodebuffer = NULL;

volatile bool disp_flush_enabled = false;
lv_timer_t *disp_refr_time = NULL;
int disp_refr_is_busy = 0;

void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    if(disp_flush_enabled) {
        uint16_t w = lv_area_get_width(area);
        uint16_t h = lv_area_get_height(area);
        // uint32_t gray = 0;
        int i=0, j=0;
        uint8_t *c  = (uint8_t *)color_p;

        // epd_poweron();
        epd_clear();
#if LV_COLOR_DEPTH == 8
        for(i = 0; i < (w * h) / 2; i++) {
            uint8_t t1 = 0, t2 = 0, tmp = 0;
            t1 = c[j + 0] / 16;
            t2 = c[j + 1] / 16;
            tmp = ((t1 << 4) & 0xF0) | (t2 & 0x0F);
            c[i] = tmp;
            j += 2;
        }
#elif LV_COLOR_DEPTH == 16
        for(i = 0; i < (w * h) / 2; i++) {
            uint8_t tmp = 0;
            tmp |= (c[j + 0] << 6) & 0xC0;
            tmp |= (c[j + 1] << 4) & 0x30;
            tmp |= (c[j + 2] << 2) & 0x0C;
            tmp |= (c[j + 3] << 0) & 0x03;
            c[i] = tmp;
            j += 4;
        }
#elif LV_COLOR_DEPTH == 32
        for (y = area->y1; y <= area->y2; y++) {
            for (x = area->x1; x <= area->x2; x++) {
                uint8_t gray = (color_p->ch.red * 38 + color_p->ch.green * 75 + color_p->ch.blue * 15) >> 7;

                color_p++;
            }
        }
#endif
        epd_draw_image(epd_full_screen(), (uint8_t *)c, BLACK_ON_WHITE);
        disp_flush_enabled = false;
        Serial.printf("dips x=%d, y=%d, w=%d, h=%d\n", area->x1, area->y1, w, h);
    }
    /* Inform the graphics library that you are ready with the flushing */
    lv_disp_flush_ready(disp);
}

void disp_refrensh_cb(lv_timer_t *t)
{
    lv_obj_invalidate(lv_scr_act());
    disp_flush_enabled = true;
    lv_timer_del(t);
    disp_refr_is_busy = false;
}

void disp_manual_refr(void)
{
    lv_obj_invalidate(lv_scr_act());
    disp_flush_enabled = true;
    disp_refr_time = lv_timer_create(disp_refrensh_cb, 200, NULL);
    disp_refr_is_busy = 1;
}

void disp_monitor_cb(lv_disp_drv_t * drv, uint32_t time, uint32_t px)
{
    // LV_UNUSED(drv);
    // LV_UNUSED(px);

    // static int monitor_cnt = 0;
    // Serial.printf("disp_monitor_cb: %d %d\n", monitor_cnt++, time);

    // if(disp_refr_is_busy == 0) {
    //     disp_refr_time = lv_timer_create(disp_refrensh_cb, time * 2, NULL);
    //     disp_refr_is_busy = 1;
    // }
    
}

void my_input_read(lv_indev_drv_t * drv, lv_indev_data_t*data)
{
    int16_t  x, y;
    if(touchOnline) {
        if(touch.getPoint(&x, &y)){
            data->point.x = x;
            data->point.y = y;
            data->state = LV_INDEV_STATE_PRESSED;
            Serial.printf("input X:%d Y:%d\n", data->point.x, data->point.y);
        }else {
            data->state = LV_INDEV_STATE_RELEASED; 
        }
    }
}

void lv_port_disp_init(void)
{
    lv_init();

    static lv_disp_draw_buf_t draw_buf;

    lv_color_t *lv_disp_buf_p = (lv_color_t *)ps_calloc(sizeof(lv_color_t), DISP_BUF_SIZE);
    lv_color_t *lv_disp_buf_p1 = (lv_color_t *)ps_calloc(sizeof(lv_color_t), DISP_BUF_SIZE);
    lv_disp_draw_buf_init(&draw_buf, lv_disp_buf_p, lv_disp_buf_p1, DISP_BUF_SIZE);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = EPD_WIDTH;
    disp_drv.ver_res = EPD_HEIGHT;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.monitor_cb = disp_monitor_cb;
    disp_drv.full_refresh = 1;
    lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);      /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;                 /*See below.*/
    indev_drv.read_cb = my_input_read;              /*See below.*/
    /*Register the driver in LVGL and save the created input device object*/
    // static lv_indev_t * my_indev = lv_indev_drv_register(&indev_drv);
    lv_indev_drv_register(&indev_drv);

}
#endif


void setup()
{
    Serial.begin(115200);

    pinMode(BL_EN, OUTPUT);
    analogWrite(BL_EN, 0);

    epd_init();

    // SD
    SPI.begin(BOARD_SPI_SCLK, BOARD_SPI_MISO, BOARD_SPI_MOSI, -1);
    sd_is_init = SD.begin(SD_CS);

    // LORA
    // Serial.print(F("[SX1262] Initializing ... "));
    // int state= radio.begin();
    // if (state == RADIOLIB_ERR_NONE) {
    //     Serial.println(F("success!"));
    //     lora_is_init = true;
    // } else {
    //     Serial.print(F("failed, code "));
    //     Serial.println(state);
    //     lora_is_init = false;
    // }

    // RTC
    Wire.begin(BOARD_SDA, BOARD_SCL);
    Wire.beginTransmission(PCF8563_SLAVE_ADDRESS);
    if (Wire.endTransmission() == 0)
    {
        rtc.begin(Wire, PCF8563_SLAVE_ADDRESS, BOARD_SDA, BOARD_SCL);
        rtc_is_init = true;
    }

    // TOUCH
    uint8_t touchAddress = 0x14;
    Wire.beginTransmission(0x5D);
    if (Wire.endTransmission() == 0)
    {
        touchAddress = 0x5D;
    }

    touch.setPins(TOUCH_RST, TOUCH_INT);
    if (touch.begin(Wire, touchAddress, BOARD_SDA, BOARD_SCL))
    {
        touch.setMaxCoordinates(EPD_WIDTH, EPD_HEIGHT);
        touch.setSwapXY(true);
        touch.setMirrorXY(false, true);
        touchOnline = true;
    }

    lv_port_disp_init();
    epd_poweron();
    epd_clear();
    
    settings_ui();
    //  ui_entry();
     disp_manual_refr();
}

void loop()
{
#if USED_EPD_LVGL
    lv_task_handler();
#else
    if (curr_page < DISPLAY_MAX_PAGE)
    {
        switch (curr_page)
        {
            case 0: page1_loop(); break;
            case 1: page2_loop(); break;
            default:
                break;
        }
    }

    if (touchOnline)
    {
        touch_press = touch.getPoint(&touch_x, &touch_y);

        if(touch_press) {
            Serial.printf("touch X:%d Y:%d\n", touch_x, touch_y);
        }

        _point btn[2] = {
            {0, 0, 240, 100, 100},               // LEFT BTN
            {1, EPD_WIDTH - 100, 240, 100, 100}, // RIGHT BTN
        };

        AREA_EVENT(btn, touch_x, touch_y)
        AREA_CASE(0){
            page_switch(curr_page, '-');
            break;
        }
        AREA_CASE(1){
            page_switch(curr_page, '+');
            break;
        }
        AREA_END;
    }
#endif
    delay(1);
}
