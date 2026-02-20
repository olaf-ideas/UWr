// 345615, Olaf Surgut
#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>

#include "uart.h"

#define BTN PB4
#define BTN_PIN PINB
#define BTN_PORT PORTB

const unsigned char morse[26] = {0xa0,0x18,0x58,0x30,0x40,0x48,0x70,0x8,0x20,0xe8,0xb0,0x28,0xe0,0x60,0xf0,0x68,0xb8,0x50,0x10,0xc0,0x90,0x88,0xd0,0x98,0xd8,0x38};

#define QUEUE_LEN 1000

int queue[QUEUE_LEN];

int main() {
	uart_init();

	BTN_PORT |= 1 << BTN;

	int i = 0;
	int signal_length = 0;
	int last_signal = 1;
	
	while (1) {
		int current_signal = BTN_PIN >> BTN & 1;
		_delay_ms(10);
		current_signal |= BTN_PIN >> BTN & 1;

		if (current_signal == last_signal) {
			signal_lenght += 1;
		}
		else {
			queue[i++] = signal_legnth;
			last_signal = current_signal;
		
			if (i == QUEUE_LEN)
				i = 0;
		}

		if (last_signal == 1 && signal_length >= 500) {
			// start analyzing the signal
		}

		queue[i] = BTN_PIN >> BTN & 1;
		_delay_ms(10);
		queue[i] |= BTN_PIN >> BTN & 1;

		fputs(stdin, queue[i] + '0');	

		i++;
		if (i == QUEUE_LEN)
			i = 0;

		if (queue[i]) {
			
		}

		_delay_ms(90);
	}
	
	return 0;
}

int main() {
	uart_init();

	while (1) {

	}

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

