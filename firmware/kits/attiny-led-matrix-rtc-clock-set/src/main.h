#pragma once

#include <ATtinySerialOut.h>

#define SERIAL_BAUD 115200

#include "LedControl.h"

#define PIN_SPI_CLK    2
#define PIN_SPI_CS     1
#define PIN_SPI_DATA   0
#define LED_MATRIX_CNT 2

LedControl* lc = new LedControl(PIN_SPI_DATA, PIN_SPI_CLK, PIN_SPI_CS, LED_MATRIX_CNT);

#include "ThreeWireInv.h"
#include <RtcDS1302.h>

ThreeWireInv myWire(PIN_SPI_DATA, PIN_SPI_CLK, PIN_SPI_CS); // IO, SCLK, CE
RtcDS1302<ThreeWireInv> Rtc(myWire);
