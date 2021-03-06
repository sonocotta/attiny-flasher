#ifndef _MAIN_H
#define _MAIN_H

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "isp.h"
#include "boards.h"

#if defined(SW_SERIAL_ENABLE)
#include <SoftwareSerial.h>
SoftwareSerial *sserial;
#endif

#include "debug.h"

#ifdef OLED_ENABLE
  // Screen settings
  #define SCREEN_WIDTH     128 
  #define SCREEN_HEIGHT     64 
  #define SCREEN_MAX_ROWS    8
  #define SCREEN_MAX_COLS   21
  #define SCREEN_ROW_HEIGHT 16
  #define OLED_RESET        -1

  #include "display.h"

  #ifdef OLED_LIB_ADAFRUIT
  Ssd1306 * display = new Ssd1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  #endif

  #ifdef OLED_LIB_TINY
  Ssd1306 * display = &oled;
  #endif
#endif

void serial_bridge_loop();
void pulse(int pin, int times);

#ifdef OLED_ENABLE
void logo_loop(Ssd1306 * display);
void logo_show(Ssd1306 * display);
#endif

#endif