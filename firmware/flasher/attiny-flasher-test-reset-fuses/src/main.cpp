#include <Arduino.h>
#include "boards.h"

#if defined(SW_SERIAL_ENABLE)
#include <SoftwareSerial.h>
SoftwareSerial *sserial = new SoftwareSerial(PIN_SERIAL_RX, PIN_SERIAL_TX);
#endif

#include "debug.h"
#include "main.h"
#define SERIAL_SPEED 9600

void setup()
{
  Serial.begin(SERIAL_SPEED);

#ifdef SERIAL_DEBUG_ENABLE
  sserial->begin(BAUDRATE_OUT);
  sserial->listen();
  sserial->println(F("Ready..."));
#endif

  RESET_INIT;
  BUFFER_OFF;
  RESET_Z;
  SETUP_PINS_HVSP;

  Serial.println("Ready");
}

void loop()
{
  if (Serial.available() > 0)
  {
    Serial.read();
    Serial.println(F("starting..."));

    RESET_HIGH_12;
    _delay_ms(25);
    RESET_Z;

    BUFFER_HV_PROG;
    SETUP_PINS_HVSP_INIT;
    RESET_LOW;
    //digitalWrite(VCC, HIGH); // Vcc On
    _delay_us(20);
    //onOff = 1; // 12v On
    Serial.println(F("HV on"));
    RESET_HIGH_12;
    _delay_us(10);
    BUFFER_OFF;

    SETUP_PINS_HVSP;
    _delay_us(300);

    BUFFER_HV_PROG;

    Serial.println(F("Reading signature..."));

    uint16_t sig = readSignature();
    Serial.print("Signature is: ");
    Serial.println(sig, HEX);

    readFuses();
    if (sig == ATTINY13)
    {
      writeFuse(LFUSE, 0x6A);
      writeFuse(HFUSE, 0xFF);
    }
    else if (sig == ATTINY24 || sig == ATTINY44 || sig == ATTINY84 ||
             sig == ATTINY25 || sig == ATTINY45 || sig == ATTINY85)
    {
      writeFuse(LFUSE, 0xF1);
      writeFuse(HFUSE, 0xDF);
      writeFuse(EFUSE, 0xFF);
    }

    readFuses();
    
    digitalWrite(PIN_SCI, LOW);
    //digitalWrite(VCC, LOW); // Vcc Off
    //onOff = 0;              // 12v Off
    BUFFER_OFF;
    RESET_Z;
  }
}

byte shiftOut(byte val1, byte val2)
{
  int inBits = 0;
  //Wait until SDO goes high
  while (!digitalRead(PIN_SDO))
    ;
  unsigned int dout = (unsigned int)val1 << 2;
  unsigned int iout = (unsigned int)val2 << 2;
  for (int ii = 10; ii >= 0; ii--)
  {
    digitalWrite(PIN_SDI, !!(dout & (1 << ii)));
    digitalWrite(PIN_SII, !!(iout & (1 << ii)));
    inBits <<= 1;
    inBits |= digitalRead(PIN_SDO);
    digitalWrite(PIN_SCI, HIGH);
    digitalWrite(PIN_SCI, LOW);
  }
  uint8_t res = inBits >> 2;
  HVSP_LOG(val1, val2, res)
  return res;
}

void writeFuse(unsigned int fuse, byte val)
{
  shiftOut(0x40, 0x4C);
  shiftOut(val, 0x2C);
  shiftOut(0x00, (byte)(fuse >> 8));
  shiftOut(0x00, (byte)fuse);
}

void readFuses()
{
  byte val;
  shiftOut(0x04, 0x4C); // LFuse
  shiftOut(0x00, 0x68);
  val = shiftOut(0x00, 0x6C);
  Serial.print("LFuse: ");
  Serial.print(val, HEX);
  shiftOut(0x04, 0x4C); // HFuse
  shiftOut(0x00, 0x7A);
  val = shiftOut(0x00, 0x7E);
  Serial.print(", HFuse: ");
  Serial.print(val, HEX);
  shiftOut(0x04, 0x4C); // EFuse
  shiftOut(0x00, 0x6A);
  val = shiftOut(0x00, 0x6E);
  Serial.print(", EFuse: ");
  Serial.println(val, HEX);
}

unsigned int readSignature()
{
  unsigned int sig = 0;
  byte val;
  for (int ii = 1; ii < 3; ii++)
  {
    shiftOut(0x08, 0x4C);
    shiftOut(ii, 0x0C);
    shiftOut(0x00, 0x68);
    val = shiftOut(0x00, 0x6C);
    sig = (sig << 8) + val;
  }
  return sig;
}