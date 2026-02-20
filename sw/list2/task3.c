// 345615, Olaf Surgut
#include <avr/io.h>
#include <util/delay.h>

#define LINE_DDR DDRD
#define LINE_PORT PORTD

#define BTN_PIN PINB
#define BTN_PORT PORTB
#define BTN_MASK ((1 << 0) | (1 << 1) | (1 << 2))

int main() {
	UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);

	BTN_PORT |= BTN_MASK;
	LINE_DDR = 0xff;

	uint8_t index = 0;
	uint8_t prev_pressed = 0;

	while (1) {
		uint8_t pressed = BTN_PIN;
		_delay_ms(10);
		pressed |= BTN_PIN;
		pressed = BTN_MASK ^ (BTN_MASK & pressed);

		uint8_t clicked = pressed & (prev_pressed ^ pressed);
		
		index &= (clicked >> 0 & 1) - 1;
		index -= (clicked >> 1 & 1);
		index += (clicked >> 2 & 1);

		LINE_PORT = index ^ (index >> 1);
		prev_pressed = pressed;
	}

	return 0;
}
