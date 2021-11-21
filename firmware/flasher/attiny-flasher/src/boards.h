#ifndef _BOARDS_H_
#define _BOARDS_H_

// Connection with host PC (HW serial)
#define BAUDRATE_IN 115200

// Connection with target MCU (SW serial)
#define BAUDRATE_OUT 115200

// ======= REVISION CAPABILITES DEFINITION ============

#if defined(FLASHER_REV_C)
#define BUF_74HC125D
// Tiny-linked Serial pins
#define PIN_SERIAL_RX 5
#define PIN_SERIAL_TX 4 // sometimes 2
#endif

#if defined(FLASHER_REV_D)
#define BUF_74HC125D
// Tiny-linked Serial pins
#define PIN_SERIAL_RX 5
#define PIN_SERIAL_TX 4
#define SERIAL_SENSOR_EN
#define SERIAL_SENSOR_PIN A3
#endif

#if defined(FLASHER_REV_E) || defined(FLASHER_REV_F) || defined(FLASHER_REV_G)
#define REV_D_TWO_PIN_RESET
#define HVSP_PROGRAMMER
#define SERIAL_SENSOR_EN
#define SERIAL_SENSOR_PIN A3
#define BUF_74HC241

// Tiny-linked Serial pins
#define PIN_SERIAL_RX 4
#define PIN_SERIAL_TX 5

// ============== RESET MODES DEFINITION ==============
#define RESET_SENSOR_EN
#define RESET_SENSOR_PIN 2
#define RESET_SENSOR_WRITE(X)          \
    pinMode(RESET_SENSOR_PIN, OUTPUT); \
    digitalWrite(RESET_SENSOR_PIN, X);
#define RESET_SENSOR_READ \
    pinMode(RESET_SENSOR_PIN, INPUT);
#endif

#ifdef MEGAFLASHER_REV_F
#define MEGAFLASHER
#define BUF_74HC125D
// Tiny-linked Serial pins
#define PIN_SERIAL_RX 4
#define PIN_SERIAL_TX 5
#define SERIAL_SENSOR_EN
#define SERIAL_SENSOR_PIN A3
#endif

#define RESET 10
#if defined(REV_D_TWO_PIN_RESET)
#define RESET_PULL A2 // Need another pin to pull it down properly
#endif

// Enable output buffer, active HIGH on 74HC126D, active LOW on 74HC125D
#if defined(FLASHER_REV_F) || defined(FLASHER_REV_G) || defined(MEGAFLASHER_REV_F)
#define PIN_BUFEN A1
#else
#define PIN_BUFEN 6
#endif

#ifdef BUF_74HC126D
#define _BUFFER_HV_PROG ;
#define _BUFFER_INIT            \
    pinMode(PIN_BUFEN, OUTPUT); \
#define _BUFFER_ON          \
        digitalWrite(PIN_BUFEN, HIGH);
#define _BUFFER_OFF \
    digitalWrite(PIN_BUFEN, LOW);
#endif

#ifdef BUF_74HC125D
#define _BUFFER_HV_PROG ;
#define _BUFFER_INIT \
    pinMode(PIN_BUFEN, OUTPUT);
#define _BUFFER_ON \
    digitalWrite(PIN_BUFEN, LOW);
#define _BUFFER_OFF \
    digitalWrite(PIN_BUFEN, HIGH);
#endif

#ifdef BUF_74HC241
#define _BUFFER_HV_PROG         \
    pinMode(PIN_BUFEN, OUTPUT); \
    digitalWrite(PIN_BUFEN, LOW); \
    if (sserial != NULL) pinMode(PIN_SERIAL_TX, INPUT) // SERIAL_TX connected to HVSP_CSI
#define _BUFFER_INIT ;
#define _BUFFER_ON              \
    pinMode(PIN_BUFEN, OUTPUT); \
    digitalWrite(PIN_BUFEN, HIGH);
#define _BUFFER_OFF \
    pinMode(PIN_BUFEN, INPUT); \
    if (sserial != NULL) pinMode(PIN_SERIAL_TX, OUTPUT) // SERIAL_TX connected to HVSP_CSI
#endif

#if defined(REV_D_TWO_PIN_RESET)
#define _RESET_INIT         \
    pinMode(RESET, OUTPUT); \
    pinMode(RESET_PULL, OUTPUT);
#define _RESET_HIGH_12             \
    RESET_SENSOR_WRITE(HIGH);      \
    digitalWrite(RESET_PULL, LOW); \
    digitalWrite(RESET, HIGH);
#define _RESET_HIGH                \
    RESET_SENSOR_WRITE(LOW);       \
    digitalWrite(RESET_PULL, LOW); \
    digitalWrite(RESET, HIGH);
#define _RESET_LOW                  \
    digitalWrite(RESET, LOW);       \
    digitalWrite(RESET_PULL, HIGH); \
    RESET_SENSOR_WRITE(LOW);
#define _RESET_Z                   \
    digitalWrite(RESET_PULL, LOW); \
    digitalWrite(RESET, LOW);      \
    RESET_SENSOR_READ;
#else
#define _RESET_INIT ;
#define _RESET_HIGH_12 ;
#define _RESET_HIGH         \
    pinMode(RESET, OUTPUT); \
    digitalWrite(RESET, HIGH);
#define _RESET_LOW          \
    pinMode(RESET, OUTPUT); \
    digitalWrite(RESET, LOW);
#define _RESET_Z \
    pinMode(RESET, INPUT);
#endif

// ======= INDICATION PINS ============================

#define PULSE_TIME 10
#define LED_HB 9    // Heartbeat
#define LED_ERR 8   // Error
#define LED_PMODE 7 // Programm

#if defined(OLED_LIB_ADAFRUIT) || defined(OLED_LIB_TINY)
#define OLED_ENABLE
#endif

// ============== PIN MODES DEFINITION ==============

#define PIN_MOSI MOSI
#define PIN_MISO MISO
#define PIN_SCK SCK

#define SETUP_PINS_ISP         \
    pinMode(PIN_MOSI, OUTPUT); \
    pinMode(PIN_MISO, INPUT);  \
    pinMode(PIN_SCK, OUTPUT);

#define PIN_SCI 3
#define PIN_SCI_PORT PORTD
#define PIN_SCI_NUM PD3

#define PIN_SDO SCK
#define PIN_SDO_PORT PORTB
#define PIN_SDO_PIN PINB
#define PIN_SDO_NUM PB5

#define PIN_SII MISO
#define PIN_SII_PORT PORTB
#define PIN_SII_NUM PB4

#define PIN_SDI MOSI
#define PIN_SDI_PORT PORTB
#define PIN_SDI_NUM PB3

#define _SETUP_PINS_HVSP       \
    pinMode(PIN_SDI, OUTPUT); \
    pinMode(PIN_SII, OUTPUT); \
    pinMode(PIN_SCI, OUTPUT); \
    pinMode(PIN_SDO, INPUT);  

#define _SETUP_PINS_HVSP_INIT    \
    pinMode(PIN_SDO, OUTPUT);   \
    digitalWrite(PIN_SDI, LOW); \
    digitalWrite(PIN_SII, LOW); \
    digitalWrite(PIN_SDO, LOW);

#define HVSP_SDI_LOW \
    PIN_SDI_PORT &= ~_BV(PIN_SDI_NUM) // digitalWrite(PIN_SDI, LOW);

#define HVSP_SDI_HIGH \
    PIN_SDI_PORT |= _BV(PIN_SDI_NUM) // digitalWrite(PIN_SDI, HIGH);

#define HVSP_SII_LOW \
    PIN_SII_PORT &= ~_BV(PIN_SII_NUM) // digitalWrite(PIN_SII, LOW);

#define HVSP_SII_HIGH \
    PIN_SII_PORT |= _BV(PIN_SII_NUM) // digitalWrite(PIN_SII, HIGH);

#define HVSP_SCI_LOW \
    PIN_SCI_PORT &= ~_BV(PIN_SCI_NUM) // digitalWrite(PIN_SCI, LOW);

#define HVSP_SCI_HIGH \
    PIN_SCI_PORT |= _BV(PIN_SCI_NUM) // digitalWrite(PIN_SCI, HIGH);

#define HVSP_SDO_STATE \
    PIN_SDO_PIN &_BV(PIN_SDO_NUM) //digitalRead(PIN_SDO)

// ============== SW SERIAL ==============

#if defined(OLED_ENABLE) || defined(SERIAL_BRIDGE_ENABLE) || defined(SERIAL_DEBUG_ENABLE)
#define SW_SERIAL_ENABLE
#endif

#endif