#include <util/delay.h>
#include <Arduino.h>
#include <IRremote.h>

#define PIN_IR_SND 1
#define PIN_IR_INPUT 3

#define SAMS_CRR_FQ 38
#define SAMS_ADDR 0xE0E0
#define SAMS_VOL_UP 0xE01F
#define SAMS_VOL_DN 0xD02F

#define PIN_BTN_1 0
#define PIN_BTN_2 2

#define SERIAL_BAUD 115200

#ifdef _SW_SERIAL_
#include <SoftwareSerial.h>
#define PIN_RX -1
#define PIN_TX 4
SoftwareSerial mySerial(PIN_RX, PIN_TX);
#define Serial mySerial
#endif

IRrecv irrecv(PIN_IR_INPUT /*, PIN_IR_RCV*/);

void setup()
{
#ifdef _SW_SERIAL_
  Serial.begin(SERIAL_BAUD);
  Serial.print(F("Ready at pin "));
  Serial.println(PIN_IR_SND);
#endif

  pinMode(PIN_BTN_1, INPUT);
  pinMode(PIN_BTN_2, INPUT);

  IrSender.begin(PIN_IR_SND, false);
  IrSender.enableIROut(SAMS_CRR_FQ);

  irrecv.enableIRIn();
}

uint8_t x = 0;
decode_results results;

void loop()
{
//   if (irrecv.decode(&results))
//   {
// #ifdef _SW_SERIAL_
//     Serial.println(results.value, HEX);
// #endif

//     /*
//     digitalWrite(PIN_IR_SND, HIGH);
//     _delay_ms(500) ;  
//     digitalWrite(PIN_IR_SND, LOW);
//     _delay_ms(150) ; 
//     */

//     // irsend.sendSAMSUNG(SAMS_ADDR, SAMS_VOL_UP);
//     irrecv.resume();
//   }

  if (digitalRead(PIN_BTN_1) == LOW)
  {
    IrSender.sendSAMSUNG(SAMS_ADDR, SAMS_VOL_UP);
#ifdef _SW_SERIAL_
    Serial.print('>');
#endif

    // digitalWrite(PIN_IR_SND, HIGH);
    // _delay_ms(500) ;
    // digitalWrite(PIN_IR_SND, LOW);
    // _delay_ms(15) ;

    _delay_ms(50);
  }

  if (digitalRead(PIN_BTN_2) == LOW)
  {
    IrSender.sendSAMSUNG(SAMS_ADDR, SAMS_VOL_DN);
#ifdef _SW_SERIAL_
    Serial.print('<');
#endif

    // digitalWrite(PIN_IR_SND, HIGH);
    // _delay_ms(500) ;
    // digitalWrite(PIN_IR_SND, LOW);
    // _delay_ms(15) ;

    _delay_ms(50);
  }
  _delay_ms(5);
}