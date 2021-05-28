#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#ifdef OLED_ENABLE

#ifdef OLED_LIB_ADAFRUIT
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define Ssd1306 Adafruit_SSD1306
#endif

#ifdef OLED_LIB_TINY
#include <Tiny4kOLED.h>
#define Ssd1306 SSD1306Device
#endif

#endif

#endif