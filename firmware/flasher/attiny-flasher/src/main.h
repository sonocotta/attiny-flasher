#ifndef _MAIN_H
#define _MAIN_H

#define PROG_FLICKER true

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#ifdef OLED_ENABLE
  // Screen settings
  #define SCREEN_WIDTH     128 
  #define SCREEN_HEIGHT     64 
  #define SCREEN_MAX_ROWS    8
  #define SCREEN_MAX_COLS   21
  #define SCREEN_ROW_HEIGHT 16
  #define OLED_RESET        -1

  #ifdef OLED_LIB_ADAFRUIT
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  #endif

  #ifdef OLED_LIB_TINY
  #include <Tiny4kOLED.h>
  SSD1306Device display = oled;
  #endif

#endif

#if defined(OLED_ENABLE) || defined(SERAIL_BRIDGE_ENABLE)
#define SW_SERIAL_ENABLE
// Tiny-linked Serial pins
#define PIN_SERIAL_RX 4
#define PIN_SERIAL_TX 5
#include <SoftwareSerial.h>
SoftwareSerial SSerial(PIN_SERIAL_RX, PIN_SERIAL_TX);
#define BAUDRATE_OUT	115200 //19200
#undef SERIAL
#endif

// Configure SPI clock (in Hz).
// E.g. for an ATtiny @ 128 kHz: the datasheet states that both the high and low
// SPI clock pulse must be > 2 CPU cycles, so take 3 cycles i.e. divide target
// f_cpu by 6:
//     #define SPI_CLOCK            (128000/6)
//
// A clock slow enough for an ATtiny85 @ 1 MHz, is a reasonable default:

#define SPI_CLOCK 		(1000000/6)

#define RESET      10  // Use pin 10 to reset the target rather than SS
#if defined(REV_D_TWO_PIN_RESET)
#define RESET_PULL A2  // Need another pin to pull it down properly
#endif
#define LED_HB      9  // Heartbeat
#define LED_ERR     8  // Error
#define LED_PMODE   7  // Programm

#define PIN_BUFEN 6  // Enable output buffer, active HIGH on 74HC126D, active LOW on 74HC125D
#ifdef BUF_74HC126D
#define BUFFER_ON  digitalWrite(PIN_BUFEN, HIGH);
#define BUFFER_OFF digitalWrite(PIN_BUFEN, LOW);
#endif
#ifdef BUF_74HC125D
#define BUFFER_ON  digitalWrite(PIN_BUFEN, LOW);
#define BUFFER_OFF digitalWrite(PIN_BUFEN, HIGH);
#endif
#ifdef BUF_74HC241
#define BUFFER_HV_PROG pinMode(PIN_BUFEN, OUTPUT); digitalWrite(PIN_BUFEN, LOW);
#define BUFFER_ON pinMode(PIN_BUFEN, OUTPUT); digitalWrite(PIN_BUFEN, HIGH);
#define BUFFER_OFF pinMode(PIN_BUFEN, INPUT);
#endif

#if defined(REV_D_TWO_PIN_RESET)
#define RESET_INIT pinMode(RESET, OUTPUT); pinMode(RESET_PULL, OUTPUT); 
#define RESET_HIGH digitalWrite(RESET_PULL, LOW); digitalWrite(RESET, HIGH);
#define RESET_LOW  digitalWrite(RESET, LOW); digitalWrite(RESET_PULL, HIGH);
#define RESET_Z    digitalWrite(RESET_PULL, LOW); digitalWrite(RESET, LOW);
#else
#define RESET_INIT ;
#define RESET_HIGH pinMode(RESET, OUTPUT); digitalWrite(RESET, HIGH);
#define RESET_LOW  pinMode(RESET, OUTPUT); digitalWrite(RESET, LOW);
#define RESET_Z    pinMode(RESET, INPUT); // high impedance state
#endif

// By default, use hardware SPI pins:
#ifndef PIN_MOSI
#define PIN_MOSI 	MOSI
#endif

#ifndef PIN_MISO
#define PIN_MISO 	MISO
#endif

#ifndef PIN_SCK
#define PIN_SCK 	SCK
#endif

// Configure the serial port to use.
//
// Prefer the USB virtual serial port (aka. native USB port), if the Arduino has one:
//   - it does not autoreset (except for the magic baud rate of 1200).
//   - it is more reliable because of USB handshaking.
//
// Leonardo and similar have an USB virtual serial port: 'Serial'.
// Due and Zero have an USB virtual serial port: 'SerialUSB'.
//
// On the Due and Zero, 'Serial' can be used too, provided you disable autoreset.
// To use 'Serial': #define SERIAL Serial

#ifdef SERIAL_PORT_USBVIRTUAL
#define SERIAL SERIAL_PORT_USBVIRTUAL
#else
#define SERIAL Serial
#endif

#define HWVER 2
#define SWMAJ 1
#define SWMIN 18

// STK Definitions
#define STK_OK      0x10
#define STK_FAILED  0x11
#define STK_UNKNOWN 0x12
#define STK_INSYNC  0x14
#define STK_NOSYNC  0x15
#define CRC_EOP     0x20 //ok it is a space...

void avrisp();

void pulse(int pin, int times);

uint8_t write_flash_pages(int length);

uint8_t write_eeprom_chunk(unsigned int start, unsigned int length);

void serialToScreen();

void display_logo();
void check_logo();

#endif

typedef struct param {
  uint8_t devicecode;
  uint8_t revision;
  uint8_t progtype;
  uint8_t parmode;
  uint8_t polling;
  uint8_t selftimed;
  uint8_t lockbytes;
  uint8_t fusebytes;
  uint8_t flashpoll;
  uint16_t eeprompoll;
  uint16_t pagesize;
  uint16_t eepromsize;
  uint32_t flashsize;
}
parameter;

parameter param;