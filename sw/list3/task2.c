// 345615, Olaf Surgut
#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>

#include "uart.h"

#define LED PC0
#define LED_DDR DDRC
#define LED_PORT PORTC

void adc_init() {
	ADMUX  = _BV(REFS0) | 0b1110;
	ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}

uint16_t adc_read() {
	ADCSRA |= _BV(ADSC);
	while (!(ADCSRA & _BV(ADIF)));
	ADCSRA |= _BV(ADIF);
	return ADC;
}

int main() {
	uart_init();
	adc_init();
	LED_DDR |= _BV(LED);

	char buf[100];

	while (1) {
		LED_PORT ^= _BV(LED);

		if (LED_PORT)
			uart_puts("ON\n");
		else
			uart_puts("OFF\n");

		_delay_ms(3);

		for (int i = 0; i < 10; i++) {
			ADCSRA |= _BV(ADSC);
			while (!(ADCSRA & _BV(ADIF)));
			ADCSRA |= _BV(ADIF);

			uint16_t adc = ADC;
			float vcc = (1024 * 1.1f) / adc; // (uint32_t) 1024UL * 1100UL / adc;
		
			sprintf(buf, "VCC = %f\n", vcc);
			//sprintf(buf, "ADC= %"PRId32" VCC= %"PRId32".%"PRId32"\n", adc, vcc/1000, vcc%1000);
			uart_puts(buf);

			_delay_ms(100);
		}
	}

	return 0;
}
