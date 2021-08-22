#include <Arduino.h>
#include "boards.h"
#include "main.h"

#ifdef REV_D_TWO_PIN_RESET
#define STATES_CNT 5
#else 
#define STATES_CNT 3
#endif

void pins_hv_flash() {
  pinMode(PIN_SCI, OUTPUT);
  pinMode(PIN_SDO, INPUT);
  pinMode(PIN_SII, OUTPUT);
  pinMode(PIN_SDI, OUTPUT);
}

void pins_lv_flash() {
  pinMode(PIN_SCI, INPUT);
  pinMode(PIN_SDO, OUTPUT);
  pinMode(PIN_SII, INPUT);
  pinMode(PIN_SDI, OUTPUT);
}

void setup() {
  Serial.begin(BAUDRATE_OUT);

  _BUFFER_INIT;
  _RESET_INIT;
  _RESET_Z;

  pinMode(LED_HB, OUTPUT);
  pinMode(LED_ERR, OUTPUT);
  pinMode(LED_PMODE, OUTPUT);

  Serial.println("Ready");
}

uint8_t state = 0;

void flash_pins_state(bool on) {
  digitalWrite(PIN_SCI, on);
  digitalWrite(PIN_SII, on);
  digitalWrite(PIN_SDI, on);
  digitalWrite(PIN_SDO, on);
}

void flash_pins() {
  for (uint8_t i = 0; i < 10; i++) {
    flash_pins_state(true);
    delay(50);
    flash_pins_state(false);
    delay(50);
  }
}

void loop()
{
  uint8_t _state = state++ % STATES_CNT;
  switch (_state) {
    case 0:
      _BUFFER_OFF
      _RESET_Z
      digitalWrite(LED_PMODE, HIGH);
      digitalWrite(LED_ERR, LOW);
      digitalWrite(LED_HB, LOW);

      flash_pins();
    break;

    case 1:
      pins_lv_flash();
      _BUFFER_ON
      _RESET_LOW

      digitalWrite(LED_PMODE, LOW);
      digitalWrite(LED_ERR, HIGH);
      digitalWrite(LED_HB, LOW);

      flash_pins();
    break;

    case 2:
      pins_lv_flash();
      _BUFFER_ON
      _RESET_HIGH

      digitalWrite(LED_PMODE, LOW);
      digitalWrite(LED_ERR, HIGH);
      digitalWrite(LED_HB, LOW);

      flash_pins();
    break;

    case 3: 
      pins_hv_flash();
      _BUFFER_HV_PROG
      _RESET_LOW

      digitalWrite(LED_PMODE, LOW);
      digitalWrite(LED_ERR, LOW);
      digitalWrite(LED_HB, HIGH);

      flash_pins();
    break;

    case 4: 
      pins_hv_flash();
      _BUFFER_HV_PROG
      _RESET_HIGH_12

      digitalWrite(LED_PMODE, LOW);
      digitalWrite(LED_ERR, LOW);
      digitalWrite(LED_HB, HIGH);

      flash_pins();
    break;
  }
   
  delay(2000);
}
