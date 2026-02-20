// 345615, Olaf Surgut

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "uart.h"

#define R_FIXED 10000UL

void adc_init() {
	ADMUX = _BV(REFS0);
	DIDR0 = _BV(ADC0D);
	ADCSRA = _BV(ADEN) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}

volatile uint32_t last_adc = 0;
ISR(ADC_vect) {
	last_adc = ADC;
}

void int0_init() {
	PORTD |= _BV(PD2);
	EICRA |= _BV(ISC01); // falling edge
	EIMSK |= _BV(INT0);
}

ISR(INT0_vect) {
	ADCSRA |= _BV(ADSC);
}

int main() {
	uart_init();

	adc_init();
	int0_init();

	sei();

	char buf[100];
	while (1) {
		uint32_t R = 0;
		
		cli();
		if (last_adc != 0) {
			R = R_FIXED * (1023UL - last_adc) / last_adc;
		}
		sei();

		sprintf(buf, "Last record: %"PRIu32" ohm\r\n", R);
		uart_puts(buf);
		_delay_ms(1000);
	}

	return 0;
}
