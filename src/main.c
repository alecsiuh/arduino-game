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
#include <button.h>
#include <display.h>
#include <led.h>

#define MAX_SLOT_DISPLAY 4
#define MAX_COINS 9999
int coins = 50;
int slot_display = 2;
int slot_number = 0;
int sequence = 0;
int coins_won = 0;
int bet_coins = 1;
uint32_t counter = 0;
uint8_t minutes = 0;
uint8_t seconds = 0;
int times_gambled;
int x, y, z, j;

/*dynamic memory setup*/
typedef struct game {
    int sequence;
    int coins;
    int slot_number;
    int bet_coins;
    int coins_won;
} GAME;

GAME *game;

/* set up the ADC */
void initADC() {
    ADMUX |= (1 << REFS0);
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRA |= (1 << ADEN);
}

void initTimer0() {
    TCCR0A |= _BV(WGM01);
    TCCR0B |= _BV(CS02) | _BV(CS00);
    OCR0A = 156;  /* set the compare value for 1-second interrupt*/
    TIMSK0 |= _BV(OCIE0A);
    sei();
}

void startTimer() {
    TCCR2B |= (1 << CS12) | (1 << CS11) | (0 << CS10);
}

void stopTimer() {
    TCCR2B = 0 ;
}

ISR(TIMER0_COMPA_vect) {
    seconds++;
    if (seconds >= 60) {
        seconds = 0;
        minutes++;
    }
}

/*possible symbol values*/
// char x;
// char y;
// char z;
// char j;
/*10 slot symbols to randomize*/
// const uint8_t SLOT_SYMBOLS[] = {0xae, 0xf6, 0xdb, 0xbf, 0xed, 0xef, 0xfb, 0xf7, 0xfe, 0xf1};
// void swap(uint8_t *a, uint8_t *b) {
//     uint8_t temp = *a;
//     *a = *b;
//     *b = temp;
// }

// void shuffle_array(uint8_t *array, int size) {
//     srand(0);
//     for (int i = size - 1; i > 0; i--) {
//         int j = rand() % (i + 1);
//         swap(&array[i], &array[j]);
//     }
// }

// void get_symbols() {
//     const int size = sizeof(SLOT_SYMBOLS) / sizeof(SLOT_SYMBOLS[0]);

//     shuffleArray(SLOT_SYMBOLS, size);

//     for (int i = 0; i < size; i++) {
//         writeCharToSegment(0, SLOT_SYMBOLS[i]);
//         writeCharToSegment(1, SLOT_SYMBOLS[i]);
//         writeCharToSegment(2, SLOT_SYMBOLS[i]);
//         writeCharToSegment(3, SLOT_SYMBOLS[i]);
//     }
//     return 0;
// }



void write_banner(char string[]) {
    for (int j = 0; j < 20; j++){
        if (j >= 3){
          writeCharToSegment(3,string[j]);
          writeCharToSegment(2,string[j-1]);
          writeCharToSegment(1,string[j-2]);
          writeCharToSegment(0,string[j-3]);
          if (counter != 500)
          {
            counter++;
            j--;

          }else{
            counter = 0;
          }
        }
        else if (j == 2){
          writeCharToSegment(3,string[j]);
          writeCharToSegment(2,string[j-1]);
          writeCharToSegment(1,string[j-2]);
          if (counter != 500)
          {
            counter++;
            j--;

          }else{
            counter = 0;
          }
        }
        else if (j == 1){
          writeCharToSegment(3,string[j]);
          writeCharToSegment(2,string[j-1]);
          if (counter != 500)
          {
            counter++;
            j--;

          }else{
            counter = 0;
          }
        }else if (j == 0){
          writeCharToSegment(3,string[j]);
          if (counter != 500)
          {
            counter++;
            j--;

          }else{
            counter = 0;
          }
        }
    }
}

void gamble() {
    if (slot_display == 2) {
        startTimer();
        if (bit_is_clear(PINC, PC3)) {
            sequence++;
            coins--;
            times_gambled++;

            for ( int i = 0; i < 4; i++ ){
                enableLed(i);
                lightUpLed(i);
                _delay_ms( 100 );
            }
            for ( int i = 0; i < 4; i++ ){
                enableLed(i);
                lightDownLed(i);
                _delay_ms( 100 );
            }

            enableBuzzer();
            playTone(1500.250,500);

            for (int i = 0; i <= 100; i++) {
                j = rand() % 9 + 1;
                z = rand() % 9 + 1;

                slot_number = (j * 10) + z; 
                writeNumberAndWait(slot_number, 50); 
            }

            if (z == j) {
                coins_won = 5;
                coins = coins + coins_won;
                enableBuzzer();
                playTone(1500.250,1000);
            } else {
                coins_won = 0;
                coins = coins + coins_won;
            }

            enableBuzzer();
            playTone(1500.250,100);

            game = malloc(sizeof(GAME)); /* alocate memory for the game*/
            game->sequence = sequence;
            game->coins = coins;
            game->coins_won = coins_won;
            game->slot_number = slot_number;

            printf("\n%d :   %d coins available - %d coin bet - %d - %d coins won", sequence, coins, bet_coins, slot_number, coins_won);
        }
        writeNumberToSegment(2, j);
        writeNumberToSegment(3, z);
    } else if (slot_display == 3) {
        startTimer();
        if (bit_is_clear(PINC, PC3)) {
            sequence++;
            coins--;
            times_gambled++;

            for ( int i = 0; i < 4; i++ ){
                enableLed(i);
                lightUpLed(i);
                _delay_ms( 100 );
            }
            for ( int i = 0; i < 4; i++ ){
                enableLed(i);
                lightDownLed(i);
                _delay_ms( 100 );
            }

            enableBuzzer();
            playTone(1500.250,500);

            for (int i = 0; i <= 100; i++) {
                y = rand() % 9 + 1;
                j = rand() % 9 + 1;
                z = rand() % 9 + 1; 

                slot_number = (y * 100) + (j * 10) + z; 
                writeNumberAndWait(slot_number, 50); 
            }

            if (y == j && j == z) {
                coins_won = 50;
                coins = coins + coins_won;
                enableBuzzer();
                playTone(1500.250,1000);
            } else {
                coins_won = 0;
                coins = coins + coins_won;
            }

            enableBuzzer();
            playTone(1500.250,100);

            game = malloc(sizeof(GAME)); /* alocate memory for the game*/
            game->sequence = sequence;
            game->coins = coins;
            game->coins_won = coins_won;
            game->slot_number = slot_number;

            printf("\n%d :   %d coins available - %d coin bet - %d - %d coins won", sequence, coins, bet_coins, slot_number, coins_won);
        }
        writeNumberToSegment(1, y);
        writeNumberToSegment(2, j);
        writeNumberToSegment(3, z);

    } else if (slot_display == 4) {
        startTimer();
        if (bit_is_clear(PINC, PC3)) {
            sequence++;
            coins--;
            times_gambled++;

            for ( int i = 0; i < 4; i++ ){
                enableLed(i);
                lightUpLed(i);
                _delay_ms( 100 );
            }
            for ( int i = 0; i < 4; i++ ){
                enableLed(i);
                lightDownLed(i);
                _delay_ms( 100 );
            }

            enableBuzzer();
            playTone(1500.250,500);

            for (int i = 0; i <= 100; i++) {
                x = rand() % 9 + 1;
                y = rand() % 9 + 1;
                j = rand() % 9 + 1;
                z = rand() % 9 + 1;

                slot_number = (x * 1000) + (y * 100) + (j * 10) + z;
                writeNumberAndWait(slot_number, 100);
            }

            if (x == y && y == j && j == z) {
                coins_won = 500;
                coins = coins + coins_won;
                enableBuzzer();
                playTone(1500.250,1000);
            } else {
                coins_won = 0;
                coins = coins + coins_won;
            }

            enableBuzzer();
            playTone(1500.250,100);

            game = malloc(sizeof(GAME)); /* alocate memory for the game*/
            game->sequence = sequence;
            game->coins = coins;
            game->coins_won = coins_won;
            game->slot_number = slot_number;

            printf("\n%d :   %d coins available - %d coin bet - %d - %d coins won", sequence, coins, bet_coins, slot_number, coins_won);
        }
        writeNumberToSegment(0, x);
        writeNumberToSegment(1, y);
        writeNumberToSegment(2, j);
        writeNumberToSegment(3, z);
    }

    if (bit_is_clear(PINC, PC2)) {
        writeNumberAndWait(coins, 3000);
    }

    free(game);
}

void choose_slot_display() {
    if (slot_display <= MAX_SLOT_DISPLAY) {
        if (bit_is_clear(PINC, PC1)) {
            slot_display++;
            _delay_ms(200);
        }
    } else if (slot_display == MAX_SLOT_DISPLAY){
        slot_display--;
        _delay_ms(200);
    }

    for (int i = 0; i < slot_display; i++) {
        enableLed(i);
        lightUpLed(i);
    }
}

int main() {
    initUSART();
    initDisplay();
    initTimer0();

    initADC();
    ADCSRA |= (1 << ADSC);
    loop_until_bit_is_clear(ADCSRA, ADSC);
    uint16_t value = ADC;
    initUSART();
    srand(value); /* use the potentiometer's value as random seed */

    write_banner("one armed bandit");
    while (1) {
        choose_slot_display();
        gamble();

        if(coins >= MAX_COINS){
            write_banner("bank break");
            enableBuzzer();
            playTone(1500.250,100);
            stopTimer();
            printf("\n The game took %d seconds, and the player gambled %d times.", seconds, times_gambled);
            break;
        }

        if(coins == 0){
            write_banner("you lost");
            enableBuzzer();
            playTone(1500.250,100);
            stopTimer();
            printf("\n The game took %d seconds, and the player gambled %d times.", seconds, times_gambled);
            break;
        }

        if(coins <= 4){
            writeNumberToSegment(0, coins);
        }
    }

    return 0;
}
