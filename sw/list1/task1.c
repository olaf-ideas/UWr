// 345615, Olaf Surgut
#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

void uart_init()
{
  UBRR0 = UBRR_VALUE;
  UCSR0A = 0;
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

int uart_transmit(char data, FILE *stream)
{
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
}

int uart_receive(FILE *stream)
{
  while (!(UCSR0A & _BV(RXC0)));
  return UDR0;
}

FILE uart_file;

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

const unsigned char morse[26] = {0xa0,0x18,0x58,0x30,0x40,0x48,0x70,0x8,0x20,0xe8,0xb0,0x28,0xe0,0x60,0xf0,0x68,0xb8,0x50,0x10,0xc0,0x90,0x88,0xd0,0x98,0xd8,0x38};

int main()
{
  uart_init();
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  
  printf("Wpisz slowa, ktore mam przetlumaczyc na morsa!\r\n");
  LED_DDR |= _BV(LED);

  while(1) {
	char buf[64];
	scanf("%63s", buf);

	for (int i = 0; buf[i]; i++) {
		int val = morse[buf[i] - 'A'], start = 0;
		for (int j = 0; j < 8; j++) {
			if (start) {
				LED_PORT |= _BV(LED);
				if (val & 1)
					_delay_ms(100);
				else
					_delay_ms(300);
				LED_PORT &= ~_BV(LED);
				_delay_ms(100);
			}
			else
				start |= val & 1;
			val >>= 1;
		}
		_delay_ms(300);
	}
	_delay_ms(700);
  }
}

