// 345615, Olaf Surgut
#include <avr/io.h>
#include <util/delay.h>

#define SEGMENT_DDR DDRD
#define SEGMENT_PORT PORTD

const int pins[] = {
	PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7
};

const int digits[] = {
	0x3f,0x6,0x5b,0x4f,0x66,0x6d,0x7d,0x7,0x7f,0x6f
};

int off[10];

int main() {
  UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);

  for (int i = 0; i < 8; i++)
   SEGMENT_DDR |= _BV(pins[i]);
  
  for (int i = 0; i <= 9; i++) {
		off[i] = 0;
		for (int j = 0; j < 8; j++)
			if ((digits[i] >> j & 1) == 0)
				off[i] |= _BV(pins[j]);
  }

  while (1) {
    for (int i = 0; i <= 9; i++) {
			SEGMENT_PORT = off[i];
			_delay_ms(1000);
  	}
  }
}
