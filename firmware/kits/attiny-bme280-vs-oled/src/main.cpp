#include <Arduino.h>
#include <util/delay.h>
#include "main.h"

#include <Wire.h>
#include <Tiny4kOLED.h>

void setup()
{
#ifdef _SERIAL_
  SSerial.begin(SERIAL_BAUD);
  //SSerial.println(F("Waking up..."));
#endif

  oled.begin(OLED_WIDTH, OLED_HEIGHT, sizeof(tiny4koled_init_128x32br), tiny4koled_init_128x32br);
  oled.setFont(FONT6X8CAPS);
  oled.clear();
  oled.on();

  if (!sens.beginI2C(0x76))
  {
    //SSerial.println(F("BMP280 init failed"));
    while (true)
      ;
  }
}

void temp()
{
  int32_t temp = sens.readFixedTempC();
  SSerial.print(F("T = "));
  SSerial.println(temp);

  oled.print(F("T = "));
  print_num(temp, 4, 2);
  oled.println(F(" C"));
}

void humi()
{
  int32_t humi = sens.readFixedHumidity();
  SSerial.print(F("H = "));
  SSerial.println(humi);

  oled.print(F("H = "));
  print_num(humi, 5, 3);
  oled.println(F(" %"));
}

void pres()
{
  int32_t pres = sens.readFixedPressure();
  SSerial.print(F("P = "));
  SSerial.println(pres);

  oled.print(F("P = "));
  oled.print(pres);
  oled.println(F(" PA"));
}

void loop()
{
  oled.clear();
  oled.setCursor(0, 0);

  temp();
  humi();
  pres();

  _delay_ms(500);
}

void print_num(uint32_t num, uint8_t digits, uint8_t point)
{
  int8_t pos = 0;
  uint8_t res[digits];
  while (pos < digits && num > 0)
  {
    uint8_t n = num % 10;
    res[pos] = n;
    num = num / 10;
    pos++;
  }

  for (pos = pos - 1; pos >= 0; pos--)
  {
    oled.print(res[pos]);
    if (pos == point)
      oled.print('.');
  }
}