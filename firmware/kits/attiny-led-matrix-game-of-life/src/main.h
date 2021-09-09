#pragma once

#include "LifeGame.h"
#include "ConsoleVisualizer.h"
#include "LedVisualizer.h"

#ifdef LIGHT_SERIAL
#include <ATtinySerialOut.h>
#endif
#ifdef SW_SERIAL
#include <SoftwareSerial.h>
#endif

#define SERIAL_BAUD 115200

#include "LedControl.h"

#define PIN_SPI_CLK    2
#define PIN_SPI_CS     1
#define PIN_SPI_DATA   0
#define LED_MATRIX_CNT 2

LedControl* lc = new LedControl(PIN_SPI_DATA, PIN_SPI_CLK, PIN_SPI_CS, LED_MATRIX_CNT);