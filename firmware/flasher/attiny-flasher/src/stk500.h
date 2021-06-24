#ifndef _STK500_H_
#define _STK500_H_

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <SoftwareSerial.h>

// Command definitions
#define CMND_STK_GET_SYNC 0x30
#define CMND_STK_GET_SIGN_ON 0x31
#define CMND_STK_SET_PARAMETER 0x40 // NOT SUPPORTED
#define CMND_STK_GET_PARAMETER 0x41
#define CMND_STK_SET_DEVICE 0x42
#define CMND_STK_SET_DEVICE_EXT 0x45
#define CMND_STK_ENTER_PROGMODE 0x50
#define CMND_STK_LEAVE_PROGMODE 0x51
#define CMND_STK_CHIP_ERASE 0x52    // NOT SUPPORTED
#define CMND_STK_CHECK_AUTOINC 0x53 // NOT SUPPORTED
#define CMND_STK_LOAD_ADDRESS 0x55
#define CMND_STK_UNIVERSAL 0x56
#define CMND_STK_UNIVERSAL_MULTI 0x57 // NOT SUPPORTED
#define CMND_STK_PROG_FLASH 0x60      // IGNORED
#define CMND_STK_PROG_DATA 0x61       // IGNORED
#define CMND_STK_PROG_FUSE 0x62       // NOT SUPPORTED
#define CMND_STK_PROG_LOCK 0x63       // NOT SUPPORTED
#define CMND_STK_PROG_PAGE 0x64
#define CMND_STK_PROG_FUSE_EXT 0x65 // NOT SUPPORTED
#define CMND_STK_READ_FLASH 0x70    // NOT SUPPORTED
#define CMND_STK_READ_DATA 0x71     // NOT SUPPORTED
#define CMND_STK_READ_FUSE 0x72     // NOT SUPPORTED
#define CMND_STK_READ_LOCK 0x73     // NOT SUPPORTED
#define CMND_STK_READ_PAGE 0x74
#define CMND_STK_READ_SIGN 0x75       // NOT SUPPORTED
#define CMND_STK_READ_OSCCAL 0x76     // NOT SUPPORTED
#define CMND_STK_READ_FUSE_EXT 0x77   // NOT SUPPORTED
#define CMND_STK_READ_OSCCAL_EXT 0x78 // NOT SUPPORTED

// STK Definitions
#define STK_OK 0x10
#define STK_FAILED 0x11
#define STK_UNKNOWN 0x12
#define STK_INSYNC 0x14
#define STK_NOSYNC 0x15
#define CRC_EOP 0x20

// Software version
#define HWVER 2
#define SWMAJ 1
#define SWMIN 18

#define BUFFER_SIZE 128 // 256

#ifdef SERIAL_PORT_USBVIRTUAL
#define Serial SERIAL_PORT_USBVIRTUAL
#endif

typedef struct param
{
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
    uint8_t eeprom_page_size;
} parameter;

class STK500
{
public:
    STK500(){};
    STK500(SoftwareSerial *serial);

    bool serial_busy = false;
    bool programming = false;
    bool reset_locked = false;
    bool error = false;

    void avrisp();

protected:
    SoftwareSerial *sserial;
    parameter param;
    uint8_t flash_page = 0;
    bool rst_active_high = true;
    uint8_t buff[BUFFER_SIZE];
    uint16_t addr = 0;

    uint8_t getch();
    void breply(char b);
    void fill(int n);
    uint16_t current_page();

    virtual void start_programming();
    virtual void end_programming();
    virtual void program_page();
    virtual void read_page();
    virtual void read_signature();
    virtual void universal();

private:
    void empty_reply();
    void get_version(uint8_t c);
    void set_parameters();
    void set_ext_parameters();
};

#ifdef SERIAL_DEBUG_ENABLE
#define LOG_CMD_IN(x)             \
    {                             \
        sserial->print("< 0x");   \
        sserial->println(x, HEX); \
    }
#ifdef SERIAL_DEBUG_SPI_ENABLE
#define SPI_LOG(x)                \
    {                             \
        sserial->print("S ");     \
        sserial->print(addr);     \
        sserial->print(" 0x");    \
        sserial->println(x, HEX); \
    }
#define HVSP_LOG(x, y, z)         \
    {                             \
        sserial->print("H");      \
        sserial->print(" 0x");    \
        sserial->print(x, HEX);   \
        sserial->print(" 0x");    \
        sserial->print(y, HEX);   \
        sserial->print(" 0x");    \
        sserial->println(z, HEX); \
    }
#else
#define SPI_LOG(x) ;
#define HVSP_LOG(x, y, z) ;
#endif

#define SERIAL_OUTC(x)            \
    {                             \
        Serial.print(x);          \
        sserial->print("> 0x");   \
        sserial->println(x, HEX); \
    }
#define SERIAL_OUTS(x)        \
    {                         \
        Serial.print(x);      \
        sserial->print("> "); \
        sserial->println(x);  \
    }
#define SERIAL_OUT(s, x)     \
    {                        \
        sserial->print(s);   \
        sserial->print(' '); \
        sserial->println(x); \
    }
#else
#define LOG_CMD_IN(x)
#define SPI_LOG(x)
#define SERIAL_OUTC(x)   \
    {                    \
        Serial.print(x); \
    }
#define SERIAL_OUTS(x)   \
    {                    \
        Serial.print(x); \
    }
#define SERIAL_OUT(s, x) ;

#define SPI_LOG(x) ;
#define HVSP_LOG(x, y, z) ;
#endif

#endif