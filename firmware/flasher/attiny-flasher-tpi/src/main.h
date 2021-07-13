#include "boards.h"

// see functions below ////////////////////////////////
// SIN  0b0aa1aaaa replace a with 6 address bits
// SOUT 0b1aa1aaaa replace a with 6 address bits
// SLDCS  0b1000aaaa replace a with address bits
// SSTCS  0b1100aaaa replace a with address bits
///////////////////////////////////////////////////////

#if defined(SW_SERIAL_ENABLE)
#define SW_SERIAL_TX 2
#include <SoftwareSerial.h>
SoftwareSerial SSerial(-1, SW_SERIAL_TX);
#undef SERIAL
#endif

char Sread(void);
uint8_t byteval(char c1, char c2);
