#ifndef _DEBUG_H_
#define _DEBUG_H_

#define _SSERIAL_INIT                                               \
    if (sserial == NULL)                                            \
        sserial = new SoftwareSerial(PIN_SERIAL_RX, PIN_SERIAL_TX); \
    sserial->begin(BAUDRATE_OUT);                                   \
    sserial->listen();

#define _SSERIAL_DEINIT                \
    if (sserial != NULL)               \
    {                                  \
        sserial->end();                \
        delete (sserial);              \
        sserial = NULL;                \
        pinMode(PIN_SERIAL_RX, INPUT); \
        pinMode(PIN_SERIAL_TX, INPUT); \
    }

#ifdef SERIAL_DEBUG_ENABLE
#define LOG_CMD_IN(x)               \
    {                               \
        if (sserial != NULL)        \
        {                           \
            sserial->print('<');    \
            sserial->print(x, HEX); \
            sserial->print(' ');    \
        }                           \
    }
#ifdef SERIAL_DEBUG_SPI_ENABLE
#define SPI_LOG(x)                \
    {                             \
        if (sserial != NULL)      \
        {                         \
            sserial->println();   \
            sserial->print('S');  \
            sserial->print(addr); \
            sserial->print(' ');  \
            sserial->print(x);    \
        }                         \
    }
#define HVSP_LOG(x, y, z)             \
    {                                 \
        if (sserial != NULL)          \
        {                             \
            sserial->print("H");      \
            sserial->print(" 0x");    \
            sserial->print(x, HEX);   \
            sserial->print(" 0x");    \
            sserial->print(y, HEX);   \
            sserial->print(" 0x");    \
            sserial->println(z, HEX); \
        }                             \
    }
#else
#define SPI_LOG(x) ;
#define HVSP_LOG(x, y, z) ;
#endif

#define SERIAL_OUTC(x)                             \
    {                                              \
        if (sserial != NULL)                       \
        {                                          \
            sserial->print('>');                   \
            sserial->print((unsigned char)x, HEX); \
            sserial->print(' ');                   \
        }                                          \
        Serial.print(x);                           \
    }
#define SERIAL_OUTS(x)           \
    {                            \
        if (sserial != NULL)     \
        {                        \
            sserial->println();  \
            sserial->print('>'); \
            sserial->print(x);   \
        }                        \
        Serial.print(x);         \
    }
#define SERIAL_OUT1(s)          \
    {                           \
        if (sserial != NULL)    \
        {                       \
            sserial->print(s);  \
            sserial->println(); \
        }                       \
    }
#define SERIAL_OUT(s, x)         \
    {                            \
        if (sserial != NULL)     \
        {                        \
            sserial->print(s);   \
            sserial->print(' '); \
            sserial->print(x);   \
            sserial->println();  \
        }                        \
    }
#define SERIAL_OUT3(s, x, y)     \
    {                            \
        if (sserial != NULL)     \
        {                        \
            sserial->print(s);   \
            sserial->print(' '); \
            sserial->print(x);   \
            sserial->print(' '); \
            sserial->print(y);   \
            sserial->println();  \
        }                        \
    }

#define RESET_INIT                \
    {                             \
        SERIAL_OUT1("_RES_INIT"); \
        _RESET_INIT;              \
    }

#define RESET_Z                  \
    {                            \
        SERIAL_OUT1("_RESET_Z"); \
        _RESET_Z;                \
    }

#define RESET_HIGH                  \
    {                               \
        SERIAL_OUT1("_RESET_HIGH"); \
        _RESET_HIGH;                \
    }

#define RESET_LOW                  \
    {                              \
        SERIAL_OUT1("_RESET_LOW"); \
        _RESET_LOW;                \
    }

#define RESET_HIGH_12                  \
    {                                  \
        SERIAL_OUT1("_RESET_HIGH_12"); \
        _RESET_HIGH_12;                \
    }

#define BUFFER_INIT                  \
    {                                \
        SERIAL_OUT1("_BUFFER_INIT"); \
        _BUFFER_INIT;                \
    }

#define BUFFER_ON                  \
    {                              \
        SERIAL_OUT1("_BUFFER_ON"); \
        _BUFFER_ON;                \
    }

#define BUFFER_OFF                  \
    {                               \
        SERIAL_OUT1("_BUFFER_OFF"); \
        _BUFFER_OFF;                \
    }

#define BUFFER_HV_PROG                  \
    {                                   \
        SERIAL_OUT1("_BUFFER_HV_PROG"); \
        _BUFFER_HV_PROG;                \
    }

#define SETUP_PINS_HVSP_INIT \
    {                                   \
        SERIAL_OUT1("_SETUP_PINS_HVSP_INIT"); \
        _SETUP_PINS_HVSP_INIT;                \
    }

#define SETUP_PINS_HVSP \
    {                                   \
        SERIAL_OUT1("_SETUP_PINS_HVSP"); \
        _SETUP_PINS_HVSP;                \
    }

#define SSERIAL_INIT \
    _SSERIAL_INIT;   \
    sserial->println(F("Ready..."));

#define SSERIAL_DEINIT               \
    sserial->println(F("Stopping")); \
    _SSERIAL_DEINIT;

#else
#define LOG_CMD_IN(x) ;
#define SERIAL_OUTC(x)   \
    {                    \
        Serial.print(x); \
    }
#define SERIAL_OUTS(x)   \
    {                    \
        Serial.print(x); \
    }

#define SERIAL_OUT1(s) ;
#define SERIAL_OUT(s, x) ;
#define SERIAL_OUT3(s, x, y) ;

#define SPI_LOG(x) ;
#define HVSP_LOG(x, y, z) ;

#define RESET_INIT _RESET_INIT
#define RESET_Z _RESET_Z
#define RESET_HIGH _RESET_HIGH
#define RESET_LOW _RESET_LOW
#define RESET_HIGH_12 _RESET_HIGH_12

#define BUFFER_INIT _BUFFER_INIT
#define BUFFER_ON _BUFFER_ON
#define BUFFER_OFF _BUFFER_OFF
#define BUFFER_HV_PROG _BUFFER_HV_PROG

#define SSERIAL_INIT _SSERIAL_INIT
#define SSERIAL_DEINIT _SSERIAL_DEINIT

#define SETUP_PINS_HVSP_INIT _SETUP_PINS_HVSP_INIT
#define SETUP_PINS_HVSP _SETUP_PINS_HVSP

#endif
#endif