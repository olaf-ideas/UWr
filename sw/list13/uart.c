#include <avr/interrupt.h>

#include "uart.h"

#define BAUD 9600
#define UBRR_VALUE ((F_CPU) / (16UL * BAUD) - 1)

void uart_init() {
	UBRR0 = UBRR_VALUE;
	UCSR0A = 0;
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

int uart_transmit(char data, FILE *stream) {
	(void) stream;
	while (!(UCSR0A & _BV(UDRE0))) { }
	UDR0 = data;
	return 0;
}
