// 345615, Olaf Surgut
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "uart.h"

#define HEATER_DDR  DDRB
#define HEATER_PORT PORTB
#define HEATER_PIN  PB5

void adc_init() {
    ADMUX  = (1u << REFS1) | (1u << REFS0); // 1.1V, ADC0
    ADCSRA = (1u << ADEN)  | (1u << ADPS2) | (1u << ADPS1) | (1u << ADPS0); // /128
    DIDR0 |= (1u << ADC0D);

    _delay_ms(5);

    ADCSRA |= (1u << ADSC);
    while (ADCSRA & (1u << ADSC)) {}
    (void)ADC;
}

uint16_t adc_read(uint8_t ch) {
    ADMUX = (ADMUX & 0xF0) | (ch & 0x0F) | (1u << REFS1) | (1u << REFS0);
    ADCSRA |= (1u << ADSC);
    while (ADCSRA & (1u << ADSC)) {}
    return ADC;
}

int16_t read_temp_x10() {
    uint32_t acc = 0;
    const uint8_t n = 16;
    for (uint8_t i = 0; i < n; i++) acc += adc_read(0);

    uint16_t adc = (uint16_t)(acc / n);

    int32_t vout_mv   = (int32_t)adc * 1100L / 1023;
    int32_t temp_x10  = vout_mv - 500L;

    return (int16_t)temp_x10;
}

void heater_init() {
    HEATER_DDR  |= (1u << HEATER_PIN);
    HEATER_PORT &= ~(1u << HEATER_PIN);
}

void heater_set(bool on) {
    if (on) HEATER_PORT |=  (1u << HEATER_PIN);
    else    HEATER_PORT &= ~(1u << HEATER_PIN);
}

int uart_putchar_stdio(char c, FILE *stream) {
    if (c == '\n') uart_transmit('\r');
    uart_transmit(c);
    return 0;
}

FILE uart_out = FDEV_SETUP_STREAM(uart_putchar_stdio, NULL, _FDEV_SETUP_WRITE);

int main() {
    uart_init();
    stdout = &uart_out;

    adc_init();
    heater_init();

    int16_t setpoint_x10 = 380;
    int16_t hyst_x10 = 10;
    bool heater_on = false;

    while (1) {
        int16_t temp_x10 = read_temp_x10();

        if (heater_on) {
            if (temp_x10 >= setpoint_x10) {
                heater_on = false;
                heater_set(false);
            }
        } else {
            if (temp_x10 <= (int16_t)(setpoint_x10 - hyst_x10)) {
                heater_on = true;
                heater_set(true);
            }
        }

        printf("T=%.1fC, zad=%1.fC, grzalka=%s\n",
            read_temp_x10()/10.0, setpoint_x10/10.0, heater_on ? "ON" : "OFF");

        _delay_ms(100);
    }
}
