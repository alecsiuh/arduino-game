#include <util/delay.h>
#include <avr/io.h>
#include "led.h"

#define NUMBER_OF_LEDS 4 

void enableLed ( int lednumber ) 
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    DDRB |= ( 1 << ( PB2 + lednumber ));     
}

void lightUpLed ( int lednumber )   
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    PORTB &= ~( 1 << ( PB2 + lednumber ));   
} 

void lightDownLed ( int lednumber )
{
    if ( lednumber < 0 || lednumber > 3 ) return;
    PORTB |= ( 1 << ( PB2 + lednumber ));   
}

void lightUpAllLeds(){
    for ( int i = 0; i < 4; i++ ) {  
        enableLed(i);  
    }

    while (1) {
        for(int i=0; i<4; i++){
            lightDownLed(i);
            _delay_ms(1000);
        }
    }
}
 