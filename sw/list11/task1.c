// 345615, Olaf Surgut
#include <avr/io.h>
#include <util/delay.h>

void adc_init() {
  ADMUX = (1 << REFS0); // ref AVcc
  ADCSRA = (1 << ADEN) | // adc on
           (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // prescaler 128
}

uint16_t adc_read() {
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC));
  return ADC;
}

void pwm_init() {
  DDRB |= (1 << PB1); // OC1A out

  TCCR1A = (1 << COM1A1); // OC1A non-inverting
  TCCR1B = (1 << WGM13) | (1 << WGM12); // Fast PWM, TOP = ICR1
  TCCR1A |= (1 << WGM11);

  TCCR1B |= (1 << CS11); // prescaler 8
  ICR1 = 1999; // ~1 kHz
  OCR1A = 0;
}

int main() {
    uint16_t adc;

    adc_init();
    pwm_init();

    for (;;) {
        adc = adc_read();
        OCR1A = (uint32_t) adc * ICR1 / 1023;
        _delay_ms(10);
    }

    return 0;
}
