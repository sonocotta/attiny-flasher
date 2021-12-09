#include <Arduino.h>
#include <util/delay.h>

#ifdef LIGHT_SERIAL
#include <ATtinySerialOut.h>
#endif
#ifdef SW_SERIAL
#include <SoftwareSerial.h>
#endif

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

#if defined(HW_SERIAL) || defined(SW_SERIAL) || defined(LIGHT_SERIAL)
#define SERIAL_EN
#define SERIAL_BAUD 115200
#endif

void printDateTime(const RtcDateTime& dt);

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.println(F("Hello"));

  //_delay_ms(1000);

  Rtc.Begin();
  _delay_ms(2000);
  RtcDateTime now = Rtc.GetDateTime();
  printDateTime(now);
  Serial.println();
  
  while (true) {
    _delay_ms(2000); 
    
    RtcDateTime now = Rtc.GetDateTime();

    printDateTime(now);
    Serial.print('\t');
    uint8_t seed = now.Second() + now.Minute();
    Serial.println(seed);

    if (!now.IsValid())
    {
        Serial.println(":(");
    }
  }
}

void loop() { 

}


#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
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
            dt.Second() );
    Serial.print(datestring);
}

