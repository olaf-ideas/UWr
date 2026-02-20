// 345615, Olaf Surgut
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "pid.h"
// #include "uart.h"

#define PWM_TOP     1023U //976 Hz
#define ADC_MOTOR   1
#define ADC_POT     0

#define PID_P       128
#define PID_I       4
#define PID_D       50

#define PWM_DDR   DDRB
#define PWM_BIT   PB1

volatile uint16_t adc_motor = 1023;
volatile uint16_t adc_pot = 0;
volatile int16_t speed = 0;
volatile uint8_t turn = 0;

pidData_t pid;

void pwm_init() {
  PWM_DDR |= (1 << PWM_BIT);

  ICR1  = PWM_TOP;
  OCR1A = 0;
  OCR1B = PWM_TOP;

  // P&F Correct PWM TOP=ICR1
  TCCR1A = (1 << COM1A1); 
  TCCR1B = (1 << WGM13) | (1 << CS11); // preskaler 8

  TIMSK1 |= (1 << OCIE1B); // Compare Match B interrupt
}

void adc_init() {
  ADMUX  = (1 << REFS0) | ADC_MOTOR; // avcc
  ADCSRA = (1 << ADEN)  | (1 << ADIE) |
           (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); /* preskaler 128 */

  DIDR0 |= (1 << ADC0D) | (1 << ADC1D);
}

ISR(TIMER1_COMPB_vect) {
  if (ADCSRA & (1 << ADSC)) return;
  ADMUX = (ADMUX & 0xF0) | ADC_MOTOR;
  ADCSRA |= (1 << ADSC);
  turn = 1;
}

ISR(ADC_vect) {
  uint16_t v = ADC;

  if (turn) {
    turn = 0;
    adc_motor = v;

    ADMUX = (ADMUX & 0xF0) | ADC_POT;
    ADCSRA |= (1 << ADSC);
    return;
  }

  adc_pot = v;

  // if (adc_pot < 5) {
  //   // reset integral
  //   pid_Reset_Integrator(&pid);
  //   OCR1A = 0;
  //   return;
  // }

  int16_t speed_raw = (int16_t)(1023U - adc_motor);

  speed += (speed_raw - speed) >> 3;

  int16_t u = pid_Controller(adc_pot, speed, &pid);

  if (u < 0) {
    u = 0;
  }
  if (u > 1022U) {
    u = 1022U;
  }
  OCR1A = u;
}

int main() {
  // uart_init();
  pid_Init(PID_P, PID_I, PID_D, &pid);

  pwm_init();
  adc_init();

  // set_sleep_mode(SLEEP_MODE_IDLE);

  sei();

  while (1) {
    // sleep_mode();
  }
}
