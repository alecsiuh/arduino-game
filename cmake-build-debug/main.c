/*
TODO:
  expand betting function
  make a click sound when displaying the last randomized number
  play music when betting
  check buttons (see coins and change display have the same button)
  check sound
  (serial monitor) the total time of the game (in minutes and seconds) and the total number of times the player won
*/

#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>
#include <stdio.h>
#include <stdlib.h>
#include <buzzer.h>
#include <timer.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>
#include <button.h>
#include <display.h>
#include <led.h>
#include <stdint.h>


#define MAX_SLOT_DISPLAY 4 /*the max display number for a game*/
#define MAX_COINS 9999 /*winning amount of coins*/
#define STARTING_COINS 50 /*coins at the beginning of the game*/
int coins; /*coins gained during the game*/
int slot_display = 2; /*minimum display number for a game that can be increased*/
int slot_number; /*gambled number*/

/*possible symbol values*/
// char x;
// char y;
// char z;
// char j;

int x;
int y;
int z;
int j;

/*banner words*/
char game_name[16] = "ONE_ARMED_BANDIT";
char loss[9] = "YOU_LOST";
char win[10] = "BANK_BREAK";

/*dynamic memory setup*/
typedef struct game {
    int coins_won;
    int sequence;
    int current_coins;
    int gambled_number;
    int bet_coins;
} game;

game *game_stats(int sequence, int current_coins, int bet_coins, int gambled_number, int coins_won) {
    game *stats = (game *) malloc(sizeof(game));
    if (stats != NULL) {
        stats->sequence = sequence;
        stats->current_coins = current_coins;
        stats->bet_coins = 1;
        stats->gambled_number = gambled_number;
        stats->coins_won = coins_won;
    }
    return stats;
}

void clear_game_stats(game *stats) {
    free(stats);
}

/*10 slot symbols to randomize*/
const uint8_t SLOT_SYMBOLS[] = {0xae, 0xf6, 0xdb, 0xbf, 0xed, 0xef, 0xfb, 0xf7, 0xfe, 0xf1};

/* set up the ADC */
void initADC() {
    ADMUX |= (1 << REFS0);
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRA |= (1 << ADEN);
}

uint32_t counter = 0;

void initTimer0() {
    TCCR0A |= _BV(WGM00) | _BV(WGM01);
    TCCR0B |= _BV(CS02) | _BV(CS00);
    TIMSK0 |= _BV(TOIE0);
    TIMSK0 |= _BV(OCIE0A);

    sei();
}

ISR(TIMER0_COMPA_vect){
        if (bit_is_clear(PINC, PC2)){
            counter++;
        }
}
ISR(TIMER0_OVF_vect){
        if (bit_is_clear(PINC, PC2)){
            counter++;
        }
}

// todo: check
void game_won() {
    for (int i = 5; i >= 0; i--) {
        for (int y = 0; y <= 10; y++) { /*as long as y is smaller or equal to the amount of letters in the word*/
            char index = win[y]; /*y increases the index and picks all the letters of the word at a time*/
            writeCharToSegment(i, index); /*write the letter on the display one by one*/
            _delay_ms(500); /*add a delay so that the numbers are actually visible*/
        }
    }
}

void game_lost() {
    for (int i = 5; i >= 0; i--) {
        for (int y = 0; y < 10; y++) { /*as long as y is smaller or equal to the amount of letters in the word*/
            char index = loss[y]; /*y increases the index and picks all the letters of the word at a time*/
            writeCharToSegment(i, index); /*write the letter on the display one by one*/
            _delay_ms(500); /*add a delay so that the numbers are actually visible*/
        }
    }
}

void randomize_slot() {
    if (bit_is_clear(PINC, PC3)) {
        x = rand() % 9 + 1;
        y = rand() % 9 + 1;
        j = rand() % 9 + 1;
        z = rand() % 9 + 1; /*randomize 4 different numbers so that they're easier to compare*/

        slot_number = (x * 1000) + (y * 100) + (j * 10) + z; /*make a 2 digit number to display*/
        writeNumberAndWait(slot_number, 100); /*add a delay so that the randomized numbers can be seen*/
    }
}

void activate_slot() {
    int sequence_number = 1;
    int *coins = (int *) calloc(1, sizeof(int));
    *coins = STARTING_COINS;
// todo: use the *coins to refer to the gambled coins

    initADC();
    ADCSRA |= (1 << ADSC);
    loop_until_bit_is_clear(ADCSRA, ADSC);
    uint16_t value = ADC;
    initUSART();
    srand(value); /* use the potentiometer's value as random seed */

    game *stats = game_stats(sequence_number, *coins, 1, 0, 0);


    if (slot_display == 2) { /*if the player chooses a 2 number display*/
        for (int i = 0; i < 2; i++) {
            lightUpLed(i);
        } /*light up 2 LEDs to show number the player chose*/

        for (int i = 0; i <= 10; i++) {
            randomize_slot();
            // todo: check
            if (bit_is_clear(PINC, PC3)) {
                *coins--;
                for (int i = 0; i < 4; i++) {
                    enableLed(i);
                    lightDownLed(i);
                    _delay_ms(100);
                } /*light up the LEDs*/
            }

            if ((x = y)) {
                *coins += 5;
            }
        }
        writeNumberToSegment(2, j);
        writeNumberToSegment(3, z);

    } else if (slot_display == 3) { /*if the player chooses a 3 number display*/
        for (int i = 0; i < 3; i++) {
            lightUpLed(i);
        } /*light up 3 LEDs to show number the player chose*/

        for (int i = 0; i <= 10; i++) {
            randomize_slot();
            // todo: check
            if (bit_is_clear(PINC, PC3)) {
                *coins--;
                for (int i = 0; i < 4; i++) {
                    enableLed(i);
                    lightDownLed(i);
                    _delay_ms(100);
                } /*light up the LEDs*/
            }

            if ((x = y = j)) {
                *coins += 50;
            }
        }
        writeNumberToSegment(1, y);
        writeNumberToSegment(2, j);
        writeNumberToSegment(3, z);

    } else if (slot_display == 4) {
        for (int i = 0; i < 4; i++) { /*if the player chooses a 4 number display*/
            lightUpLed(i);
        } /*light up 4 LEDs to show number the player chose*/

        for (int i = 0; i <= 10; i++) {
            randomize_slot();
            // todo: check
            if (bit_is_clear(PINC, PC3)) {
                *coins--;
                for (int i = 0; i < 4; i++) {
                    enableLed(i);
                    lightDownLed(i);
                    _delay_ms(100);
                } /*light up the LEDs*/
            }

            if ((x = y = j = z)) {
                *coins += 500;
            }
        }
        writeNumberToSegment(0, x);
        writeNumberToSegment(1, y);
        writeNumberToSegment(2, j);
        writeNumberToSegment(3, z);
    }

    // if ((*coins = MAX_COINS)) {
    //     game_won();
    // }

    // if (*coins <= 4) {
    //     writeNumberToSegment(0, *coins);
    // }
}

// todo: check
void see_amount_of_coins() {
    if (bit_is_clear(PINC, PC2)) { /*if you press on the middle button*/
        writeNumberAndWait(coins, 3000); /*show the amount of coins the player has*/
    }

    if (bit_is_clear(PINC, PC2)) { /*by pressing the middle button again*/
        {
            activate_slot(); /*resume game*/
        }
    }
}

void choose_slot_display() {
    activate_slot(); /*start actual game*/
    if (slot_display <=
        MAX_SLOT_DISPLAY) { /*as long as the chosen display is smaller or equal to the MAX_SLOT_DISPLAY*/
        if (bit_is_clear(PINC, PC1)) { /*by pressing the left button*/
            slot_display++; /*increase the display number*/
        }
    }

    if ((slot_display = MAX_SLOT_DISPLAY) &&
        (slot_display <= 2)) { /*if the display number is equal to the MAX_SLOT_DISPLAY but more or equal to 2*/
        if (bit_is_clear(PINC, PC1)) { /*by pressing the left button*/
            slot_display--; /*increase the display number*/
        }
    }
}

void display_progress(const game *stats) {
    printf("%d: %d coins available - %d coin bet - %d - %d coins won\n",
           stats->sequence,
           stats->current_coins,
           stats->bet_coins,
           stats->gambled_number,
           stats->coins_won);
}

int main() {
    initUSART();
    initDisplay();

    while (1) {
        choose_slot_display();
    }
    return 0;
}
