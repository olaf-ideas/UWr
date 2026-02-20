#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#include "uart.h"

void adc_init() {
	ADMUX  = _BV(REFS0) | 1;
	DIDR0  = _BV(ADC0D);
	ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}

uint16_t adc_read() {
	ADCSRA |= _BV(ADSC);
	while (!(ADCSRA & _BV(ADIF)));
	ADCSRA |= _BV(ADIF);
	return ADC;
}

uint16_t avg_adc_read(uint8_t ile) {
	uint16_t res = 0;
	for (int i = 0; i < ile; i++) {
		res += adc_read();
	}
	return res / ile;
}

int main() {
	DDRC = _BV(PC0);

	uart_init();
	adc_init();

	char buf[100];

	uint32_t left = 0;
	while (1) {
		if (left == 0) {
			PORTC &= ~_BV(PC0);
			left = 1ULL << ((avg_adc_read(10) >> 6) - 1);
			
			sprintf(buf, "%"PRId32"\n", left);
			uart_puts(buf);
		}
		else {
			PORTC |= _BV(PC0);
			left -= 1;
		}
		//_delay_ms(5);
	}

	return 0;
}
