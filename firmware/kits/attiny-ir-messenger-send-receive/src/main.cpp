#include <util/delay.h>
#include <Arduino.h>

#ifdef _SW_SERIAL_
#include <ATtinySerialOut.h>
#endif

#define IRMP_INPUT_PIN 3
#define IRSND_OUTPUT_PIN 1
#define TONE_PIN 5

#define IRMP_PROTOCOL_NAMES 1
// #define IRMP_SUPPORT_NEC_PROTOCOL 1
#define IRMP_SUPPORT_SAMSUNG_PROTOCOL 1
#define IRSND_SUPPORT_SAMSUNG_PROTOCOL 1
/*
 * After setting the definitions we can include the code and compile it.
 */
#define USE_ONE_TIMER_FOR_IRMP_AND_IRSND // otherwise we get an error on AVR platform: redefinition of 'void __vector_8()
#include <irmp.hpp>
#include <irsnd.hpp>

// #define SAMS_CRR_FQ 38
// #define SAMS_ADDR   0xE0E0
// #define SAMS_VOL_UP 0xE01F
// #define SAMS_VOL_DN 0xD02F

#define PIN_BTN_1 0
#define PIN_BTN_2 2

#define SERIAL_BAUD 115200

uint16_t x = 0;
IRMP_DATA irmp_data;
IRMP_DATA irsnd_data;

void setup()
{
#ifdef _SW_SERIAL_
  Serial.begin(SERIAL_BAUD);
  Serial.print(F("Ready to receive at pin "));
  Serial.println(IRMP_INPUT_PIN);
  Serial.print(F("Ready to send at pin "));
  Serial.println(IRSND_OUTPUT_PIN);
#endif

  pinMode(IRMP_INPUT_PIN, INPUT);
  pinMode(PIN_BTN_1, INPUT);
  pinMode(PIN_BTN_2, INPUT);

  irmp_init();
  irsnd_init();

  irsnd_data.protocol = IRMP_SAMSUNG32_PROTOCOL;
  irsnd_data.address = 0x0707;
  irsnd_data.flags = 1; // repeat frame 1 time
  irsnd_data.command = 0;
}

void loop()
{
  /*
     * Check if new data available and get them
     */
  if (irmp_get_data(&irmp_data))
  {
    /*
     * Skip repetitions of command
     */
    if (!(irmp_data.flags & IRMP_FLAG_REPETITION))
    {
      /*
      * Here data is available and is no repetition -> evaluate IR command
      */
#ifdef _SW_SERIAL_
      Serial.println(irmp_data.command, HEX);
#endif
      irsnd_data = irmp_data;
    }
    irmp_result_print(&irmp_data);
  }

  if (x++ == 0)
  {
    if (irsnd_data.command != 0)
      if (!irsnd_send_data(&irsnd_data, true))
      {
        Serial.println(F("XXX"));
      }
#ifdef _SW_SERIAL_
    Serial.print(".");
#endif
  }
}

// void __loop()
// {
//   if (irrecv.decode(&results))
//   {
//     #ifdef _SW_SERIAL_
//     Serial.println(results.value, HEX);
//     #endif

//     // digitalWrite(PIN_IR_SND, HIGH);
//     // _delay_ms(500) ;
//     // digitalWrite(PIN_IR_SND, LOW);
//     // _delay_ms(150) ;

//     // irsend.sendSAMSUNG(SAMS_ADDR, SAMS_VOL_UP);
//     irrecv.resume();
//   }

//   _delay_ms(5) ;
// }