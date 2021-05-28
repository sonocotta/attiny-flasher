// ArduinoISP
// Copyright (c) 2008-2011 Randall Bohn
// If you require a license, see
// http://www.opensource.org/licenses/bsd-license.php
//
// This sketch turns the Arduino into a AVRISP:

#include "main.h"
#include "logo.h"
#include "isp.h"

ISP *isp = new ISP(&SSerial);

void setup()
{
  Serial.begin(BAUDRATE_IN);

#ifdef SW_SERIAL_ENABLE
  SSerial.begin(BAUDRATE_OUT);
#endif

  pinMode(LED_PMODE, OUTPUT);
  pulse(LED_PMODE, 2);

  pinMode(LED_ERR, OUTPUT);
  pulse(LED_ERR, 2);

  pinMode(LED_HB, OUTPUT);
  pulse(LED_HB, 2);

  isp->init();

#ifdef OLED_ENABLE

#ifdef OLED_LIB_ADAFRUIT
  if (!display->begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    // TODO: blink red led
    for (;;)
      ;
  }

  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(SSD1306_WHITE);
  display->setCursor(0, 0);
  display->cp437(true);
#endif

#ifdef OLED_LIB_TINY
  display->begin(SCREEN_WIDTH, SCREEN_HEIGHT, sizeof(tiny4koled_init_128x64br), tiny4koled_init_128x64br);
#endif

  logo_show(display);

#ifdef OLED_LIB_TINY
  //display->setFont(FONT6X8);
  display->on();
#endif
#endif

#ifdef SW_SERIAL_ENABLE
  SSerial.listen();
#endif

#ifdef SERIAL_DEBUG_ENABLE
  SSerial.println(F("Ready..."));
#endif
}

void loop(void)
{
  digitalWrite(LED_PMODE, isp->programming ? HIGH : LOW);
  digitalWrite(LED_ERR, isp->error ? HIGH : LOW);
  digitalWrite(LED_HB, millis() % 1000 < 10 ? HIGH : LOW);

  if (Serial.available())
    isp->avrisp();

#ifdef SERAIL_BRIDGE_ENABLE
  serial_bridge_loop();
#endif

#ifdef OLED_ENABLE
  logo_loop(display);
#endif
}

void pulse(int pin, int times)
{
  do
  {
    digitalWrite(pin, HIGH);
    _delay_ms(PULSE_TIME);
    digitalWrite(pin, LOW);
    _delay_ms(PULSE_TIME);
  } while (times--);
}

#ifdef SERAIL_BRIDGE_ENABLE

#define bufferSize 64
uint8_t buf[bufferSize];
uint8_t i = 0;

void serial_bridge_loop()
{
  if (isp->programming || isp->serial_busy)
    return;

  if (SSerial.available())
  {
    while (1)
    {
      if (SSerial.available())
      {
        buf[i] = (char)SSerial.read();
        if (i < bufferSize - 1)
          i++;
      }
      else
      {
        if (!SSerial.available())
        {
          break;
        }
      }
    }

    for (uint8_t j = 0; j < i; j++)
    {
      if (!isp->programming)
        Serial.print((char)buf[j]);

#ifdef OLED_ENABLE
#ifdef OLED_LIB_ADAFRUIT
      display->print((char)buf[j]);
#endif

#ifdef OLED_LIB_TINY
      if (buf[j] == '\n')
      {
        if (display->getCursorY() << 3 >= SCREEN_HEIGHT - SCREEN_ROW_HEIGHT)
        {
          display->clear();
          display->setCursor(0, 0);
        }
        else
        {
          display->print((char)buf[j]);
        }
      }
      else
        display->print((char)buf[j]);
#endif
#endif
    }
#ifdef OLED_ENABLE
#ifdef OLED_LIB_ADAFRUIT
    display->display();
#endif

#ifdef OLED_LIB_ADAFRUIT
    if (display->getCursorY() > SCREEN_HEIGHT - SCREEN_ROW_HEIGHT)
    {
      display->clearDisplay();
      display->setCursor(0, 0);
    }
#endif

    i = 0;
#endif
  }
}
#endif

#ifdef OLED_ENABLE

bool logo_hidden = true;

void logo_show(Ssd1306 *display)
{
#ifdef OLED_LIB_ADAFRUIT
  display->clearDisplay();
  display->drawBitmap(0, 0, bitmap_logo, LOGO_BMPWIDTH, LOGO_BMPHEIGHT, SSD1306_WHITE);

  display->setTextSize(1);
  display->setCursor(72, 0);
  display->print(F("ATTINY"));
  display->setCursor(72, 16);
  display->print(F("FLASHER"));
  display->setCursor(72, 32);
  display->print(F("Rev.E by"));
  display->setCursor(72, 48);
  display->print(F("SONOCOTTA"));
  display->display();
  logo_hidden = false;
#endif

#ifdef OLED_LIB_TINY
  display->setFont(FONT8X16);
  display->clear();
  display->bitmap(0, 0, LOGO_BMPWIDTH, 8, bitmap_logo);
  display->setCursor(72, 0);
  display->print(F("ATTINY"));
  display->setCursor(72, 2);
  display->print(F("FLASHER"));
  display->setCursor(72, 4);
  display->print(F("rv.E by"));
  display->setCursor(56, 6);
  display->print(F("SONOCOTTA"));
  display->on();
  logo_hidden = false;
#endif
}

void logo_loop(Ssd1306 *display)
{
#ifdef OLED_LIB_ADAFRUIT
  if (!logo_hidden)
    if (millis() > LOGO_SHOW_MSEC)
    {
      display->clearDisplay();
      display->setCursor(0, 0);
      display->display();
      logo_hidden = true;
    }
#endif

#ifdef OLED_LIB_TINY
  if (!logo_hidden)
    if (millis() > LOGO_SHOW_MSEC)
    {
      display->clear();
      display->setCursor(0, 0);
      logo_hidden = true;
    }
#endif
}

#endif