/**************************************************
 * TPI programmer for ATtiny4/5/9/10/20/40
 *
 * Make the connections as shown below.
 *
 * To use:
 ***** Buad rate must be set to 9600 ****
 *
 *  - Upload to arduino and power off
 *  - Connect ATtiny10 as shown
 *  - Power on and open the serial monitor
 *  - If things are working so far you should
 *    see "NVM enabled" and "ATtiny10/20/40 connected".
 *  - Input one-letter commands via serial monitor:
 *
 *    D = dump memory. Displays all current memory
 *        on the chip
 *
 *    E = erase chip. Erases all program memory
 *        automatically done at time of programming
 *
 *    P = write program. After sending this, paste
 *        the program from the hex file into the
 *        serial monitor.
 *
 *    S = set fuse. follow the instructions to set
 *        one of the three fuses.
 *
 *    C = clear fuse. follow the instructions to clear
 *        one of the three fuses.
 *
 *    L = Set Lock Bits No further programming & verification
 *        possible
 *
 *    H = Toggle High Voltage Programming  
 *
 *    T = Toggle +12v enabled by High, or Low
 *
 *    R/r = Quick reset
 *
 *  - Finally, power off the arduino and remove the
 *    Attiny10/20/40
 *
 *
 * Arduino                 ATtiny10               *
 * ----------+          +----------------         *
 * (SS#)  10 |--[R]-----| 6 (RESET#/PB3)          *
 *           |          |                         *
 * (MOSI) 11 |--[R]--+--| 1 (TPIDATA/PB0)         *
 *           |       |  |                         *
 * (MISO) 12 |--[R]--+  |                         *
 *           |          |                         *
 * (SCK)  13 |--[R]-----| 3 (TPICLK/PB1)          *
 * ----------+          +----------------         *
 *                                                *
 * ----------+          +----------------         *
 * (HVP)   9 |---       | 6 (RESET#/PB3)          *
 *           |          |                         *
 *                                                *
 *  -[R]-  =  a 220 - 1K Ohm resistor             *
 *                                                *
 *  this picture : 2011/12/08 by pcm1723          *
 *  modified :2015/02/27 by KD                    *
 *                                                *
 * thanks to pcm1723 for tpitest.pde upon which   *
 * this is based                                  *
 **************************************************
 Updates:
 
   Apr 02, 2018: Ksdsksd@gmail.com
                * Added Lock bit setting to main menu
                
   Jan 23, 2017: Ksdsksd@gmail.com
                * Thanks to InoueTaichi Fixed incorrect #define Tiny40 
                
   Mar 05, 2015: Ksdsksd@gamil.com
                * Added notifications to setting and clearing the system flags.
 
   Feb 23, 2015: Ksdsksd@gamil.com
                * Changed the programmer Diagram, This is the config I use, and get a sucessful programming of a tiny10 at 9600 baud.
                
   Mar 22, 2014: Ksdsksd@gmail.com
                * Added the quick reset to high before resetting the device.
                * Added code to stop the SPI and float the pins for testing the device while connected.

   Mar 20, 2014: Ksdsksd@gmail.com
                * Added a quick reset by sending 'r' or 'R' via the serial monitor.
                * Added a High voltage programming option from pin 9, toggled by 'H'
                * Added a High/low option for providing 12v to the reset pin, toggled by 'T'

   Mar 17, 2014: Ksdsksd@gmail.com
                * Had some trouble with the nibbles being swapped when programming on the 10 & 20,
                  added b1,b2 to hold the serial data before calling byteval()
                * Added Nat Blundell's patch to the code

   Apr 10, 2013: Ksdsksd@gmail.com
                * Applied Fix for setting and clearing flags

   Feb 7,  2013: Ksdsksd@gmail.com
                * Fixed programming timer, had intitial start at zero instead of current time.

   Dec 11, 2012: Ksdsksd@gmail.com
                * Added detect and programming for 4/5/9

   Dec 5-6, 2012: Ksdsksd@gmail.com
                * Incorperated read, and verify into program. Now have no program size limitation by using 328p.
                * Changed the outHex routines consolidated them into 1, number to be printed, and number of nibbles
                * Added a type check to distinguish between Tiny10/20/40
                * Added an auto word size check to ensure that there is the proper amount of words written for a 10/20/40
                * Removed Read program, Verify, and Finish from options
                * Changed baud rate to 19200 for delay from data written to the chip, to prevent serial buffer overrun.

   Oct 5, 2012: Ksdsksd@gmail.com
                *** Noticed that when programming, the verification fails
                    at times by last 1-2 bytes programmed, and the Tiny would act erratic.
                    Quick fix was adding 3 NOP's to the end the Tiny's code, and ignoring the errors, the Tiny then performed as expected.

   Oct 4, 2012: Ksdsksd@gmail.com
                * Moved all Serial printed strings to program space
                * Added code to detect Tiny20
*/

#include <Arduino.h>
#include <SPI.h>
#include "main.h"
#include "tpi.h"

// #define HVReset 9

// used for storing a program file
uint8_t data[16]; //program data

// used for various purposes
unsigned long startTime;
unsigned int timeout;
boolean idChecked;

// char HVP = 0;
// char HVON = 0;

void setup()
{
  // you cant increase this over 9600, it'll overrun the buffer
  Serial.begin(BAUDRATE_IN);
  sserial->begin(BAUDRATE_OUT);

  Serial.println('>');

  BUFFER_INIT;
  
  // RESET TARGET
  RESET_INIT;
  // RESET_LOW;
  // _delay_ms(50);
  // RESET_Z;

  BUFFER_ON;
  TPI::start();
  // pinMode(HVReset, OUTPUT);

  timeout = 20000;
  idChecked = false;
}

// void hvserial()
// {
//   if (HVP)
//     Serial.println(F("***High Voltage Programming Enabled***"));
//   else
//     Serial.println(F("High Voltage Programming Disabled"));

//   Serial.print(F("Pin 9 "));
//   Serial.print(HVON ? F("HIGH") : F("LOW"));
//   Serial.print(F(" supplies 12v"));
// }

// void hvReset(char highLow)
// {
//   // if (HVP)
//   // {
//   //   if (HVON)             // if high enables 12v
//   //     highLow = !highLow; // invert the typical reset
//   //   digitalWrite(HVReset, highLow);
//   // }
//   // else
//     digitalWrite(SS, highLow);
// }

void loop()
{
  if (!idChecked)
  {
    TPI::checkID();
    idChecked = true;
    TPI::finish();
  }

  // when ready, send ready signal '.' and wait
  Serial.println(F("Ready.."));
  while (Serial.available() < 1)
  {
  }

  TPI::start();

  // the first byte is a command
  //** 'P' = program the ATtiny using the read program
  //** 'D' = dump memory to serial monitor
  //** 'E' = erase chip. erases current program memory.(done automatically by 'P')
  //** 'S' = set fuse
  //** 'C' = clear fuse
  //** 'L' = Set Lock Bits

  char comnd = Sread();

  switch (comnd)
  {
  case 'r':
  case 'R':
    BUFFER_ON;
    RESET_HIGH
    _delay_ms(1);
    RESET_LOW;
    _delay_ms(10);
    RESET_HIGH;
    BUFFER_OFF;
    break;

  case 'D':
    TPI::dumpMemory();
    break;

    // case 'H':
    //   HVP = !HVP;
    //   hvserial();
    //   break;

    // case 'T':
    //   HVON = !HVON;
    //   hvserial();
    //   break;

    // case 'P':
    //   if (!TPI::writeProgram())
    //   {
    //     startTime = millis();
    //     while (millis() - startTime < 8000)
    //       Serial.read(); // if exited due to error, disregard all other serial data
    //   }
    //   break;

  case 'E':
    TPI::eraseChip();
    break;

  case 'S':
    TPI::setConfig(true);
    break;

  case 'C':
    TPI::setConfig(false);
    break;

  case 'L':
    TPI::setLockBits();
    break;

  default:
    Serial.println(F("Received unknown command"));
  }

  TPI::finish();
}

void ERROR_pgmSize(void)
{
  Serial.println(F("program size is 0??"));
}

void ERROR_data(char i)
{
  Serial.println(F("couldn't receive data:"));
  switch (i)
  {
  case TimeOut:
    Serial.println(F("timed out"));
    break;
  case HexError:
    Serial.println(F("hex file format error"));
    break;
  case TooLarge:
    Serial.println(F("program is too large"));
    break;

  default:
    break;
  }
}

// converts two chars to one byte
// c1 is MS, c2 is LS
uint8_t byteval(char c1, char c2)
{
  uint8_t by;
  if (c1 <= '9')
  {
    by = c1 - '0';
  }
  else
  {
    by = c1 - 'A' + 10;
  }
  by = by << 4;
  if (c2 <= '9')
  {
    by += c2 - '0';
  }
  else
  {
    by += c2 - 'A' + 10;
  }
  return by;
}

char Sread(void)
{
  while (Serial.available() < 1)
  {
  }
  return Serial.read();
}
