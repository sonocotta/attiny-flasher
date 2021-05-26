//#define BUF_74HC125D // <- prototype
//#define BUF_74HC125D // <- rev B, C, D
#define BUF_74HC241 // <- rev E

#define PIN_BUFEN 6  // Enable output buffer, active HIGH on 74HC126D, active LOW on 74HC125D
#ifdef BUF_74HC126D
#define BUFFER_ON  digitalWrite(PIN_BUFEN, HIGH);
#define BUFFER_OFF digitalWrite(PIN_BUFEN, LOW);
#endif
#ifdef BUF_74HC125D
#define BUFFER_ON  digitalWrite(PIN_BUFEN, LOW);
#define BUFFER_OFF digitalWrite(PIN_BUFEN, HIGH);
#endif
#ifdef BUF_74HC241
#define BUFFER_HV_PROG pinMode(PIN_BUFEN, OUTPUT); digitalWrite(PIN_BUFEN, LOW);
#define BUFFER_ON pinMode(PIN_BUFEN, OUTPUT); digitalWrite(PIN_BUFEN, HIGH);
#define BUFFER_OFF pinMode(PIN_BUFEN, INPUT);
#endif

#define RESET      10 // Use pin 10 to reset the target rather than SS
#define RESET_PULL A2
#define LED_HB     9  // Heartbeat
#define LED_ERR    8  // Error
#define LED_PMODE  7  // Programm

#define RESET_HIGH digitalWrite(RESET_PULL, LOW); digitalWrite(RESET, HIGH);
#define RESET_LOW  digitalWrite(RESET, LOW); digitalWrite(RESET_PULL, HIGH);
#define RESET_Z    digitalWrite(RESET_PULL, LOW); digitalWrite(RESET, LOW);

#define  HFUSE  0x747C
#define  LFUSE  0x646C
#define  EFUSE  0x666E

// Define ATTiny series signatures
#define  ATTINY13   0x9007  // L: 0x6A, H: 0xFF             8 pin
#define  ATTINY24   0x910B  // L: 0x62, H: 0xDF, E: 0xFF   14 pin
#define  ATTINY25   0x9108  // L: 0x62, H: 0xDF, E: 0xFF    8 pin
#define  ATTINY44   0x9207  // L: 0x62, H: 0xDF, E: 0xFFF  14 pin
#define  ATTINY45   0x9206  // L: 0x62, H: 0xDF, E: 0xFF    8 pin
#define  ATTINY84   0x930C  // L: 0x62, H: 0xDF, E: 0xFFF  14 pin
#define  ATTINY85   0x930B  // L: 0x62, H: 0xDF, E: 0xFF    8 pin

//#define  RST     13    // Output to level shifter for !RESET from transistor
#define  SCI     3     // 12    // Target Clock Input
#define  SDO     SCK   // 11    // Target Data Output
#define  SII     MISO  // 10   // Target Instruction Input
#define  SDI     MOSI  // 9    // Target Data Input
//#define  VCC      8    // Target VCC


void readFuses();
void writeFuse(unsigned int fuse, byte val);
unsigned int readSignature();
void readVoltage();