// 345615, Olaf Surgut
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#include <util/delay.h>

void spi_init() {
	DDRB |= _BV(PB3) | _BV(PB5) | _BV(PB2) | _BV(PB1);
	PORTB &= ~_BV(PB2);
	PORTB &= ~_BV(PB1);
	SPCR |= _BV(SPE) | _BV(MSTR) | _BV(SPR1);
	SPSR = 0;
}

uint8_t spi_transfer(uint8_t data) {
	SPDR = data;
	while (!(SPSR & _BV(SPIF)));
	SPSR |= _BV(SPIF);
	return SPDR;
}

const uint8_t digits[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

int main() {
	spi_init();

	int d = 0;
	while (1) {

		spi_transfer(~digits[d]);
		PORTB |= _BV(PB1);
		_delay_us(1);
		PORTB &= ~_BV(PB1);

		d++;
		if (d == 10)
			d = 0;

		_delay_ms(1000);
	}

	return 0;
}
