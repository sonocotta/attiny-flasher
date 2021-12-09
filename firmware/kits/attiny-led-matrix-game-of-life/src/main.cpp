#include <Arduino.h>
#include <util/delay.h>
#include <LedControl.h>
#include "LifeGame.h"
#include "ConsoleVisualizer.h"
#include "LedVisualizer.h"

#if defined(HW_SERIAL) || defined(SW_SERIAL) || defined(LIGHT_SERIAL)
#define SERIAL_EN
#define SERIAL_BAUD 115200
#endif

#ifdef LIGHT_SERIAL
#include <ATtinySerialOut.h>
#endif
#ifdef SW_SERIAL
#include <SoftwareSerial.h>
SoftwareSerial mySerial(-1, TX_PIN);
#define Serial mySerial
#endif

#define PIN_SPI_CLK    2
#define PIN_SPI_CS     1
#define PIN_SPI_DATA   0
#define LED_MATRIX_CNT 2

LedControl* lc = new LedControl(PIN_SPI_DATA, PIN_SPI_CLK, PIN_SPI_CS, LED_MATRIX_CNT);

#define TIME_PER_STEP_MS 100

#define GRID_SIZE_X 16
#define GRID_SIZE_Y 8

uint8_t grid[GRID_SIZE_X][GRID_SIZE_Y] = {};

#ifdef SERIAL_EN
ConsoleVisualizer *console = new ConsoleVisualizer(GRID_SIZE_X, GRID_SIZE_Y, grid);
#endif
LedVisualizer *led = new LedVisualizer(lc, GRID_SIZE_X, GRID_SIZE_Y, grid);
LifeGame *game = new LifeGame(GRID_SIZE_X, GRID_SIZE_Y, grid);

void init_led()
{
  uint8_t devices = lc->getDeviceCount();
  for (uint8_t address = 0; address < devices; address++)
  {
    lc->shutdown(address, false);
    lc->setIntensity(address, 8);
    lc->clearDisplay(address);
  }
}

void setup()
{
  #ifdef SERIAL_EN
  Serial.begin(SERIAL_BAUD);
  Serial.println(F("Hello"));
  _delay_ms(1000);
  #endif

  init_led();
  game->init();
}

void loop()
{
  Serial.println(F("loop"));

#ifdef SERIAL_EN
//  console->display();
#endif
  led->display();

  _delay_ms(TIME_PER_STEP_MS);
  game->doStep();
}