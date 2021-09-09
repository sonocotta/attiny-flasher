#include <Arduino.h>
#include <util/delay.h>
#include "main.h"

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

