// 345615, Olaf Surgut
#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>

#define LED PB3
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BTN PB4
#define BTN_PIN PINB
#define BTN_PORT PORTB

#define QUEUE_LEN 100

int queue[QUEUE_LEN];

int main() {

	for (int i = 0; i < QUEUE_LEN; i++)
		queue[i] = 1;

	BTN_PORT |= 1 << BTN;
	LED_DDR  |= 1 << LED;

	int i = 0;
	while (1) {
		queue[i] = BTN_PIN >> BTN & 1;
		
		i++;
		if (i == QUEUE_LEN)
			i = 0;

		if (queue[i])
			LED_PORT &= ~(1 << LED);
		else
			LED_PORT |= 1 << LED;

		_delay_ms(10);
	}

	return 0;
}
