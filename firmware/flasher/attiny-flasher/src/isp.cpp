#include <Arduino.h>
#include <SPI.h>
#include "isp.h"

void ISP::init()
{
    // sserial->println("hvsp::init");
    BUFFER_OFF;
    RESET_INIT;
    RESET_Z;
    SETUP_PINS_ISP;
    reset_locked = false;
}

void ISP::start_programming()
{
    reset_locked = true;
    // Reset target before driving PIN_SCK or PIN_MOSI
    BUFFER_ON;
    // SPI.begin() will configure SS as output, so SPI master mode is selected.
    // We have defined RESET as pin 10, which for many Arduinos is not the SS pin.
    // So we have to configure RESET as output addr,
    // (reset_target() first sets the correct level)
    reset_target(true);

    SPI.begin();
    SPI.beginTransaction(SPISettings(SPI_CLOCK, MSBFIRST, SPI_MODE0));

    // See AVR datasheets, chapter "SERIAL_PRG Programming Algorithm":

    // Pulse RESET after PIN_SCK is low:
    digitalWrite(PIN_SCK, LOW);
    _delay_ms(20); // discharge PIN_SCK, value arbitrarily chosen
    reset_target(false);
    // Pulse must be minimum 2 target CPU clock cycles so 100 usec is ok for CPU
    // speeds above 20 KHz
    _delay_us(100);
    reset_target(true);

    // Send the enable programming command:
    _delay_ms(50); // datasheet: must be > 20 msec
    spi_transaction(0xAC, 0x53, 0x00, 0x00);
    programming = true;
}

void ISP::end_programming()
{
    SPI.end();
    // We're about to take the target out of reset so configure SPI pins as input
    // pinMode(PIN_MOSI, INPUT);
    // pinMode(PIN_SCK, INPUT);
    reset_target(false);
    RESET_Z;
    BUFFER_OFF;
    programming = false;
    reset_locked = false;
}

void ISP::reset_target(bool reset)
{
    if (reset && rst_active_high)
    {
        RESET_HIGH;
    }
    else if (!reset && !rst_active_high)
    {
        RESET_HIGH;
    }
    else
    {
        RESET_LOW;
    }
}

uint8_t ISP::spi_transaction(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    SPI_LOG(a);
    SPI_LOG(b);
    SPI_LOG(c);
    SPI_LOG(d);

    SPI.transfer(a);
    SPI.transfer(b);
    SPI.transfer(c);
    uint8_t r = SPI.transfer(d);
    SPI_LOG(r);
    return r;
}

void ISP::universal()
{
    uint8_t ch;

    fill(4);
    ch = spi_transaction(buff[0], buff[1], buff[2], buff[3]);
    breply(ch);
}

void ISP::flash(uint8_t hilo, unsigned int addr, uint8_t data)
{
    spi_transaction(0x40 + 8 * hilo,
                    addr >> 8 & 0xFF,
                    addr & 0xFF,
                    data);
}

void ISP::commit(unsigned int addr)
{
    spi_transaction(0x4C, (addr >> 8) & 0xFF, addr & 0xFF, 0);
}

void ISP::write_flash_pages(int length)
{
    int x = 0;
    unsigned int page = current_page();
    while (x < length)
    {
        if (page != current_page())
        {
            commit(page);
            page = current_page();
        }
        flash(0x0, addr, buff[x++]);
        flash(0x1, addr, buff[x++]);
        addr++;
    }

    commit(page);
}

// write (length) bytes, (start) is a byte address
void ISP::write_eeprom_chunk(unsigned int start, unsigned int length)
{
    // this writes byte-by-byte, page writing may be faster (4 bytes at a time)
    fill(length);
    for (unsigned int x = 0; x < length; x++)
    {
        unsigned int addr = start + x;
        spi_transaction(0xC0, (addr >> 8) & 0xFF, addr & 0xFF, buff[x]);
        _delay_ms(45);
    }
}

void ISP::program_page()
{
    uint16_t length = (getch() << 8) | getch();
    char memtype = getch();
    switch (memtype)
    {
    case 'F':
        fill(length);
        if (CRC_EOP == getch())
        {
            SERIAL_OUTC((char)STK_INSYNC);
            write_flash_pages(length);
            SERIAL_OUTC((char)STK_OK);
        }
        else
        {
            error++;
            SERIAL_OUTC((char)STK_NOSYNC);
        }
        break;

    case 'E':
        // addr is a word address, get the byte address
        uint16_t start = addr * 2;
        uint16_t remaining = length;
        if (length > param.eepromsize)
        {
            error++;
            SERIAL_OUTC((char)STK_FAILED);
            return;
        }

        while (remaining > EECHUNK)
        {
            write_eeprom_chunk(start, EECHUNK);
            start += EECHUNK;
            remaining -= EECHUNK;
        }
        write_eeprom_chunk(start, remaining);
        SERIAL_OUTC((char)STK_OK);

        if (CRC_EOP == getch())
        {
            SERIAL_OUTC((char)STK_INSYNC);
            SERIAL_OUTC((char)STK_OK);
        }
        else
        {
            error++;
            SERIAL_OUTC((char)STK_NOSYNC);
        }
        break;

    default:
        SERIAL_OUTC((char)STK_FAILED);
        break;
    }
}

uint8_t ISP::flash_read(uint8_t hilo, unsigned int addr)
{
    return spi_transaction(0x20 + hilo * 8,
                           (addr >> 8) & 0xFF,
                           addr & 0xFF,
                           0);
}

void ISP::read_page()
{
    int16_t length = (getch() << 8) | getch();
    char memtype = getch();
    if (CRC_EOP != getch())
    {
        error++;
        SERIAL_OUTC((char)STK_NOSYNC);
        return;
    }
    SERIAL_OUTC((char)STK_INSYNC);

    switch (memtype)
    {
    case 'F':
        for (int x = 0; x < length; x += 2)
        {
            uint8_t low = flash_read(0x0, addr);
            SERIAL_OUTC((char)low);
            uint8_t high = flash_read(0x1, addr);
            SERIAL_OUTC((char)high);
            addr++;
        }
        SERIAL_OUTC((char)STK_OK);
        break;

    case 'E':
        int start = addr * 2;
        for (int x = 0; x < length; x++)
        {
            int addr = start + x;
            uint8_t ee = spi_transaction(0xA0, (addr >> 8) & 0xFF, addr & 0xFF, 0xFF);
            SERIAL_OUTC((char)ee);
        }
        SERIAL_OUTC((char)STK_OK);
        break;

    default:
        SERIAL_OUTC((char)STK_FAILED);
        break;
    }
}

void ISP::read_signature()
{
    if (CRC_EOP != getch())
    {
        error++;
        SERIAL_OUTC((char)STK_NOSYNC);
        return;
    }
    SERIAL_OUTC((char)STK_INSYNC);
    uint8_t high = spi_transaction(0x30, 0x00, 0x00, 0x00);
    SERIAL_OUTC((char)high);
    uint8_t middle = spi_transaction(0x30, 0x00, 0x01, 0x00);
    SERIAL_OUTC((char)middle);
    uint8_t low = spi_transaction(0x30, 0x00, 0x02, 0x00);
    SERIAL_OUTC((char)low);
    SERIAL_OUTC((char)STK_OK);
}