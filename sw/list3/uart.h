#include <avr/io.h>
#include <stdio.h>

#define BAUD 9600
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)

void uart_init() {
	UBRR0 = UBRR_VALUE;
	UCSR0A = 0;
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

void uart_putc(char c) {
	while (!(UCSR0A & _BV(UDRE0)));
	UDR0 = c;
}

void uart_puts(const char* s) {
	while (*s) uart_putc(*s++);
}
