// 345615, Olaf Surgut
#include <avr/io.h>
#include <avr/interrupt.h>

#include "uart.h"

#include <math.h>

void adc_init() {
	ADMUX = _BV(REFS0) | _BV(MUX0);
	ADCSRA = _BV(ADEN) | _BV(ADIE)
				 | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
	DIDR0 |= _BV(ADC1D);
}

void timer1_init() {
	TCCR1A = 0;
	TCCR1B = _BV(WGM12);
	OCR1A = 249; // 16e6 / (8 * 8000) - 1;
	TCCR1B |= _BV(CS11);
	TIMSK1 |= _BV(OCIE1A);
}

#define SAMPLE_NB 512

volatile uint16_t samples[SAMPLE_NB] = {0};
volatile uint16_t sample_idx = 0;
volatile uint8_t block_ready = 0;

ISR(TIMER1_COMPA_vect) {
	ADCSRA |= _BV(ADSC);
}

ISR(ADC_vect) {
	samples[sample_idx++] = ADC;
	if (sample_idx == SAMPLE_NB) {
		sample_idx = 0;
		block_ready = 1;
	}
}

FILE uart_file;

int main() {
	uart_init();
	fdev_setup_stream(&uart_file, uart_transmit, NULL, _FDEV_SETUP_WRITE);
	stdout = &uart_file;

	timer1_init();
	adc_init();

	sei();

	for (;;) {
		if (block_ready) {
			cli();
	
			block_ready = 0;

			int32_t sum = 0;
			for (uint16_t i = 0; i < SAMPLE_NB; i++) {
				int32_t diff = (int32_t) samples[i] - 512; // 512 <=> ~2.5V		
				sum += diff * diff;
			}

			sei();

			float x = sqrtf(sum / SAMPLE_NB);

			if (x < 1E-6)
				x = 1E-6;

			// peak = rms * sqrt(2)
			float x_ref = 512 / 1.4142135623730951;

			// printf("x: %.2f x_ref: %.2f\r\n", x, x_ref);
			// float dBFS = 20.0 * log10f(x / x_ref);
			float dBFS = 20.0 * (log10f(x) - 2.55875496314384);		
			printf("%.2f\n", dBFS);
		}
	}

	return 0;
}
