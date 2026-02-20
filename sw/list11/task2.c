// 345615, Olaf Surgut
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>

#include "uart.h"

#define ADC_CH_POT  0
#define ADC_CH_NODE 1
#define VCC_MV 5000UL

typedef enum { ADC_IDLE=0, ADC_MEAS_ON, ADC_MEAS_OFF, ADC_POT } adc_state_t;

static volatile adc_state_t g_adc_state = ADC_IDLE;
static volatile uint16_t g_v_on_mv = 0;
static volatile uint16_t g_v_off_mv = 0;
static volatile uint8_t  g_new_pair = 0;


void adc_init() {
    ADMUX  = _BV(REFS0); // AVcc
    ADCSRA = _BV(ADEN) | // enable adc
             _BV(ADIE) | // enable interrupt
             _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0); // 128
    DIDR0  = _BV(ADC0D) | _BV(ADC1D);
}

void timer1_pwm_init_pf_correct_1khz() {
    DDRB |= _BV(DDB1); // OC1A output

    TCCR1A = _BV(COM1A1);
    TCCR1B = _BV(WGM13) | _BV(CS11); // prescaler 8

    ICR1  = 1000; // 1 kHz
    OCR1A = 0;

    // OVF (BOTTOM) + CAPT (TOP)
    TIMSK1 = _BV(TOIE1) | _BV(ICIE1);
}

// bottom, ocr1a = 1000
ISR(TIMER1_OVF_vect) {
    // BOTTOM: middle of ON time
    if (!(ADCSRA & _BV(ADSC))) {
        ADMUX |= 1;
        g_adc_state = ADC_MEAS_ON;
        ADCSRA |= _BV(ADSC);
    }
}

ISR(TIMER1_CAPT_vect) {
    // TOP: middle of OFF time
    if (!(ADCSRA & _BV(ADSC))) {
        g_adc_state = ADC_MEAS_OFF;
        ADMUX |= 1;
        ADCSRA |= _BV(ADSC);
    }
}

ISR(ADC_vect) {
    uint16_t raw = ADC;

    switch (g_adc_state) {
        case ADC_MEAS_ON:
            g_v_on_mv = (uint16_t)((uint32_t)raw * VCC_MV / 1023UL);
            g_adc_state = ADC_POT;
            ADMUX &= ~1;
            ADCSRA |= _BV(ADSC);
            break;

        case ADC_MEAS_OFF:
            g_v_off_mv = (uint16_t)((uint32_t)raw * VCC_MV / 1023UL);
            g_new_pair = 1;
            g_adc_state = ADC_IDLE;
            break;

        case ADC_POT: {
            uint16_t pot = raw;
            uint16_t duty = (uint16_t)((uint32_t)pot * (uint32_t)ICR1 / 1023UL);
            OCR1A = duty;
            g_adc_state = ADC_IDLE;
            break;
        }

        default:
            g_adc_state = ADC_IDLE;
            break;
    }
}

int main(void) {
    uart_init();
    adc_init();
    timer1_pwm_init_pf_correct_1khz();

    sei();

    uint16_t decim = 0;

    for (;;) {
        if (g_new_pair) {
            uint16_t v_on, v_off;

            cli();
            v_on  = g_v_on_mv;
            v_off = g_v_off_mv;
            g_new_pair = 0;
            sei();

            // Print ~20 Hz to not flood UART
            if (++decim >= 50) {
                decim = 0;

                uint16_t v_emf = (v_off <= (uint16_t)VCC_MV) ? (uint16_t)(VCC_MV - v_off) : 0;

                uart_puts("U_open=");
                uart_put_u16(v_on);
                uart_puts("mV  U_close=");
                uart_put_u16(v_emf);
                uart_puts("mV\r\n");
            }
        }
    }
}
