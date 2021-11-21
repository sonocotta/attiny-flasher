#include "stk500.h"
#include "debug.h"

STK500::STK500(SoftwareSerial *serial)
{
    sserial = serial;
}

void STK500::avrisp()
{
    uint8_t ch = getch();
    switch (ch)
    {
    case CMND_STK_GET_SYNC:
        error = 0;
        serial_busy = true;
        empty_reply();
        break;

    case CMND_STK_GET_SIGN_ON:
        if (getch() == CRC_EOP)
        {
            SSERIAL_DEINIT;
            SERIAL_OUT1("HELLO");
            SERIAL_OUTC((char)STK_INSYNC);
            SERIAL_OUTS(F("AVR ISP"));
            SERIAL_OUTC((char)STK_OK);
        }
        else
        {
            error++;
            SERIAL_OUTC((char)STK_NOSYNC);
        }
        break;

    case CMND_STK_GET_PARAMETER:
        {
            char c = getch();
            SERIAL_OUT("GET_PAR", c);
            get_version(c);
        }
        break;

    case CMND_STK_SET_DEVICE:
        SERIAL_OUT1("SET_PAR");
        fill(20);
        set_parameters();
        empty_reply();
        break;

    case CMND_STK_SET_DEVICE_EXT:
        SERIAL_OUT1("SET_PAR_EX");
        fill(getch() - 1);
        set_ext_parameters();
        empty_reply();
        break;

    case CMND_STK_ENTER_PROGMODE:
        SERIAL_OUT1("PMODE");
        if (!programming)
            start_programming();
        empty_reply();
        break;

    case CMND_STK_LEAVE_PROGMODE:
        SERIAL_OUT1("QUIT");
        error = 0;
        end_programming();
        empty_reply();
        serial_busy = false;
        break;

    case CMND_STK_LOAD_ADDRESS:
        addr = getch();
        addr += getch() << 8;
        SERIAL_OUT("SET_ADDR", addr);
        empty_reply();
        break;

    case CMND_STK_PROG_FLASH:
        SERIAL_OUT1("PROG_FLASH");
        getch(); // low addr
        getch(); // high addr
        empty_reply();
        break;

    case CMND_STK_PROG_DATA:
        SERIAL_OUT1("PROG_DATA");
        getch(); // data
        empty_reply();
        break;

    case CMND_STK_PROG_PAGE:
        program_page();
        break;

    case CMND_STK_READ_PAGE:
        read_page();
        break;

    case CMND_STK_UNIVERSAL:
        SERIAL_OUT1("UNIV");
        universal();
        break;

    case CMND_STK_READ_SIGN:
        read_signature();
        break;

    // expecting a command, not CRC_EOP
    // this is how we can get back in sync
    case CRC_EOP:
        error++;
        SERIAL_OUTC((char)STK_NOSYNC);
        break;

    // anything else we will return STK_UNKNOWN
    default:
        error++;
        if (CRC_EOP == getch())
        {
            SERIAL_OUTC((char)STK_UNKNOWN);
        }
        else
        {
            SERIAL_OUTC((char)STK_NOSYNC);
        }
    }
}

uint8_t STK500::getch()
{
    while (!Serial.available())
        ;
    uint8_t c = Serial.read();
    LOG_CMD_IN(c);
    return c;
}

void STK500::breply(char b)
{
    if (CRC_EOP == getch())
    {
        SERIAL_OUTC((char)STK_INSYNC);
        SERIAL_OUTC((char)b);
        SERIAL_OUTC((char)STK_OK);
    }
    else
    {
        error++;
        SERIAL_OUTC((char)STK_NOSYNC);
    }
}

void STK500::empty_reply()
{
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
}

void STK500::get_version(uint8_t c)
{
    switch (c)
    {
    case 0x80:
        breply(HWVER);
        break;
    case 0x81:
        breply(SWMAJ);
        break;
    case 0x82:
        breply(SWMIN);
        break;
    case 0x93:
        breply('S'); // serial programmer
        break;
    default:
        breply(0);
    }
}

uint16_t STK500::current_page()
{
    if (param.pagesize == 32)
    {
        return addr & 0xFFFFFFF0;
    }
    if (param.pagesize == 64)
    {
        return addr & 0xFFFFFFE0;
    }
    if (param.pagesize == 128)
    {
        return addr & 0xFFFFFFC0;
    }
    if (param.pagesize == 256)
    {
        return addr & 0xFFFFFF80;
    }
    return addr;
}

void STK500::fill(int n)
{
    for (int x = 0; x < n; x++)
        buff[x] = getch();
}

#define beget16(addr) (*addr * 256 + *(addr + 1))

void STK500::set_parameters()
{
    // call this after reading parameter packet into buff[]
    param.devicecode = buff[0];
    param.revision = buff[1];
    param.progtype = buff[2];
    param.parmode = buff[3];
    param.polling = buff[4];
    param.selftimed = buff[5];
    param.lockbytes = buff[6];
    param.fusebytes = buff[7];
    param.flashpoll = buff[8];
    // ignore buff[9] (= buff[8])
    // following are 16 bits (big endian)
    param.eeprompoll = beget16(&buff[10]);
    param.pagesize = beget16(&buff[12]);
    param.eepromsize = beget16(&buff[14]);

    // 32 bits flashsize (big endian)
    param.flashsize = buff[16] * 0x01000000 + buff[17] * 0x00010000 + buff[18] * 0x00000100 + buff[19];

    // AVR devices have active low reset, AT89Sx are active high
    rst_active_high = (param.devicecode >= 0xe0);
}

void STK500::set_ext_parameters()
{
    param.eeprom_page_size = buff[0];
}