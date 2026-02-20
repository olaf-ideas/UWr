// 345615, Olaf Surgut
#include <avr/io.h>
#include <util/delay.h>

#define LINE_DDR DDRD
#define LINE_PORT PORTD

const int line[] = {
	PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7
};

int main() {
  UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);

  for (int i = 0; i < 8; i++)
   LINE_DDR |= _BV(line[i]);
  
  while (1) {
    for (int i = 0; i < 8; i++) {
			LINE_PORT = 1 << i;	
    	_delay_ms(20);
		}
    for (int i = 7; i >= 0; i--) {
			LINE_PORT |= _BV(line[i]);
    	_delay_ms(20);
    	LINE_PORT &= ~_BV(line[i]);
    	_delay_ms(20);
		}
  }
}
