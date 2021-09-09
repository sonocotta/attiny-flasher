#include <stdint.h>
#include <Arduino.h>
#include <util/delay.h>
#include "ConsoleVisualizer.h"

#ifdef LIGHT_SERIAL
#include <ATtinySerialOut.h>
#endif
#ifdef SW_SERIAL
#include <SoftwareSerial.h>
#endif

static uint8_t SizeX = 0;
static uint8_t SizeY = 0;
static void* Data;

ConsoleVisualizer::ConsoleVisualizer(uint8_t sizeX, uint8_t sizeY, void* data) {
  //uint8_t (*p_arr)[sizeY][sizeX] = (uint8_t (*)[sizeY][sizeX]) data;
  SizeX = sizeX;
  SizeY = sizeY;
  Data = data;
}

void ConsoleVisualizer::display() {
    uint8_t (*p_arr)[SizeY][SizeX] = Data;
    //Serial.println(SizeX);
    //Serial.println(SizeY);
    Serial.println('+');
    for (uint8_t y = 0; y < SizeY; y++) {
      Serial.print(y);
      for (uint8_t x = 0; x < SizeX; x++)
      {
        if ((*p_arr)[y][x] == 0)
          Serial.print('.');
        else 
          Serial.print('*');
        //_delay_ms(5);
      }
      Serial.println('<');
      _delay_ms(10);
    }
    Serial.println(' ');
}
