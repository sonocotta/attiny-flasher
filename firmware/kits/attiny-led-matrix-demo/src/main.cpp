#include <Arduino.h>
#include <util/delay.h>

#ifdef LIGHT_SERIAL
#include <ATtinySerialOut.h>
#endif
#ifdef SW_SERIAL
#include <SoftwareSerial.h>
SoftwareSerial mySerial(-1, TX_PIN);
#define Serial mySerial
#endif

#define PIN_SPI_CLK 2
#define PIN_SPI_CS 1
#define PIN_SPI_DATA 0
#define LED_MATRIX_CNT 2

#ifdef DEMO_SCREEN
#define GRID_SIZE_X 16
#define GRID_SIZE_Y 8

#include "LedControl.h"

LedControl *lc = new LedControl(PIN_SPI_DATA, PIN_SPI_CLK, PIN_SPI_CS, LED_MATRIX_CNT);
#endif

#if defined(HW_SERIAL) || defined(SW_SERIAL) || defined(LIGHT_SERIAL)
#define SERIAL_EN
#define SERIAL_BAUD 115200
#endif

#ifdef DEMO_RTC
#include "ThreeWireInv.h"
#include <RtcDS1302.h>

ThreeWireInv myWire(PIN_SPI_DATA, PIN_SPI_CLK, PIN_SPI_CS); // IO, SCLK, CE
RtcDS1302<ThreeWireInv> Rtc(myWire);
#endif

#ifdef DEMO_SCREEN

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

void test_screen()
{
  int devices = lc->getDeviceCount();

  for (uint8_t i = 0; i < GRID_SIZE_Y; i++)
  {
    for (uint8_t address = 0; address < devices; address++)
      lc->setColumn(address, i, 0xff);
    _delay_ms(100);
    for (uint8_t address = 0; address < devices; address++)
      lc->setColumn(address, i, 0x00);
  }

  for (int8_t i = GRID_SIZE_Y - 1; i >= 0; i--)
  {
    for (uint8_t address = 0; address < devices; address++)
      lc->setColumn(address, i, 0xff);
    _delay_ms(100);
    for (uint8_t address = 0; address < devices; address++)
      lc->setColumn(address, i, 0x00);
  }

  for (uint8_t i = 0; i < GRID_SIZE_X; i++)
  {
    lc->setRow(i >> 3, i % GRID_SIZE_Y, 0xff);
    _delay_ms(100);
    lc->setRow(i >> 3, i % GRID_SIZE_Y, 0x00);
  }

  for (int8_t i = GRID_SIZE_X - 1; i >= 0; i--)
  {
    lc->setRow(i >> 3, i % GRID_SIZE_Y, 0xff);
    _delay_ms(100);
    lc->setRow(i >> 3, i % GRID_SIZE_Y, 0x00);
  }

  for (uint8_t i = 0; i < GRID_SIZE_Y; i++)
  {
    for (uint8_t address = 0; address < devices; address++)
      lc->setColumn(address, i, 0xff);
  }

  for (uint8_t x = 0; x < GRID_SIZE_Y; x++)
  {
    for (uint8_t address = 0; address < devices; address++)
      lc->setIntensity(address, x);
    _delay_ms(150);
  }

  for (uint8_t i = 0; i < GRID_SIZE_Y; i++)
  {
    for (uint8_t address = 0; address < devices; address++)
      lc->setColumn(address, i, 0x00);
  }

  _delay_ms(50);
}

#endif

#ifdef DEMO_RTC
void init_rtc()
{
  Rtc.Begin();
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime &dt)
{
  char datestring[20];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
             dt.Month(),
             dt.Day(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second());
  Serial.println(datestring);
}

void test_rtc()
{
  RtcDateTime now = Rtc.GetDateTime();
  printDateTime(now);
}

#endif

void setup()
{
  Serial.begin(SERIAL_BAUD);
  Serial.println(F("Hello"));

  // digitalWrite(PIN_SPI_CS, LOW);
  // pinMode(PIN_SPI_CS, INPUT);
  // while (1) ;

  _delay_ms(1000);

#ifdef DEMO_SCREEN
  init_led();
#endif

#ifdef DEMO_RTC
  init_rtc();
#endif
}

uint8_t counter = 0;

void loop()
{
#ifdef DEMO_RTC
  test_rtc();
#endif

#ifdef DEMO_SCREEN
if (counter++ % 10 == 0)
   test_screen();
#endif

  _delay_ms(500);
}