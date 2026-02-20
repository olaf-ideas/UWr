// 345615, Olaf Surgut
#pragma once

#include <stdint.h>

#ifndef UART_BAUD
#define UART_BAUD 115200UL
#endif

void uart_init(void);
int uart_transmit(char);
int uart_receive();