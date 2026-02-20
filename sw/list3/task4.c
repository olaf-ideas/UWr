// 345615, Olaf Surgut
#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>

#include "uart.h"

void adc_init() {
	ADMUX = _BV(REFS0) | 0;
	DIDR0 = _BV(ADC0D);
	ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}

uint16_t adc_read() {
	ADCSRA |= _BV(ADSC);
	while (!(ADCSRA & _BV(ADIF)));
	ADCSRA |= _BV(ADIF);
	return ADC;
}

#define R_FIXED 2200.0f
#define R0 4700.0f
#define T0 298.15f // 25*C
#define B 3900.0f

int main() {
	uart_init();
	adc_init();
	
	char buf[100];

	while (1) {
		uint16_t adc = adc_read();
		float r = adc / (1023.0f - adc) * R_FIXED;
		float inv_t = (1.0f / T0) + (1.0f / B) * logf(r / R0);
		float T_kelvin = 1.0f / inv_t;
		float T_celcius = T_kelvin - 273.15f;

		sprintf(buf, "ADC=%u T=%f C\r\n", adc, T_celcius);
		uart_puts(buf);	
		_delay_ms(1000);
	}

	return 0;
}
