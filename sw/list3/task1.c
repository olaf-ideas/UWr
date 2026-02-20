// 345615, Olaf Surgut
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>

#include "music.h"

#define BUZZER PC0
#define BUZZER_DDR DDRC
#define BUZZER_PORT PORTC

void play(uint16_t freq, uint16_t dur) {
	if (freq == 0) {
		for (uint32_t j = 0; j < (dur / 10); j++)
			_delay_ms(10);
		return;
	}

	uint32_t p_us = 1000000UL / freq;
	uint32_t half_us = p_us / 2;
	uint32_t cycles = (uint32_t) freq * dur / 1000UL;

	for (uint32_t i = 0; i < cycles; i++) {
		BUZZER_PORT = _BV(BUZZER);
		for (uint32_t j = 0; j < (half_us / 10); j++)
				_delay_loop_2(5);
		BUZZER_PORT = 0;
		for (uint32_t j = 0; j < (half_us / 10); j++)
				_delay_loop_2(5);
	}
}

int main() {
	BUZZER_DDR |= _BV(BUZZER);

	while (1) {
		for (int i = 0; i < MELODY_LEN; i++) {
			Note buf;
			memcpy_P(&buf, melody + i, 4);	
			play(buf.freq, buf.dur);
			_delay_ms(60);
		}
		_delay_ms(1000);
	}

	return 0;
}
