#ifndef _ISP_H_
#define _ISP_H_

#include "stk500.h"

#define PROG_FLICKER true
#define PULSE_TIME 10
#define EECHUNK (32)

// Configure SPI clock (in Hz).
// E.g. for an ATtiny @ 128 kHz: the datasheet states that both the high and low
// SPI clock pulse must be > 2 CPU cycles, so take 3 cycles i.e. divide target
// f_cpu by 6:
//     #define SPI_CLOCK            (128000/6)
//
// A clock slow enough for an ATtiny85 @ 1 MHz, is a reasonable default:
#define SPI_CLOCK (1000000 / 6)

class ISP : public STK500
{
public:
    ISP() : STK500(){};
    ISP(SoftwareSerial *serial) : STK500(serial) {}

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
    void commit(unsigned int addr);
    uint8_t flash_read(uint8_t hilo, unsigned int addr);
    void flash(uint8_t hilo, unsigned int addr, uint8_t data);
    void write_flash_pages(int length);
    void write_eeprom_chunk(unsigned int start, unsigned int length);
};

#endif