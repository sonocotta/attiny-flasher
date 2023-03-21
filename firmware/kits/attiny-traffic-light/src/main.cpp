#include <Arduino.h>

#ifdef REV_A
#define PIN_BTN 4
#define PIN_GRN 0
#define PIN_YLW 1
#define PIN_RED 2
#ifdef FEATURES_ARROW
#define PIN_ARR 3
#endif
#endif

#ifdef REV_B
#define PIN_BTN 3
#define PIN_GRN 4
#define PIN_YLW 1
#define PIN_RED 0
#ifdef FEATURES_ARROW
#define PIN_ARR 2
#endif
#endif

#ifdef FEATURES_ARROW
#define PIN_ARR 3
#endif

#define LED_OFF LOW
#define LED_ON HIGH

#define BTN_OFF HIGH
#define BTN_ON LOW

#define OFF_AFTER_SEC 5

enum State
{
  OFF,
  ON,
  GREEN,
  YELLOW_TO_RED,
  RED,
  YELLOW_TO_GREEN,
  TO_OFF
};

State current_state = OFF;

void do_demo_pin(uint8_t pin)
{
  for (uint8_t x = 0; x < 0xff; x++)
  {
    analogWrite(pin, x);
    delay(1);
  }
  for (uint8_t x = 0xff; x > 0; x--)
  {
    analogWrite(pin, x);
    delay(1);
  }
  analogWrite(pin, 0);
}

void setup()
{
  pinMode(PIN_BTN, INPUT);
  pinMode(PIN_GRN, OUTPUT);
  pinMode(PIN_YLW, OUTPUT);
  pinMode(PIN_RED, OUTPUT);

#ifdef FEATURES_ARROW
  pinMode(PIN_ARR, OUTPUT);
  digitalWrite(PIN_ARR, LOW);
#endif

#ifdef DEMO_MODE
  while (1)
  {
    do_demo_pin(PIN_GRN);
    do_demo_pin(PIN_YLW);
    do_demo_pin(PIN_RED);
  }
#endif
}

uint16_t last_off = 0;
#ifdef FEATURES_ARROW
uint8_t arrow_flag = 0;
uint8_t arrow_seq = 0;
#endif

void loop()
{
  uint8_t btn_state = digitalRead(PIN_BTN);
  if (btn_state == BTN_OFF)
  {
    last_off = millis() >> 10;
  }
  if (btn_state == BTN_ON)
  {
    uint16_t now = millis() >> 10;
    uint16_t sec_on = now - last_off;
    // check if long press
    if (sec_on >= OFF_AFTER_SEC)
    {
      current_state = TO_OFF;
    }
  }

  switch (current_state)
  {
  case TO_OFF:
    if (btn_state == BTN_ON)
    {
      digitalWrite(PIN_YLW, OFF);
      digitalWrite(PIN_RED, OFF);
      digitalWrite(PIN_GRN, OFF);
    }
    if (btn_state == BTN_OFF)
    {
      current_state = OFF;
    }
    break;

  case OFF:
    if (btn_state == BTN_OFF)
    {
      uint16_t seconds = millis() >> 10;
      uint8_t yellow_state = seconds % 2;
      digitalWrite(PIN_YLW, yellow_state);
    }
    if (btn_state == BTN_ON)
    {
      current_state = ON;
    }
    break;

  case ON:
    if (btn_state == BTN_ON)
    {
      digitalWrite(PIN_YLW, ON);
    }
    if (btn_state == BTN_OFF)
    {
      current_state = GREEN;
    }
    break;

  case GREEN:
    if (btn_state == BTN_OFF)
    {
      digitalWrite(PIN_RED, LED_OFF);
      digitalWrite(PIN_YLW, LED_OFF);
      digitalWrite(PIN_GRN, LED_ON);
    }
    if (btn_state == BTN_ON)
    {
      current_state = YELLOW_TO_RED;
    }
    break;

  case YELLOW_TO_RED:
    if (btn_state == BTN_ON)
    {
      digitalWrite(PIN_GRN, LED_OFF);
      digitalWrite(PIN_YLW, LED_ON);
    }
    if (btn_state == BTN_OFF)
    {
      current_state = RED;
    }
    break;

  case RED:
    if (btn_state == BTN_OFF)
    {
      digitalWrite(PIN_YLW, LED_OFF);
      digitalWrite(PIN_RED, LED_ON);
#ifdef FEATURES_ARROW
      if (arrow_flag == 0)
      {
        bool should_switch_on = arrow_seq++ % 2 == 0;
        //millis() % 2 == 0;
        if (should_switch_on)
          digitalWrite(PIN_ARR, LED_ON);
        arrow_flag = 1;
      }
#endif
    }
    if (btn_state == BTN_ON)
    {
      current_state = YELLOW_TO_GREEN;
#ifdef FEATURES_ARROW
      arrow_flag = 0;
#endif
    }
    break;

  case YELLOW_TO_GREEN:
    if (btn_state == BTN_ON)
    {
#ifdef FEATURES_ARROW
      digitalWrite(PIN_ARR, LED_OFF);
#endif
      digitalWrite(PIN_RED, LED_ON);
      digitalWrite(PIN_YLW, LED_ON);
    }
    if (btn_state == BTN_OFF)
    {
      current_state = GREEN;
    }
    break;
  }
}