#include <Arduino.h>
#include <SPI.h>
#include "hvsp.h"

void HVSP::init()
{
    // sserial->println("hvsp::init");
    BUFFER_OFF;
    RESET_INIT;
    RESET_Z;
    SETUP_PINS_HVSP;
    reset_locked = false;
}

void HVSP::start_programming()
{
    reset_locked = true;

    RESET_HIGH_12;
    _delay_ms(25);
    RESET_Z;
    
    BUFFER_HV_PROG;
    SETUP_PINS_HVSP_INIT;
    RESET_LOW;
    _delay_us(20);
    RESET_HIGH_12;
    _delay_us(20);
    BUFFER_OFF;
    SETUP_PINS_HVSP;
    _delay_us(300);

    BUFFER_HV_PROG;
    programming = true;
}

void HVSP::end_programming()
{
    RESET_Z;
    BUFFER_OFF;
    programming = false;
    reset_locked = false;
}

void HVSP::universal()
{
    uint16_t cmd = (getch() << 8) | getch();
    uint8_t idx = getch();
    uint8_t data = getch();
    if (CRC_EOP == getch())
    {
        SERIAL_OUTC((char)STK_INSYNC);
        switch (cmd)
        {
        case 0x3000: // prefered by stk500v1 programmer
            data = read_signature_byte(idx);
            break;
        case 0x5000:
            data = read_low_fuse();
            break;
        case 0x5008:
            data = read_ext_fuse();
            break;
        case 0x5800:
            data = read_lock_bits();
            break;
        case 0x5808:
            data = read_high_fuse();
            break;
        case 0xAC80:
            chip_erase();
            break;
        case 0xACA0:
            write_low_fuse(data);
            break;
        case 0xACA4:
            write_ext_fuse(data);
            break;
        case 0xACA8:
            write_high_fuse(data);
            break;
        case 0xACE0:
            write_lock_bits(data);
            break;
        }
        SERIAL_OUTC((char)data);
        SERIAL_OUTC((char)STK_OK);
    }
    else
    {
        error++;
        SERIAL_OUTC((char)STK_NOSYNC);
    }
}

void HVSP::write_flash_pages(int len)
{
    write_flash_mode();
    int x = 0;
    uint16_t page = current_page();
    while (x < len)
    {
        if (page != current_page())
        {
            write_flash_page();
            page = current_page();
        }
        load_flash_page_low_byte(buff[x++]);
        load_flash_page_high_byte(buff[x++]);
        // flash(0x0, addr, buff[x++]);
        // flash(0x1, addr, buff[x++]);
        addr++;
    }

    write_flash_page();

    // uint16_t pageMask = (param.pagesize >> 1) - 1;
    // while (len)
    // {
    //     SERIAL_OUT("A", addr);
    //     load_flash_page_low_byte(getch());
    //     load_flash_page_high_byte(getch());
        
    //     addr++;
    //     if (!(addr & pageMask))
    //         write_flash_page();
    //     len -= 2;
    // }

    // if (addr & pageMask)
    //     write_flash_page();
}

void HVSP::program_page()
{
    uint16_t len = (getch() << 8) | getch();
    uint8_t memType = getch();
    uint16_t pageMask = 0;
    switch (memType)
    {
    case 'F':
        fill(len);
        if (CRC_EOP == getch())
        {
            SERIAL_OUTC((char)STK_INSYNC);
            write_flash_pages(len);
            SERIAL_OUTC((char)STK_OK);
        }
        else
        {
            error++;
            SERIAL_OUTC((char)STK_NOSYNC);
        }
        break;

    case 'E':
        write_eeprom_mode();
        pageMask = param.eeprom_page_size - 1;
        addr <<= 1;
        while (len--)
        {
            load_eeprom_page(getch());
            addr++;
            if (!(addr & (pageMask)))
                write_eeprom_page();
        }
        if (addr & (pageMask))
            write_eeprom_page();
        addr >>= 1;

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

void HVSP::read_page()
{
    int16_t length = (getch() << 8) | getch();
    uint8_t memType = getch();

    if (CRC_EOP != getch())
    {
        error++;
        SERIAL_OUTC((char)STK_NOSYNC);
        return;
    }
    SERIAL_OUTC((char)STK_INSYNC);

    switch (memType)
    {
    case 'F':
        read_flash_mode();
        while (length--)
        {
            uint8_t low = read_flash_low_byte();
            SERIAL_OUTC((char)low);
            if (length)
            {
                uint8_t high = read_flash_high_byte();
                SERIAL_OUTC((char)high);
                --length;
            }
            ++addr;
        }
        SERIAL_OUTC((char)STK_OK);
        break;

    case 'E':
        read_eeprom_mode();
        addr <<= 1;
        while (length--)
        {
            uint8_t ee = read_eeprom_byte();
            SERIAL_OUTC((char)ee);
            ++addr;
        }
        addr >>= 1;
        SERIAL_OUTC((char)STK_OK);
        break;

    default:
        SERIAL_OUTC((char)STK_FAILED);
        break;
    }
}

void HVSP::read_signature()
{
    if (CRC_EOP != getch())
    {
        error++;
        SERIAL_OUTC((char)STK_NOSYNC);
        return;
    }
    SERIAL_OUTC((char)STK_INSYNC);
    uint8_t high = read_signature_byte(0);
    SERIAL_OUTC((char)high);
    uint8_t middle = read_signature_byte(1);
    SERIAL_OUTC((char)middle);
    uint8_t low = read_signature_byte(2);
    SERIAL_OUTC((char)low);
    SERIAL_OUTC((char)STK_OK);
}

uint8_t HVSP::read_signature_byte(uint8_t idx)
{
    transfer(0x08, 0x4C);
    transfer(idx, 0x0C);
    transfer(0x00, 0x68);
    return transfer(0x00, 0x6c);
}

uint8_t HVSP::transfer(uint8_t data, uint8_t cmd)
{
    uint8_t result = 0;
    // wait chip ready
    while (!HVSP_SDO_STATE)
        ;
    // start bits
    HVSP_SDI_LOW;
    HVSP_SII_LOW;
    clock_pulse();

    // 8-bit shifter
    for (uint8_t mask = 128; mask != 0; mask >>= 1)
    {
        HVSP_SDI_LOW;
        if (data & mask)
            HVSP_SDI_HIGH;
        HVSP_SII_LOW;
        if (cmd & mask)
            HVSP_SII_HIGH;
        if (HVSP_SDO_STATE)
            result |= mask;
        clock_pulse();
    }

    // double stop bits
    HVSP_SDI_LOW;
    HVSP_SII_LOW;
    clock_pulse();
    clock_pulse();
    HVSP_LOG(data, cmd, result)
    return result;
}

// uint8_t HVSP::transfer(uint8_t val1, uint8_t val2)
// {
//     int inBits = 0;
//     while (!digitalRead(PIN_SDO))
//         ;
//     unsigned int dout = (unsigned int)val1 << 2;
//     unsigned int iout = (unsigned int)val2 << 2;
//     for (int ii = 10; ii >= 0; ii--)
//     {
//         digitalWrite(PIN_SDI, !!(dout & (1 << ii)));
//         digitalWrite(PIN_SII, !!(iout & (1 << ii)));
//         inBits <<= 1;
//         inBits |= digitalRead(PIN_SDO);
//         digitalWrite(PIN_SCI, HIGH);
//         digitalWrite(PIN_SCI, LOW);
//     }
//     uint8_t res = inBits >> 2;
//     HVSP_LOG(val1, val2, res)
//     return res;
// }

void HVSP::clock_pulse()
{
    _delay_us(8);
    HVSP_SCI_HIGH;
    _delay_us(2);
    HVSP_SCI_LOW;
}

uint8_t HVSP::read_low_fuse()
{
    transfer(0x04, 0x4C);
    transfer(0x00, 0x68);
    return transfer(0x00, 0x6C);
}

void HVSP::write_low_fuse(uint8_t fuse)
{
    transfer(0x40, 0x4C);
    transfer(fuse, 0x2C);
    transfer(0x00, 0x64);
    transfer(0x00, 0x6C);
}

uint8_t HVSP::read_high_fuse()
{
    transfer(0x04, 0x4C);
    transfer(0x00, 0x7A);
    return transfer(0x00, 0x7E);
}

void HVSP::write_high_fuse(uint8_t fuse)
{
    transfer(0x40, 0x4C);
    transfer(fuse, 0x2C);
    transfer(0x00, 0x74);
    transfer(0x00, 0x7C);
}

uint8_t HVSP::read_ext_fuse()
{
    transfer(0x04, 0x4C);
    transfer(0x00, 0x6A);
    return transfer(0x00, 0x6E);
}

void HVSP::write_ext_fuse(uint8_t fuse)
{
    transfer(0x40, 0x4C);
    transfer(fuse, 0x2C);
    transfer(0x00, 0x66);
    transfer(0x00, 0x6e);
}

uint8_t HVSP::read_lock_bits()
{
    transfer(0x04, 0x4C);
    transfer(0x00, 0x78);
    return transfer(0x00, 0x7C);
}

void HVSP::write_lock_bits(uint8_t fuse)
{
    transfer(0x20, 0x4C);
    transfer(fuse, 0x2C);
    transfer(0x00, 0x64);
    transfer(0x00, 0x6c);
}

void HVSP::chip_erase()
{
    transfer(0x80, 0x4C);
    transfer(0x00, 0x64);
    transfer(0x00, 0x6C);
}

void HVSP::write_flash_mode()
{
    transfer(0x10, 0x4C);
}

void HVSP::read_flash_mode()
{
    transfer(0x02, 0x4C);
}

void HVSP::write_eeprom_mode()
{
    transfer(0x11, 0x4C);
}

void HVSP::read_eeprom_mode()
{
    transfer(0x03, 0x4C);
}

void HVSP::load_flash_page_low_byte(uint8_t b)
{
    flash_page = addr >> 8;
    transfer(addr & 0xFF, 0x0C);
    transfer(b, 0x2C);
}

void HVSP::load_flash_page_high_byte(uint8_t b)
{
    transfer(b, 0x3C);
    transfer(0x00, 0x7D);
    transfer(0x00, 0x7C);
}

uint8_t HVSP::read_flash_low_byte()
{
    transfer(addr, 0x0C);
    transfer(addr >> 8, 0x1C);
    transfer(0x00, 0x68);
    return transfer(0x00, 0x6C);
}

uint8_t HVSP::read_flash_high_byte()
{
    transfer(0x00, 0x6C);
    transfer(0x00, 0x78);
    uint8_t result = transfer(0x00, 0x7C);
    transfer(0x00, 0x7C);
    return result;
}

void HVSP::write_flash_page()
{
    SERIAL_OUT("FP", flash_page);
    transfer(flash_page, 0x1C);
    transfer(0x00, 0x64);
    transfer(0x00, 0x6C);
}

void HVSP::load_eeprom_page(uint8_t b)
{
    transfer(addr, 0x0C);
    transfer(addr >> 8, 0x1C);
    transfer(b, 0x2C);
    transfer(0x00, 0x6D);
    transfer(0x00, 0x6C);
}

void HVSP::write_eeprom_page()
{
    transfer(0x00, 0x64);
    transfer(0x00, 0x6C);
}

uint8_t HVSP::read_eeprom_byte()
{
    return read_flash_low_byte();
}