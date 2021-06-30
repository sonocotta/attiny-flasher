#define HFUSE 0x747C
#define LFUSE 0x646C
#define EFUSE 0x666E

// Define ATTiny series signatures
#define ATTINY13 0x9007 // L: 0x6A, H: 0xFF             8 pin
#define ATTINY24 0x910B // L: 0x62, H: 0xDF, E: 0xFF   14 pin
#define ATTINY25 0x9108 // L: 0x62, H: 0xDF, E: 0xFF    8 pin
#define ATTINY44 0x9207 // L: 0x62, H: 0xDF, E: 0xFFF  14 pin
#define ATTINY45 0x9206 // L: 0x62, H: 0xDF, E: 0xFF    8 pin
#define ATTINY84 0x930C // L: 0x62, H: 0xDF, E: 0xFFF  14 pin
#define ATTINY85 0x930B // L: 0x62, H: 0xDF, E: 0xFF    8 pin

#define HVSP_LOG(x, y, z)     \
    {                         \
        Serial.print("> ");   \
        Serial.print(" 0x");  \
        Serial.print(x, HEX); \
        Serial.print(" 0x");  \
        Serial.print(y, HEX); \
        Serial.print(" 0x");  \
        Serial.print(z, HEX); \
        Serial.println(" <"); \
    }

void readFuses();
void writeFuse(unsigned int fuse, byte val);
unsigned int readSignature();