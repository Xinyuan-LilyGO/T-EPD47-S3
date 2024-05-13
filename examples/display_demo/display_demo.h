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

// define 
#define WIFI_SSID "Your WiFi SSID"
#define WIFI_PASSWORD "Your WiFi PASSWORD"

// init flag
extern bool sd_is_init;
extern bool rtc_is_init;
extern bool lora_is_init;
extern bool touchOnline;
extern bool wifi_is_connect;

// lora
#define LORA_MODE_SEND 0
#define LORA_MODE_RECV 1
extern int lora_mode;
extern void lora_set_mode(int mode);
extern bool lora_receive(String *str);
extern void lora_transmit(const char *str);

// wifi
extern bool wifi_is_connect;
extern struct tm timeinfo;

// refresh ink
extern void disp_manual_refr(uint16_t time);