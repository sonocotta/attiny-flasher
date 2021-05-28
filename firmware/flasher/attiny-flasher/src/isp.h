#ifndef _ISP_H_
#define _ISP_H_

#include "stk500.h"

#define PROG_FLICKER true
#define PULSE_TIME 10

// Configure SPI clock (in Hz).
// E.g. for an ATtiny @ 128 kHz: the datasheet states that both the high and low
// SPI clock pulse must be > 2 CPU cycles, so take 3 cycles i.e. divide target
// f_cpu by 6:
//     #define SPI_CLOCK            (128000/6)
//
// A clock slow enough for an ATtiny85 @ 1 MHz, is a reasonable default:
#define SPI_CLOCK (1000000 >> 3)

#define RESET 10 // Use pin 10 to reset the target rather than SS
#if defined(REV_D_TWO_PIN_RESET)
#define RESET_PULL A2 // Need another pin to pull it down properly
#endif

#define PIN_BUFEN 6 // Enable output buffer, active HIGH on 74HC126D, active LOW on 74HC125D
#ifdef BUF_74HC126D
#define BUFFER_ON digitalWrite(PIN_BUFEN, HIGH);
#define BUFFER_OFF digitalWrite(PIN_BUFEN, LOW);
#endif
#ifdef BUF_74HC125D
#define BUFFER_ON digitalWrite(PIN_BUFEN, LOW);
#define BUFFER_OFF digitalWrite(PIN_BUFEN, HIGH);
#endif
#ifdef BUF_74HC241
#define BUFFER_HV_PROG          \
    pinMode(PIN_BUFEN, OUTPUT); \
    digitalWrite(PIN_BUFEN, LOW);
#define BUFFER_ON               \
    pinMode(PIN_BUFEN, OUTPUT); \
    digitalWrite(PIN_BUFEN, HIGH);
#define BUFFER_OFF pinMode(PIN_BUFEN, INPUT);
#endif

#if defined(REV_D_TWO_PIN_RESET)
#define RESET_INIT          \
    pinMode(RESET, OUTPUT); \
    pinMode(RESET_PULL, OUTPUT);
#define RESET_HIGH                 \
    digitalWrite(RESET_PULL, LOW); \
    digitalWrite(RESET, HIGH);
#define RESET_LOW             \
    digitalWrite(RESET, LOW); \
    digitalWrite(RESET_PULL, HIGH);
#define RESET_Z                    \
    digitalWrite(RESET_PULL, LOW); \
    digitalWrite(RESET, LOW);
#else
#define RESET_INIT ;
#define RESET_HIGH          \
    pinMode(RESET, OUTPUT); \
    digitalWrite(RESET, HIGH);
#define RESET_LOW           \
    pinMode(RESET, OUTPUT); \
    digitalWrite(RESET, LOW);
#define RESET_Z pinMode(RESET, INPUT); // high impedance state
#endif

// By default, use hardware SPI pins:
#ifndef PIN_MOSI
#define PIN_MOSI MOSI
#endif

#ifndef PIN_MISO
#define PIN_MISO MISO
#endif

#ifndef PIN_SCK
#define PIN_SCK SCK
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

class ISP : public STK500
{
public:
    ISP(SoftwareSerial * serial);

    void init();
   
protected:
    void start_programming();
    void end_programming();
    void program_page();
    void read_page();
    void read_signature();
    void universal();

private:   
    void reset_target(bool reset);
    uint8_t spi_transaction(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
    void prog_lamp(int state);
    void commit(unsigned int addr);
    uint8_t flash_read(uint8_t hilo, unsigned int addr);
    void write_flash(int length);
    void flash(uint8_t hilo, unsigned int addr, uint8_t data);
    void flash_read_page(int length);
    unsigned int current_page();
    void write_flash_pages(int length);
    void write_eeprom(unsigned int length);
    void write_eeprom_chunk(unsigned int start, unsigned int length);
    void eeprom_read_page(int length);
};

#endif