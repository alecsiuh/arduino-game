#include <util/delay.h>
#include <avr/io.h>
#include "LEDlibrary.h"

#define NUMBER_OF_LEDS 4 //Define is a "preprocessor directive". It ensures that every NUMBER_OF_LEDS will be replaced by 4 in the following code

void enableLed ( int lednumber ) //C has no classes; functions can be included directly in the .c file.
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    DDRB |= ( 1 << ( PB2 + lednumber ));    //Check the tutorial "Writing to a Pin". We know from the documentation on
                                            //the multifunctional shield that the LEDs start at PB2
}

void lightUpLed ( int lednumber )    //Note: enabled LEDs light up immediately ( 0 = on )
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    PORTB &= ~( 1 << ( PB2 + lednumber ));  //Check the tutorial on "Bit Operations" to know what happens in this line.
}

void lightDownLed ( int lednumber )
{
    if ( lednumber < 0 || lednumber > 3 ) return;
    PORTB |= ( 1 << ( PB2 + lednumber ));   //Make sure you understand this line as well!
}

void enableMultipleLeds (uint8_t leds){} //enables any combination of the 4 LEDs at once
//the parameter that this function receives is an 8-bit value (a byte)
//if I enter 0b00001101, the 0-th, the 2nd and the 3rd led will be enabled for writing
//if I enter 0b00000110, the 1st and 2nd led will be enabled

void enableAllLeds ( ){}

void lightUpMultipleLeds (uint8_t leds){}

void lightUpAllLeds( ){}

void lightDownMultipleLeds (uint8_t leds){}

void lightDownAllLeds( ){}

void lightToggleOneLed (int lednumber){}

void dimLed (int lednumber, int percentage, int duration){}

void fadeInLed(int lednumber, int duration){}

void fadeOutLed(int lednumber, int duration){}