#include <avr/io.h>
#include <util/delay.h>

#ifdef _ATmega_x8_
#define PINS_COUNT 8
#else
#define PINS_COUNT 3
#endif
#define BLINK_ON 250
#define BLINK_OFF 250

int main()
{
  // Pin mode - output
  for (uint8_t i = 0; i < PINS_COUNT; i++)
    DDRB = DDRB | _BV(i);

  while (1)
  {
    for (uint8_t i = 0; i < PINS_COUNT; i++)
    {
      PORTB |= _BV(i);
      _delay_ms(BLINK_ON);
      PORTB &= ~_BV(i);
      _delay_ms(BLINK_OFF);
    }
  }

  // DDRB = 1;                        // PB0 as an output
  // TCCR0A = 1 << COM0A0;            // Toggle OC0A
  // TCCR0B = 1 << WGM02 | 3 << CS00; // CTC mode and set prescaler to 64
  // OCR0A = 3905;                    // Frequency 2Hz
  // while (1)
  //   ;
}