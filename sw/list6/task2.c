// 345615, Olaf Surgut
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include <util/delay.h>

#define BAUD 9600
#define UBRR_VALUE (F_CPU/16/BAUD-1)

void uart_init() {
	UBRR0 = UBRR_VALUE;	
	UCSR0A = 0;
	UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); 
}

volatile uint8_t I[256], I_head = 0, I_tail = 0;
volatile uint8_t O[256], O_head = 0, O_tail = 0;

ISR(USART_UDRE_vect) {
	UDR0 = O[O_tail++];

	if (O_head == O_tail) {
		UCSR0B &= ~_BV(UDRIE0);
	}
}

ISR(USART_RX_vect) {
	if (I_head + 1 != I_tail) {
		I[I_head++] = UDR0;
	}
	else {
		volatile uint8_t empty = UDR0;
		(void) empty;
	}
}

void uart_transmit(uint8_t data) {
	while (O_head + 1 == O_tail) { }
	
	cli();
	O[O_head++] = data;
	UCSR0B |= _BV(UDRIE0);
	sei();
}

void uart_puts(char* s) {
	while (*s) {
		uart_transmit(*s++);
	}
}

uint8_t uart_receive() {
	while (I_head == I_tail) { }

	cli();
	uint8_t data = I[I_tail++];
	sei();
	
	return data;
}

int main() {
	uart_init();

	//set_sleep_mode(SLEEP_MODE_IDLE);
	sei();

	char buf[100];
	char input = uart_receive();

	while (1) {
		cli();
		sprintf(buf, "%d %d %d %d %c\r\n", I_tail, I_head, O_tail, O_head, input);
		sei();
		uart_puts(buf);
		_delay_ms(10);
	}

	return 0;
}
