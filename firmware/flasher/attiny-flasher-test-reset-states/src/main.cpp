#include <Arduino.h>
#include "main.h"

//#define PIN_BST_ON 2

#define SERIAL_SPEED 9600

void pins_hv_flash() {
  pinMode(SCI, OUTPUT);
  pinMode(SDO, INPUT);
  pinMode(SII, OUTPUT);
  pinMode(SDI, OUTPUT);
}

void pins_lv_flash() {
  pinMode(SCI, INPUT);
  pinMode(SDO, OUTPUT);
  pinMode(SII, INPUT);
  pinMode(SDI, OUTPUT);
}

void setup() {
  Serial.begin(SERIAL_SPEED);

  pinMode(PIN_BUFEN, OUTPUT);
  pinMode(RESET, OUTPUT);
  pinMode(RESET_PULL, OUTPUT);
  RESET_Z;

  pinMode(LED_HB, OUTPUT);
  pinMode(LED_ERR, OUTPUT);
  pinMode(LED_PMODE, OUTPUT);

  // pinMode(SCI, OUTPUT);
  // pinMode(SDO, INPUT);
  // pinMode(SII, OUTPUT);
  // pinMode(SDI, OUTPUT);

  Serial.println("Ready");
}

uint8_t state = 0;

void flash_pins_state(bool on) {
  digitalWrite(SCI, on);
  digitalWrite(SII, on);
  digitalWrite(SDI, on);
  digitalWrite(SDO, on);
}

void flash_pins() {
  for (uint8_t i = 0; i< 10; i++) {
    flash_pins_state(true);
    delay(50);
    flash_pins_state(false);
    delay(50);
  }
}

void loop()
{
  uint8_t _state = state++ % 3;
  switch (_state) {
    case 0:
      BUFFER_OFF
      RESET_Z
      digitalWrite(LED_PMODE, HIGH);
      digitalWrite(LED_ERR, LOW);
      digitalWrite(LED_HB, LOW);

      flash_pins();
    break;

    case 1:
      pins_lv_flash();
      BUFFER_ON
      RESET_LOW

      digitalWrite(LED_PMODE, LOW);
      digitalWrite(LED_ERR, HIGH);
      digitalWrite(LED_HB, LOW);

      flash_pins();
    break;

    case 2: 
      pins_hv_flash();
      BUFFER_HV_PROG
      RESET_HIGH

      digitalWrite(LED_PMODE, LOW);
      digitalWrite(LED_ERR, LOW);
      digitalWrite(LED_HB, HIGH);

      flash_pins();
    break;
  }
   
  delay(2000);
}
