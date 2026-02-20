// 345615, Olaf Surgut
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "dzwiek.h"

void spi_init() {
	DDRB |= _BV(PB3) | _BV(PB5) | _BV(PB2);
	PORTB |= _BV(PB2);
	SPCR |= _BV(SPE) | _BV(MSTR) | _BV(SPR0);
	SPSR = 0;
}

uint8_t spi_transfer(uint8_t data) {
	SPDR = data;
	while (!(SPSR & _BV(SPIF)));
	SPSR |= _BV(SPIF);
	return SPDR;
}

void timer1_init() {
	TCCR1A = 0;
	TCCR1B = _BV(WGM12); // CTC A
	OCR1A = 249;
	TCCR1B |= _BV(CS11); // prescaler 8
	TIMSK1 |= _BV(OCIE1A); // interrupt
}

volatile uint16_t index = 0;

ISR(TIMER1_COMPA_vect) {
	uint8_t sample = pgm_read_byte(&dzwiek_raw[index]);

	uint16_t packet = (1UL << 12) | (1UL << 13) | (((uint16_t) sample) << 4);
	PORTB &= ~_BV(PB2);
	spi_transfer(packet >> 8);
	spi_transfer(packet & 0xff);
	PORTB |= _BV(PB2);

	if (index == 0) {
		index = dzwiek_raw_len;
	}
	index--;
	/*
	if (++index == dzwiek_raw_len) {
		index = 0;
	}
	*/
}

int main() {
	spi_init();
	timer1_init();

	set_sleep_mode(SLEEP_MODE_IDLE);

	sei();

	while (1) {
		sleep_mode();
	}

	return 0;
}
