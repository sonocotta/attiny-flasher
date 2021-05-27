

// see functions below ////////////////////////////////
// SIN  0b0aa1aaaa replace a with 6 address bits
// SOUT 0b1aa1aaaa replace a with 6 address bits
// SLDCS  0b1000aaaa replace a with address bits
// SSTCS  0b1100aaaa replace a with address bits
///////////////////////////////////////////////////////

char Sread(void);
uint8_t byteval(char c1, char c2);

#define PIN_BUFEN 6  // Enable output buffer, active HIGH on 74HC126D, active LOW on 74HC125D
#ifdef BUF_74HC126D
#define BUFFER_ON  digitalWrite(PIN_BUFEN, HIGH);
#define BUFFER_OFF digitalWrite(PIN_BUFEN, LOW);
#endif
#ifdef BUF_74HC125D
#define BUFFER_ON  digitalWrite(PIN_BUFEN, LOW);
#define BUFFER_OFF digitalWrite(PIN_BUFEN, HIGH);
#endif

#define QUICK_RES digitalWrite(SS, HIGH); _delay_ms(1); digitalWrite(SS, LOW); _delay_ms(10); digitalWrite(SS, HIGH);
