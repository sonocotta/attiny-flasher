#include <util/delay.h>
#include <IRremote.h>
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)
#include "ATtinySerialOut.h"
#endif

//#define SAMS_ADDR   0xE0E0
#define SAMS_VOL_UP 0xE0E0E01F
#define SAMS_VOL_DN 0xE0E0D02F

#define PIN_BTN_1    0
#define PIN_BTN_2    2

#define IR_SEND_PIN  1
#define SERIAL_BAUD 115200

IRsend IrSender;

void setup() {
    Serial.begin(SERIAL_BAUD);
    Serial.println(IR_SEND_PIN);
    Serial.println(F("Send with standard protocol encoders"));

    pinMode(PIN_BTN_1, INPUT);
    pinMode(PIN_BTN_2, INPUT);
    
}

uint8_t repeats = 0;

void loop()
{
  if (digitalRead(PIN_BTN_1) == LOW) {
    IrSender.sendSAMSUNG(SAMS_VOL_UP, 32);
    _delay_ms(5) ;  
  } else 
  if (digitalRead(PIN_BTN_2) == LOW) {
    IrSender.sendSAMSUNG(SAMS_VOL_DN, 32);

    _delay_ms(5) ;  
  }
  _delay_ms(5) ;
}

// void loop() {
//     /*
//      * Print values
//      */
//     Serial.println();
//     Serial.print(F("address=0x"));
//     Serial.print(sAddress, HEX);
//     Serial.print(F(" command=0x"));
//     Serial.print(sCommand, HEX);
//     Serial.print(F(" repeats="));
//     Serial.print(sRepeats);
//     Serial.println();

//     Serial.println(F("Send Samsung"));
//     IrSender.sendSamsung(sAddress, sCommand, sRepeats);
//     delay(2000);

//     /*
//      * Increment values
//      */
//     sAddress += 0x0101;
//     sCommand += 0x11;
//     sRepeats++;
//     // clip repeats at 4
//     if (sRepeats > 4) {
//         sRepeats = 4;
//     }

//     delay(5000); //  second additional delay between each values
// }
