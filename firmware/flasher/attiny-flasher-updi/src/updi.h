#ifndef _UPDI_H_
#define _UPDI_H_

#include "stk500.h"
#include "boards.h"

#define PROG_FLICKER true
#define PULSE_TIME 10
#define EECHUNK (32)

class UPDI : public STK500
{
public:
    UPDI() : STK500(){};
    UPDI(SoftwareSerial *serial) : STK500(serial) {}

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