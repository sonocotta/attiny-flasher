#include <Arduino.h>
#include <util/delay.h>
#include <SoftwareSerial.h>
 
#define PIN_RX -1
#define PIN_TX 4
#define SERIAL_BAUD 19200

SoftwareSerial mySerial(PIN_RX, PIN_TX);

#define PINS_COUNT 4

void setup() {
  for (uint8_t i = 0; i < PINS_COUNT; i++)
    pinMode(i, OUTPUT);
  mySerial.begin(SERIAL_BAUD);
  mySerial.println(F("Hello,")); 
  mySerial.println(F("I'm"));
  mySerial.println(F("ATTINY85!"));
}

uint8_t state = 0;

void loop() {
  uint8_t pinNumber = state++ % PINS_COUNT;
  digitalWrite(pinNumber, HIGH);
  _delay_ms(250);
  digitalWrite(pinNumber, LOW);
  // mySerial.println(millis());
  _delay_ms(250);
}

