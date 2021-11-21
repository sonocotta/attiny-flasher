#include "main.h"
#include "logo.h"
#include "isp.h"

ISP *isp = NULL;
#if defined(HVSP_PROGRAMMER)
#include "hvsp.h"
HVSP *hvsp = NULL;
#endif

void setup()
{
  Serial.begin(BAUDRATE_IN);

#if defined(SERIAL_SENSOR_EN) /* && !defined(SERIAL_DEBUG_ENABLE) */
  pinMode(SERIAL_SENSOR_PIN, INPUT);
  uint8_t _ser_mode = digitalRead(SERIAL_SENSOR_PIN);
#else
  uint8_t _ser_mode = LOW;
#endif
  if (_ser_mode == LOW)
  {
    SSERIAL_INIT;
    SERIAL_OUT1("? SERIAL MODE -> ON");
  }
  else
  {
    SSERIAL_DEINIT;
    SERIAL_OUT1("? SERIAL MODE -> OFF");
  }

#if defined(SW_SERIAL_ENABLE)
  isp = new ISP(sserial);
#else
  isp = new ISP();
#endif

#if defined(HVSP_PROGRAMMER)
#if defined(SW_SERIAL_ENABLE)
  hvsp = new HVSP(sserial);
#else
  hvsp = new HVSP();
#endif
  SERIAL_OUT1("? HV PROG -> ON");
#else
  SERIAL_OUT1("? HV PROG -> OFF");
#endif

#ifdef SERIAL_BRIDGE_ENABLE
  SERIAL_OUT1("? SERIAL BR -> ON");
#else
  SERIAL_OUT1("? SERIAL BR -> OFF");
#endif

#ifdef OLED_ENABLE
  SERIAL_OUT1("? OLED -> ON");
#else
  SERIAL_OUT1("? OLED -> OFF");
#endif

  BUFFER_INIT;

  // RESET TARGET
  RESET_INIT;
  RESET_LOW;
  _delay_ms(50);
  RESET_Z;

  pinMode(LED_PMODE, OUTPUT);
  pulse(LED_PMODE, 2);

  pinMode(LED_ERR, OUTPUT);
  pulse(LED_ERR, 2);

  pinMode(LED_HB, OUTPUT);
  pulse(LED_HB, 2);

#ifdef OLED_ENABLE

#ifdef OLED_LIB_ADAFRUIT
  if (!display->begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      pulse(LED_ERR, 1);
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
}

uint8_t reset_state_cnt = 0;
int8_t hv_mode = -1;

uint8_t ser_state_cnt = 0;
int8_t ser_mode = -1;
bool last_prog_mode = false;

void loop(void)
{
#if defined(SERIAL_SENSOR_EN)
  // check if in prog mode, since sserial blocks communication in hvsp mode
//   bool cur_prog_mode = false;
//   if (hv_mode)
//   {
// #if defined(HVSP_PROGRAMMER)
//     cur_prog_mode = hvsp->programming;
// #endif
//   }
  // else // sserial is not interferring with ISP programmer
  // {
  //   cur_prog_mode = isp->programming;
  // }

  // if (cur_prog_mode != last_prog_mode)
  // {
  //   if (cur_prog_mode)
  //   {
  //     SERIAL_OUT1("PROG MODE -> ON, SERIAL MODE -> OFF");
  //     SSERIAL_DEINIT;
  //     ser_mode = -1;
  //   }
  //   else
  //   {
  //     SERIAL_OUT1("PROG MODE -> OFF");
  //   }
  //   last_prog_mode = cur_prog_mode;
  // }

  // if (!cur_prog_mode)
  // {
    // take measurment each 255th time
    if (++ser_state_cnt == 0)
    {
      uint8_t _ser_mode = digitalRead(SERIAL_SENSOR_PIN);
      if (ser_mode != _ser_mode)
      {
        if (_ser_mode == HIGH)
        {
          SERIAL_OUT1("SERIAL MODE -> OFF");
          SSERIAL_DEINIT;
        }
        if (_ser_mode == LOW)
        {
          SSERIAL_INIT;
          SERIAL_OUT1("SERIAL MODE -> ON");
        }
        ser_mode = _ser_mode;
      }
    }
  // }
#else
  if (ser_mode == -1)
  {
    ser_mode = 0;
  }
#endif

#if defined(RESET_SENSOR_EN) && defined(HVSP_PROGRAMMER)
  if (!(hv_mode && hvsp->reset_locked) || (!hv_mode && isp->reset_locked))
  {
    // take measurment each 255th time
    if (++reset_state_cnt == 0)
    {
      RESET_SENSOR_READ;
      uint8_t _hv_mode = digitalRead(RESET_SENSOR_PIN);
      if (hv_mode != _hv_mode)
      {
        SERIAL_OUT("HV MODE -> ", _hv_mode);
        if (_hv_mode == HIGH)
        {
          hvsp->init();
        }
        if (_hv_mode == LOW)
        {
          isp->init();
        }
        hv_mode = _hv_mode;
      }
    }
  }
#else
  if (hv_mode == -1)
  {
    isp->init();
    hv_mode = 0;
  }
#endif

  if (hv_mode)
  {
#if defined(HVSP_PROGRAMMER)
    digitalWrite(LED_PMODE, hvsp->programming ? HIGH : LOW);
    digitalWrite(LED_ERR, hvsp->error ? HIGH : LOW);
    digitalWrite(LED_HB, (millis() % 256) < 4 ? HIGH : LOW);

    if (Serial.available())
      hvsp->avrisp();
#endif
  }
  else
  {
    digitalWrite(LED_PMODE, isp->programming ? HIGH : LOW);
    digitalWrite(LED_ERR, isp->error ? HIGH : LOW);
    digitalWrite(LED_HB, (millis() % 1024) < 8 ? HIGH : LOW);

    if (Serial.available())
      isp->avrisp();
  }

#ifdef SERIAL_BRIDGE_ENABLE
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

#ifdef SERIAL_BRIDGE_ENABLE

#define bufferSize 64
uint8_t buf[bufferSize];
uint8_t i = 0;

void serial_bridge_loop()
{
  if (isp->programming || isp->serial_busy)
    return;

  if (sserial != NULL && sserial->available())
  {
    while (1)
    {
      if (sserial != NULL && sserial->available())
      {
        buf[i] = (char)sserial->read();
        if (i < bufferSize - 1)
          i++;
      }
      else
      {
        if (!sserial->available())
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
#ifdef MEGAFLASHER
  display->print(F("MEGA"));
#else
  display->print(F("ATTINY"));
#endif
  display->setCursor(72, 16);
  display->print(F("FLASHER"));
  display->setCursor(72, 32);
#ifdef FLASHER_REV_C
  display->print(F("Rev.C by"));
#endif
#ifdef FLASHER_REV_D
  display->print(F("Rev.D by"));
#endif
#ifdef FLASHER_REV_E
  display->print(F("Rev.E by"));
#endif
#ifdef FLASHER_REV_F
  display->print(F("Rev.F by"));
#endif
#ifdef FLASHER_REV_G
  display->print(F("Rev.G by"));
#endif
#ifdef MEGAFLASHER_REV_F
  display->print(F("Rev.F by"));
#endif
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
#ifdef MEGAFLASHER
  display->print(F("MEGA"));
#else
  display->print(F("ATTINY"));
#endif
  display->setCursor(72, 2);
  display->print(F("FLASHER"));
  display->setCursor(72, 4);
#ifdef FLASHER_REV_C
  display->print(F("Rv.C by"));
#endif
#ifdef FLASHER_REV_D
  display->print(F("Rv.D by"));
#endif
#ifdef FLASHER_REV_E
  display->print(F("Rv.E by"));
#endif
#ifdef FLASHER_REV_F
  display->print(F("Rv.F by"));
#endif
#ifdef FLASHER_REV_G
  display->print(F("Rv.G by"));
#endif
#ifdef MEGAFLASHER_REV_F
  display->print(F("Rv.F by"));
#endif
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