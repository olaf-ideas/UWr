// 345615, Olaf Surgut
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define BTN_PORT PORTA
#define BTN_PIN PINA
#define BTN PA7

#define LED_DDR  DDRB
#define LED_PORT PORTB
#define LED PB2

#define QUEUE_LEN 100

uint8_t queue[QUEUE_LEN];
uint8_t last = 0;

void io_init() {
    BTN_PORT |= _BV(BTN);
    LED_DDR  |= _BV(LED);
}

void timer0_init() {
    TCCR0A = _BV(WGM01); // CTC
    TCCR0B = _BV(CS01) | _BV(CS00); // /64 prescaler
    OCR0A = 155; // 100 hz = 8e6 / (2 * 64 * (1 + 155.25))
    TIMSK0 |= _BV(OCIE0A); // enable interrupt
}

ISR(TIM0_COMPA_vect) {
    LED_PORT = queue[last] << LED;

    uint8_t pressed = (BTN_PIN & _BV(BTN)) ? 0 : 1;
    queue[last] = pressed;

    last++;
    if (last == QUEUE_LEN) {
        last = 0;
    }
}

int main() {
    io_init();
    timer0_init();

    set_sleep_mode(SLEEP_MODE_IDLE);
    sei();

    while (1) {
        sleep_mode();
    }

    return 0;
}
