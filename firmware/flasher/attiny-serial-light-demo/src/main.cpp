#include <Arduino.h>
#include <util/delay.h>
#include <ATtinySerialOut.h>

#define SERIAL_BAUD 115200
 
#ifdef _SW_SERIAL_
#define PIN_RX -1
#define PIN_TX 4
#endif

#define PINS_COUNT 4

void setup() {
  for (uint8_t i = 0; i < PINS_COUNT; i++)
    pinMode(i, OUTPUT);
  Serial.begin(SERIAL_BAUD);
  Serial.println(F("Hello,")); 
  Serial.println(F("I'm"));
  Serial.println(F("ATTINY85!"));
}

uint8_t state = 0;

void loop() {
  uint8_t pinNumber = state++ % PINS_COUNT;
  digitalWrite(pinNumber, HIGH);
  _delay_ms(250);
  digitalWrite(pinNumber, LOW);
  Serial.println(millis());
  _delay_ms(250);
}

