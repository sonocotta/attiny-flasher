#ifndef _BOARDS_H_
#define _BOARDS_H_

// Connection with host PC (HW serial)
#define BAUDRATE_IN 115200
// Connection with target MCU (SW serial)
#define BAUDRATE_OUT 115200

// REVISION CAPABILITES DEFINITION
#if defined(FLASHER_REV_C) || defined(FLASHER_REV_D)
#define BUF_74HC125D
#endif

#ifdef FLASHER_REV_E
#define REV_D_TWO_PIN_RESET
#define RESET_SENSOR_EN
#define BUF_74HC241

#define RESET_SENSOR_PIN 2
#define RESET_SENSOR_WRITE(X)          \
    pinMode(RESET_SENSOR_PIN, OUTPUT); \
    digitalWrite(RESET_SENSOR_PIN, X);
#define RESET_SENSOR_READ \
    pinMode(RESET_SENSOR_PIN, INPUT);
#endif

#define RESET 10 // Use pin 10 to reset the target rather than SS
#if defined(REV_D_TWO_PIN_RESET)
#define RESET_PULL A2 // Need another pin to pull it down properly
#endif

#define PIN_BUFEN 6 // Enable output buffer, active HIGH on 74HC126D, active LOW on 74HC125D
#ifdef BUF_74HC126D
#define BUFFER_HV_PROG ;
#define BUFFER_ON digitalWrite(PIN_BUFEN, HIGH);
#define BUFFER_OFF digitalWrite(PIN_BUFEN, LOW);
#endif
#ifdef BUF_74HC125D
#define BUFFER_HV_PROG ;
#define BUFFER_ON digitalWrite(PIN_BUFEN, LOW);
#define BUFFER_OFF digitalWrite(PIN_BUFEN, HIGH);
#endif
#ifdef BUF_74HC241
#define BUFFER_HV_PROG          \
    pinMode(PIN_BUFEN, OUTPUT); \
    digitalWrite(PIN_BUFEN, LOW);
#define BUFFER_ON               \
    pinMode(PIN_BUFEN, OUTPUT); \
    digitalWrite(PIN_BUFEN, HIGH);
#define BUFFER_OFF pinMode(PIN_BUFEN, INPUT);
#endif

#if defined(REV_D_TWO_PIN_RESET)
#define RESET_INIT          \
    pinMode(RESET, OUTPUT); \
    pinMode(RESET_PULL, OUTPUT);
#define RESET_HIGH_12              \
    RESET_SENSOR_WRITE(HIGH);      \
    digitalWrite(RESET_PULL, LOW); \
    digitalWrite(RESET, HIGH);
#define RESET_HIGH                 \
    RESET_SENSOR_WRITE(LOW);       \
    digitalWrite(RESET_PULL, LOW); \
    digitalWrite(RESET, HIGH);
#define RESET_LOW                   \
    digitalWrite(RESET, LOW);       \
    digitalWrite(RESET_PULL, HIGH); \
    RESET_SENSOR_WRITE(LOW);
#define RESET_Z                    \
    digitalWrite(RESET_PULL, LOW); \
    digitalWrite(RESET, LOW);      \
    RESET_SENSOR_READ;
#else
#define RESET_INIT ;
#define RESET_HIGH_12 ;
#define RESET_HIGH          \
    pinMode(RESET, OUTPUT); \
    digitalWrite(RESET, HIGH);
#define RESET_LOW           \
    pinMode(RESET, OUTPUT); \
    digitalWrite(RESET, LOW);
#define RESET_Z pinMode(RESET, INPUT); // high impedance state
#endif

#define PIN_MOSI MOSI
#define PIN_MISO MISO
#define PIN_SCK SCK

#define SETUP_PINS_ISP         \
    pinMode(PIN_MOSI, OUTPUT); \
    pinMode(PIN_MISO, INPUT);  \
    pinMode(PIN_SCK, OUTPUT);

#define PIN_SCI 3
#define PIN_SDO SCK
#define PIN_SII MISO
#define PIN_SDI MOSI

#define SETUP_PINS_HVSP       \
    pinMode(PIN_SDI, OUTPUT); \
    pinMode(PIN_SII, OUTPUT); \
    pinMode(PIN_SCI, OUTPUT); \
    pinMode(PIN_SDO, INPUT);

#define SETUP_PINS_HVSP_INIT    \
    pinMode(PIN_SDO, OUTPUT);   \
    digitalWrite(PIN_SDI, LOW); \
    digitalWrite(PIN_SII, LOW); \
    digitalWrite(PIN_SDO, LOW);

#define HVSP_SDI_LOW \
    digitalWrite(PIN_SDI, LOW);

#define HVSP_SDI_HIGH \
    digitalWrite(PIN_SDI, HIGH);

#define HVSP_SII_LOW \
    digitalWrite(PIN_SII, LOW);

#define HVSP_SII_HIGH \
    digitalWrite(PIN_SII, HIGH);

#define HVSP_SCI_LOW \
    digitalWrite(PIN_SCI, LOW);

#define HVSP_SCI_HIGH \
    digitalWrite(PIN_SCI, HIGH);

#define HVSP_SDO_STATE \
    digitalRead(PIN_SDO)

#endif