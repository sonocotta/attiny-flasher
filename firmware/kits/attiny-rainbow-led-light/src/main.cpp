#include <Arduino.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <light_ws2812.h>
#include "main.h"

struct cRGB leds[NUMPIXELS];

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
void wheel(uint8_t WheelPos, uint8_t i)
{
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85)
    {
        leds[i].r = 255 - WheelPos * 3;
        leds[i].g = 0;
        leds[i].b = WheelPos * 3;
        //return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
        return;
    }
    if (WheelPos < 170)
    {
        WheelPos -= 85;
        //return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
        leds[i].r = 0;
        leds[i].g = WheelPos * 3;
        leds[i].b = 255 - WheelPos * 3;
        return;
    }
    WheelPos -= 170;
    // return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);

    leds[i].r = WheelPos * 3;
    leds[i].g = 255 - WheelPos * 3;
    leds[i].b = 0;
    return;
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait)
{
    uint16_t i, j;

    for (j = 0; j < 256 * 5; j++)
    { // 5 cycles of all colors on wheel
        for (i = 0; i < NUMPIXELS; i++)
        {
            #ifdef COLOR_SAME
            wheel(((0 * 256 / NUMPIXELS) + j) & 255, i);
            #else
            wheel(((i * 256 / NUMPIXELS) + j) & 255, i);
            #endif
        }
        //strip.show();
        ws2812_setleds(leds, NUMPIXELS);
        _delay_ms(wait);
    }
}

void setup()
{
    pinMode(PIN_BTN_1, INPUT);
    pinMode(PIN_BTN_2, INPUT);
    pinMode(PIN_BTN_3, INPUT);
}

void set_leds_val(uint8_t val)
{
    for (uint8_t i = 0; i < NUMPIXELS; i++)
    {
        leds[i].r = val;
        leds[i].g = 0;
        leds[i].b = 0;
    }
}

void loop()
{
    while (1)
        rainbowCycle(100);
}