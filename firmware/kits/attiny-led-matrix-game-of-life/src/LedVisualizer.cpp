#include <stdint.h>
#include <Arduino.h>
#include <util/delay.h>
#include "LedVisualizer.h"
#include "LedControl.h"

static uint8_t SizeX = 0;
static uint8_t SizeY = 0;
static void* Data;
static LedControl* LC;

LedVisualizer::LedVisualizer(LedControl* lc, uint8_t sizeX, uint8_t sizeY, void* data) {
  LC = lc;
  SizeX = sizeX;
  SizeY = sizeY;
  Data = data;
}

void LedVisualizer::display() {
    uint8_t (*p_arr)[SizeY][SizeX] = Data;
    for (uint8_t y = 0; y < SizeY; y++) {
      for (uint8_t x = 0; x < SizeX; x++)
      {
        uint8_t addr = x >> 3;
        if ((*p_arr)[y][x] == 0)
          LC->setLed(addr, y, x % 8, false);
        else 
          LC->setLed(addr, y, x % 8, true);
      }
    }
}
