// 345615, Olaf Surgut
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

void adc_init() {
    ADMUX  = _BV(REFS0);
    ADCSRA = _BV(ADEN)  | // enable adc
             _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0); // prescaler 128
    DIDR0  = _BV(ADC0D); // digital input disable
}

void pwm_init() {
    DDRB |= _BV(DDB1); // OC1A = PB1 output

    // fast pwm 8-bit WGM12=1, WGM10=1
    // non-inverting OC1A: COM1A1=1
    TCCR1A = _BV(COM1A1) | _BV(WGM10);
    TCCR1B = _BV(WGM12) | _BV(CS11) | _BV(CS10); // prescaler 64
    // 16e6 / (64*256)=976.5625 hz
    OCR1A = 0;
}

int main() {
    adc_init();
    pwm_init();

    while (1) {
        ADCSRA |= _BV(ADSC);
        while (ADCSRA & _BV(ADSC));
        OCR1A = (uint8_t)(ADC >> 2);
        _delay_ms(10);
    }
}
