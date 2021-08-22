#include <Arduino.h>
#include <util/delay.h>

#if defined(HW_SERIAL) || defined(SW_SERIAL) || defined(LIGHT_SERIAL)
#define SERIAL_EN
#define SERIAL_BAUD 115200
#endif

#ifdef SERIAL_EN
  #ifdef _ATmega_x8_
    #define START_PIN  2
    #define PINS_COUNT 14
  #else 
    #define START_PIN  0
    #define PINS_COUNT 3
  #endif
#else
  #ifdef _ATmega_x8_
    #define START_PIN  0
    #define PINS_COUNT 16
  #else 
    #define START_PIN  0
    #define PINS_COUNT 6
  #endif
#endif

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
#endif

void setup() {
  for (uint8_t i = 0; i < PINS_COUNT; i++)
    pinMode(START_PIN + i, OUTPUT);
  
  #ifdef SERIAL_EN
  Serial.begin(SERIAL_BAUD);
  Serial.println(F("Hello,")); 
  Serial.println(F("I'm"));
  #if defined(__AVR_ATtiny85__)
  Serial.println(F("ATTINY85!"));
  #endif
  #if defined(__AVR_ATtiny13__)
  Serial.println(F("ATTINY13!"));
  #endif
  #if defined(__AVR_Atmega328__)
  Serial.println(F("ATMEGA328P!"));
  #endif
  #endif
}

uint8_t state = 0;

void loop() {
  uint8_t pinNumber = state++ % PINS_COUNT;
  digitalWrite(START_PIN + pinNumber, HIGH);
  _delay_ms(250);
  digitalWrite(START_PIN + pinNumber, LOW);
  
  #ifdef SERIAL_EN
  Serial.println(millis());
  #endif
  
  _delay_ms(250);
}