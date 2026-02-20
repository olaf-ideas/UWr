// 345615, Olaf Surgut
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "pins.h"

#define QUEUE_LEN 1000

uint8_t queue[QUEUE_LEN];
uint16_t last = 0;

void io_init() {
	BTN_PORT |= 1 << BTN;
	LED_DDR  |= 1 << LED;
}

void timer2_init() {
	TCCR2A = _BV(WGM21); // CTC
	TCCR2B = _BV(CS21) | _BV(CS20); // /32 prescaler
	OCR2A = 249; // 1khz = 16e6 / (2 * 32 * (1 + 249))
	TIMSK2 |= _BV(OCIE2A); // interrupt
}

ISR(TIMER2_COMPA_vect) {	
	LED_PORT = queue[last] << LED;

	uint8_t pressed = (BTN_PIN & _BV(BTN)) ? 0 : 1;
	queue[last++] = pressed;
	
	if (last == QUEUE_LEN)
		last = 0;
}

int main() {
	io_init();
	timer2_init();

	set_sleep_mode(SLEEP_MODE_IDLE);
	sei();

	while (1) {
		sleep_mode();
	}

	return 0;
}
