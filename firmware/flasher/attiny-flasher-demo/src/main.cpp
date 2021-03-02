#include <Arduino.h>
#include <util/delay.h>
#ifdef _SERIAL_
#include <SoftwareSerial.h>
 
#define PIN_RX -1
#define PIN_TX 4
#define SERIAL_BAUD 19200

SoftwareSerial mySerial(PIN_RX, PIN_TX);

#define PINS_COUNT 4
#else
#define PINS_COUNT 5
#endif



void setup() {
  for (uint8_t i = 0; i < PINS_COUNT; i++)
    pinMode(i, OUTPUT);
  _delay_ms(100);
  
  #ifdef _SERIAL_
  mySerial.begin(SERIAL_BAUD);
  mySerial.println(F("Hello,")); 
  mySerial.println(F("I'm")); 
  mySerial.println(F("ATTINY85!"));
  #endif
}

uint8_t state = 0;

void loop() {
  uint8_t pinNumber = state++ % PINS_COUNT;
  digitalWrite(pinNumber, HIGH);
  _delay_ms(250);
  digitalWrite(pinNumber, LOW);
  #ifdef _SERIAL_
  mySerial.println(millis());
  #endif
  _delay_ms(250);
}

