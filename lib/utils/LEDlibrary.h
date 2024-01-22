#include <util/delay.h>
#include <avr/io.h>

#define NUMBER_OF_LEDS 4

void enableLed ( int lednumber );
void lightUpLed ( int lednumber );
void lightDownLed ( int lednumber );
void enableMultipleLeds (uint8_t leds);