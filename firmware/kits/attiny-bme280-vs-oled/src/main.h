#define _SERIAL_

#ifdef _ATMEGA_328P_
#define PIN_I2C_SDA A4
#define PIN_I2C_SCL A5
#endif

#ifdef _ATTINY_85_
#define PIN_I2C_SDA 0
#define PIN_I2C_SCL 2
#endif

#define NO_DOUBLE_BUFFERING

#define OLED_WIDTH 128
#define OLED_HEIGHT 32

#ifdef _ATTINY_13_
#include "uart.h"
#else 
#include <SoftwareSerial.h>
#endif
 
#define PIN_RX -1
#define PIN_TX 4
#define SERIAL_BAUD 19200

SoftwareSerial SSerial(PIN_RX, PIN_TX);


#define TINY_BME280_I2C
#include "TinyBME280.h"
tiny::BME280 sens;

void temp();
void pres();
void humi();
void print_num(uint32_t num, uint8_t digits, uint8_t point);