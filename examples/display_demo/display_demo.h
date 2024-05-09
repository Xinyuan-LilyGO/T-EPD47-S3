#pragma once
// include 
#include <FS.h>
#include <SPI.h>
#include <SD.h>



// init flag
extern bool sd_is_init;
extern bool rtc_is_init;
extern bool lora_is_init;
extern bool touchOnline;

// wifi
extern bool wifi_is_connect;
extern struct tm timeinfo;

// refresh ink
extern void disp_manual_refr(uint16_t time);