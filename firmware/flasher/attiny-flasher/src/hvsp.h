#ifndef _HVSP_H_
#define _HVSP_H_

#include "stk500.h"
#include "boards.h"

#define PULSE_TIME 10

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

class HVSP : public STK500
{
public:
    HVSP() : STK500(){};
    HVSP(SoftwareSerial *serial) : STK500(serial){};

    void init();

protected:
    void start_programming();
    void end_programming();
    void program_page();
    void read_page();
    void read_signature();
    void universal();

private:
    uint8_t read_signature_byte(uint8_t idx);
    uint8_t transfer(uint8_t data, uint8_t cmd);
    void clock_pulse();

    uint8_t read_low_fuse();
    void write_low_fuse(uint8_t fuse);
    uint8_t read_high_fuse();
    void write_high_fuse(uint8_t fuse);
    uint8_t read_ext_fuse();
    void write_ext_fuse(uint8_t fuse);
    uint8_t read_lock_bits();
    void write_lock_bits(uint8_t fuse);
    void chip_erase();
    void read_flash_mode();
    void read_eeprom_mode();
    void write_flash_mode();
    void write_flash_pages(int len);
    void write_eeprom_mode();
    void load_flash_page_low_byte(uint8_t b);
    void load_flash_page_high_byte(uint8_t b);
    uint8_t read_flash_high_byte();
    uint8_t read_flash_low_byte();
    void write_flash_page();
    void load_eeprom_page(uint8_t b);
    void write_eeprom_page();
    uint8_t read_eeprom_byte();
};

#endif