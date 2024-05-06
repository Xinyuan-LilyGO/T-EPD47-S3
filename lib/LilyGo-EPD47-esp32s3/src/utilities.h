#pragma once

#if defined(CONFIG_IDF_TARGET_ESP32)



#define BUTTON_1  (34)
#define BUTTON_2  (35)
#define BUTTON_3  (39)

#define BATT_PIN  (36)

#define SD_MISO   (12)
#define SD_MOSI   (13)
#define SD_SCLK   (14)
#define SD_CS     (15)

#define BOARD_SCL (14)
#define BOARD_SDA (15)
#define TOUCH_INT (13)

#define GPIO_MISO (12)
#define GPIO_MOSI (13)
#define GPIO_SCLK (14)
#define GPIO_CS   (15)

#elif defined(CONFIG_IDF_TARGET_ESP32S3)


#define BUTTON_1   (48)

#define BATT_PIN   (4)

#define GPIO_MISO  (45)
#define GPIO_MOSI  (10)
#define GPIO_SCLK  (48)
#define GPIO_CS    (39)

//////////
#define BOARD_SPI_MISO    (8)
#define BOARD_SPI_MOSI    (17)
#define BOARD_SPI_SCLK    (18)

#define BOARD_SCL  (5)
#define BOARD_SDA  (6)

#define SD_MISO    (BOARD_SPI_MISO)
#define SD_MOSI    (BOARD_SPI_MOSI)
#define SD_SCLK    (BOARD_SPI_SCLK)
#define SD_CS      (16)

#define LORA_MISO (BOARD_SPI_MISO)
#define LORA_MOSI (BOARD_SPI_MOSI)
#define LORA_SCLK (BOARD_SPI_SCLK)
#define LORA_CS   (46)
#define LORA_IRQ  (3)
#define LORA_RST  (43)
#define LORA_BUSY (44)

#define TOUCH_SCL  (BOARD_SCL)
#define TOUCH_SDA  (BOARD_SDA)
#define TOUCH_INT  (15)
#define TOUCH_RST  (41)

#define RTC_INT 7
#define RT_SCL  (BOARD_SCL)
#define RT_SDA  (BOARD_SDA)

#define BL_EN (40)

#define BATT_PIN (4)

#define BOOT_BTN (0)
#define KEY_BTN  (48)


#endif
