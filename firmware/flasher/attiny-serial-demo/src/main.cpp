#include <Arduino.h>
#include <util/delay.h>
#ifdef LIGHT_SERIAL
#include <ATtinySerialOut.h>
#endif
#ifdef SW_SERIAL
#include <SoftwareSerial.h>
#endif

#ifdef SW_SERIAL
#define PIN_RX -1
#define PIN_TX 4
SoftwareSerial mySerial(PIN_RX, PIN_TX);
#define Serial mySerial

#define PINS_COUNT 3
#else
#define PINS_COUNT 6
#endif



void setup() {
  for (uint8_t i = 0; i < PINS_COUNT; i++)
    pinMode(i, OUTPUT);
  
  #if defined(SW_SERIAL) || defined(LIGHT_SERIAL)
  Serial.begin(SERIAL_BAUD);
  Serial.println(F("Hello,")); 
  Serial.println(F("I'm"));
  Serial.println(F("ATTINY85!"));
  #endif
}

uint8_t state = 0;

void loop() {
  uint8_t pinNumber = state++ % PINS_COUNT;
  digitalWrite(pinNumber, HIGH);
  _delay_ms(250);
  digitalWrite(pinNumber, LOW);
  
  #if defined(SW_SERIAL) || defined(LIGHT_SERIAL)
  Serial.println(millis());
  #endif
  
  _delay_ms(250);
}

