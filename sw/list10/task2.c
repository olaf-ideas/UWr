// 345615, Olaf Surgut
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>

#include "hd44780.h"

#define LCD_COLS 16
#define PROGRESS_MAX 80  // 16 * 5

const int chars[6][8] = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10},
    {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18},
    {0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C},
    {0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E},
    {0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F}
};

void init_font() {
    for (int i = 0; i < 6; i++) {
        LCD_WriteCommand(HD44780_CGRAM_SET | (i << 3));
        for (int j = 0; j < 8; j++) {
            LCD_WriteData(chars[i][j]);
        }
    }
}

void draw(int value) {
    int full = value / 5;
    int rem  = value % 5;

    LCD_GoTo(0, 0);
    for (int i = 0; i < LCD_COLS; i++) {
        int c = 0;
        if (i < full) {
            c = 5;
        }
        else if (i == full && rem) {
            c = rem;
        }
        else {
            c = 0;
        }
        LCD_WriteData(c);
    }
}

int main() {
    LCD_Initialize();
    LCD_Clear();

    init_font();

    while (1) {
        for (int v = 0; v <= PROGRESS_MAX; v++) {
            draw(v);
            _delay_ms(80);
        }
        for (int v = PROGRESS_MAX; v >= 0; v--) {
            draw(v);
            _delay_ms(80);
        }
    }
}