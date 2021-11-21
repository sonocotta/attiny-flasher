#if defined(SW_SERIAL_ENABLE)
#include <SoftwareSerial.h>
SoftwareSerial* SSerial = NULL;
#undef SERIAL
#endif

void pulse(int pin, int times);