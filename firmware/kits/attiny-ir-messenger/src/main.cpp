#include <util/delay.h>
#include <Arduino.h>
#include <IRremote.h>

#define PIN_IR_SND   4
#define SAMS_CRR_FQ 38
#define SAMS_ADDR   0xE0E0
#define SAMS_VOL_UP 0xE01F
#define SAMS_VOL_DN 0xD02F

#define PIN_BTN_1    0
#define PIN_BTN_2    2

#define SERIAL_BAUD 115200
 
#ifdef _SW_SERIAL_
#include <SoftwareSerial.h>
#define PIN_RX -1
#define PIN_TX 4
SoftwareSerial mySerial(PIN_RX, PIN_TX);
#define Serial mySerial
#endif

IRsend irsend;

void setup()
{
    #ifdef _SW_SERIAL_
    Serial.begin(SERIAL_BAUD);
    Serial.print(F("Ready at pin "));
    Serial.println(PIN_IR_INPUT);
    #endif

    pinMode(PIN_BTN_1, INPUT);
    pinMode(PIN_BTN_2, INPUT);
    irsend.enableIROut(SAMS_CRR_FQ);
}

uint8_t x = 0;
decode_results results;

void loop()
{
  if (digitalRead(PIN_BTN_1) == LOW) {
    irsend.sendSAMSUNG(SAMS_ADDR, SAMS_VOL_UP);
    _delay_ms(5) ;  
  } else 
  if (digitalRead(PIN_BTN_2) == LOW) {
    irsend.sendSAMSUNG(SAMS_ADDR, SAMS_VOL_DN);
    _delay_ms(5) ;  
  }
  _delay_ms(5) ;
}

// uint8_t state = 0;

// void loop() {
//   digitalWrite(1, state++ % 2 == 0 ? HIGH : LOW);
//   _delay_ms(1000) ; 
// }