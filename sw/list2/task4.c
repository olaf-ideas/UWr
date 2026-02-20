// 345615, Olaf Surgut
#include <avr/io.h>
#include <util/delay.h>

#define DISPLAY_DDR DDRD
#define DISPLAY_PORT PORTD

#define MUX_DDR DDRC
#define MUX_PORT PORTC

const uint8_t digits[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

int main() {
	UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);

	MUX_DDR |= 0x03;
	DISPLAY_DDR |= 0xff;

	int ones = 0;
	int tens = 0;
	while (1) {
		for (int i = 0; i < 100; i++) {
			MUX_PORT = 1 << 0;
			DISPLAY_PORT = digits[ones];
			_delay_ms(5);
				
			MUX_PORT = 1 << 1;
			DISPLAY_PORT = digits[tens];
			_delay_ms(5);
		}

		if (++ones == 10) {
			ones = 0;
			if (++tens == 6) {
				tens = 0;
			}
		}
	}

	return 0;
}
