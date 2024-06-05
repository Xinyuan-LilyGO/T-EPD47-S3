#pragma once
// include 
#include <Arduino.h>
#include "epd_driver.h"
#include "utilities.h"
#include "firasans.h"
#include "esp_adc_cal.h"
#include <Wire.h>
#include <TouchDrvGT911.hpp>
#include <SensorPCF8563.hpp>
#include <WiFi.h>
#include <esp_sntp.h>
#include "utilities.h"
#include <RadioLib.h>
#include <FS.h>
#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>
#include "BQ25896.h"
#include "bq27220.h"
#include <XPowersLib.h>

// define 
#define WIFI_SSID "xinyuandianzi"
#define WIFI_PASSWORD "AA15994823428"

// init flag
extern bool sd_is_init;
extern bool rtc_is_init;
extern bool lora_is_init;
extern bool touchOnline;
extern bool bq25896_is_init;
extern bool bq27220_is_init;
extern bool wifi_is_connect;
extern bool home_btn_st;

// sensor
extern BQ25896 battery_25896;
extern BQ27220 bq27220;
extern SensorPCF8563 rtc;
extern TouchDrvGT911 touch;
extern XPowersPPM PPM;

// lora
#define LORA_MODE_SEND 0
#define LORA_MODE_RECV 1
extern SX1262 radio;
extern int lora_mode;
extern void lora_set_mode(int mode);
extern bool lora_receive(String *str);
extern void lora_transmit(const char *str);

// wifi
#define WIFI_SSID_MAX_LEN 30
#define WIFI_PSWD_MAX_LEN 30
extern bool wifi_is_connect;
extern struct tm timeinfo;
extern char wifi_ssid[WIFI_SSID_MAX_LEN];
extern char wifi_password[WIFI_PSWD_MAX_LEN];

// eeprom
#define EEPROM_UPDATA_FLAG_NUM    0xAA
#define WIFI_SSID_EEPROM_ADDR     (1)
#define WIFI_PSWD_EEPROM_ADDR     (WIFI_SSID_EEPROM_ADDR + WIFI_SSID_MAX_LEN)
// #define UI_THEME_EEPROM_ADDR      (WIFI_PSWD_EEPROM_ADDR + WIFI_PSWD_MAX_LEN)
#define UI_BACKLIGHT_EEPROM_ADDR  (WIFI_PSWD_EEPROM_ADDR + WIFI_PSWD_MAX_LEN)
#define UI_REFR_CYCLE_EEPROM_ADDR (UI_BACKLIGHT_EEPROM_ADDR + 1)
#define UI_REFR_TIMES_EEPROM_ADDR (UI_REFR_CYCLE_EEPROM_ADDR + 1)
#define EEPROM_SIZE_MAX 64

void eeprom_wr(int addr, uint8_t val);
void eeprom_wr_wifi(const char *ssid, uint16_t ssid_len, const char *pwsd, uint16_t pwsd_len);


// refresh ink
extern void disp_manual_refr(uint16_t time);
extern void disp_manual_refr_cycle(uint16_t time, uint16_t cycle, uint16_t times);
extern uint16_t refr_backlight;
extern uint16_t refr_cycle;
extern uint16_t refr_times;