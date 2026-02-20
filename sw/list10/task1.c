// 345615, Olaf Surgut
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#include "hd44780.h"
#include "uart.h"

#define LCD_COLS 16

char line0[LCD_COLS];
char line1[LCD_COLS];
uint8_t x = 0;

void lcd_do_draw(void) {
    LCD_GoTo(0, 0);
    for (uint8_t i = 0; i < LCD_COLS; i++) LCD_WriteData(line0[i]);

    LCD_GoTo(0, 1);
    for (uint8_t i = 0; i < LCD_COLS; i++) LCD_WriteData(line1[i]);

    LCD_GoTo(x, 1);
}

void lcd_put_char(char c) {
    if (c == '\r') return;

    if (c == '\n') {
        memcpy(line0, line1, LCD_COLS);
        for (uint8_t i = 0; i < LCD_COLS; i++) line1[i] = ' ';
        x = 0;
        lcd_do_draw();
        return;
    }

    if (c < 0x20) return;

    line1[x] = c;
    LCD_GoTo(x, 1);
    LCD_WriteData(c);

    x++;

    if (x >= LCD_COLS) {
        memcpy(line0, line1, LCD_COLS);
        for (uint8_t i = 0; i < LCD_COLS; i++) line1[i] = ' ';
        x = 0;
        lcd_do_draw();
    } else {
        LCD_GoTo(x, 1);
    }
}

int main() {
    uart_init();
    LCD_Initialize();
    LCD_Clear();

    LCD_WriteCommand(HD44780_DISPLAY_ONOFF |
                     HD44780_DISPLAY_ON   |
                     HD44780_CURSOR_ON    |
                     HD44780_CURSOR_NOBLINK);

    for (uint8_t i = 0; i < LCD_COLS; i++) {
        line0[i] = ' ';
        line1[i] = ' ';
    }
    x = 0;
    lcd_do_draw();

    while (1) {
        char c = uart_receive();
        uart_transmit(c);
        lcd_put_char(c);
    }
}
