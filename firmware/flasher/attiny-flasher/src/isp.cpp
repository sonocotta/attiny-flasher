#include <Arduino.h>
#include <SPI.h>
#include "isp.h"

ISP::ISP(SoftwareSerial *serial) : STK500(serial) {}

void ISP::init()
{
    pinMode(PIN_BUFEN, OUTPUT);
    BUFFER_OFF;
    RESET_INIT;
}

void ISP::start_programming()
{
    // Reset target before driving PIN_SCK or PIN_MOSI
    BUFFER_ON;
    // SPI.begin() will configure SS as output, so SPI master mode is selected.
    // We have defined RESET as pin 10, which for many Arduinos is not the SS pin.
    // So we have to configure RESET as output here,
    // (reset_target() first sets the correct level)
    reset_target(true);
    /// pinMode(RESET, OUTPUT);
    SPI.begin();
    SPI.beginTransaction(SPISettings(SPI_CLOCK, MSBFIRST, SPI_MODE0));

    // See AVR datasheets, chapter "SERIAL_PRG Programming Algorithm":

    // Pulse RESET after PIN_SCK is low:
    digitalWrite(PIN_SCK, LOW);
    delay(20); // discharge PIN_SCK, value arbitrarily chosen
    reset_target(false);
    // Pulse must be minimum 2 target CPU clock cycles so 100 usec is ok for CPU
    // speeds above 20 KHz
    delayMicroseconds(100);
    reset_target(true);

    // Send the enable programming command:
    delay(50); // datasheet: must be > 20 msec
    spi_transaction(0xAC, 0x53, 0x00, 0x00);
    programming = true;
}

void ISP::end_programming()
{
    SPI.end();
    // We're about to take the target out of reset so configure SPI pins as input
    pinMode(PIN_MOSI, INPUT);
    pinMode(PIN_SCK, INPUT);
    reset_target(false);
    RESET_Z;
    programming = false;
    BUFFER_OFF;
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
    return SPI.transfer(d);
}

void ISP::prog_lamp(int state)
{
    // TODO: restore
    // if (PROG_FLICKER)
    // {
    //     digitalWrite(LED_PMODE, state);
    // }
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
    // TODO: restore
    if (PROG_FLICKER)
    {
        //     prog_lamp(LOW);
    }
    spi_transaction(0x4C, (addr >> 8) & 0xFF, addr & 0xFF, 0);
    // TODO: restore
    if (PROG_FLICKER)
    {
        _delay_ms(PULSE_TIME);
        //     prog_lamp(HIGH);
    }
}

unsigned int ISP::current_page()
{
    if (param.pagesize == 32)
    {
        return here & 0xFFFFFFF0;
    }
    if (param.pagesize == 64)
    {
        return here & 0xFFFFFFE0;
    }
    if (param.pagesize == 128)
    {
        return here & 0xFFFFFFC0;
    }
    if (param.pagesize == 256)
    {
        return here & 0xFFFFFF80;
    }
    return here;
}

void ISP::write_flash(int length)
{
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
        flash(0x0, here, buff[x++]);
        flash(0x1, here, buff[x++]);
        here++;
    }

    commit(page);
}

#define EECHUNK (32)
void ISP::write_eeprom(unsigned int length)
{
    // here is a word address, get the byte address
    unsigned int start = here * 2;
    unsigned int remaining = length;
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
}

// write (length) bytes, (start) is a byte address
void ISP::write_eeprom_chunk(unsigned int start, unsigned int length)
{
    // this writes byte-by-byte, page writing may be faster (4 bytes at a time)
    fill(length);
    prog_lamp(LOW);
    for (unsigned int x = 0; x < length; x++)
    {
        unsigned int addr = start + x;
        spi_transaction(0xC0, (addr >> 8) & 0xFF, addr & 0xFF, buff[x]);
        delay(45);
    }
    prog_lamp(HIGH);
}

void ISP::program_page()
{
    unsigned int length = 256 * getch();
    length += getch();
    char memtype = getch();
    switch (memtype)
    {
    case 'F':
        write_flash(length);
        break;

    case 'E':
        write_eeprom(length);
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

void ISP::flash_read_page(int length)
{
    for (int x = 0; x < length; x += 2)
    {
        uint8_t low = flash_read(0x0, here);
        SERIAL_OUTC((char)low);
        uint8_t high = flash_read(0x1, here);
        SERIAL_OUTC((char)high);
        here++;
    }
}

void ISP::eeprom_read_page(int length)
{
    // here again we have a word address
    int start = here * 2;
    for (int x = 0; x < length; x++)
    {
        int addr = start + x;
        uint8_t ee = spi_transaction(0xA0, (addr >> 8) & 0xFF, addr & 0xFF, 0xFF);
        SERIAL_OUTC((char)ee);
    }
}

void ISP::read_page()
{
    int length = 256 * getch();
    length += getch();
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
        flash_read_page(length);
        SERIAL_OUTC((char)STK_OK);
        break;

    case 'E':
        eeprom_read_page(length);
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