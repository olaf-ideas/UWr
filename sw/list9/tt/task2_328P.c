// 345615, Olaf Surgut
#include <avr/io.h>
#include <util/delay.h>

#define BTN_DDR  DDRD
#define BTN_PORT PORTD
#define BTN_PIN  PIND
#define BTN      PD2

#define LED_DDR  DDRD
#define LED_PORT PORTD
#define LED      PD4

void spi_init() {
    DDRB |= _BV(PB4);
    PORTB |= _BV(PB2);
    SPCR = _BV(SPE); // slave
}

int main() {
    LED_DDR |= _BV(LED);
    BTN_PORT |= _BV(BTN);

    spi_init();

    SPDR = (BTN_PIN & _BV(BTN)) ? 0 : 1;
    for (;;) {
        if (SPSR & _BV(SPIF)) {
            uint8_t rx = SPDR;
            if (rx)
                LED_PORT |= _BV(LED);
            else
                LED_PORT &= ~_BV(LED);
        
            SPDR = (BTN_PIN & _BV(BTN)) ? 0 : 1;
        }
    }
}