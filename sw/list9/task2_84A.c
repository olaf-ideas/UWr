// 345615, Olaf Surgut
#include <avr/io.h>
#include <util/delay.h>

#define LED_DDR   DDRB
#define LED_PORT  PORTB
#define LED       PB2

#define BTN_DDR   DDRA
#define BTN_PORT  PORTA
#define BTN_PIN   PINA
#define BTN       PA7

#define MOSI_DDR  DDRA
#define MOSI_PORT PORTA
#define MOSI      PA6

#define MISO_DDR  DDRA
#define MISO_PIN  PINA
#define MISO      PA5

#define SCK_DDR   DDRA
#define SCK_PORT  PORTA
#define SCK       PA4

uint8_t spi_transfer(uint8_t tx) {
    uint8_t rx = 0;

    for (uint8_t i = 0; i < 8; i++) {
        if (tx & 0x80) MOSI_PORT |= _BV(MOSI);
        else           MOSI_PORT &= ~_BV(MOSI);

        SCK_PORT |= _BV(SCK);
        rx <<= 1;
        rx |= (MISO_PIN >> MISO) & 1;
        SCK_PORT &= ~_BV(SCK);
        tx <<= 1;
    }

    return rx;
}

int main() {
    LED_DDR |= _BV(LED);
    BTN_PORT |= _BV(BTN);

    MOSI_DDR |= _BV(MOSI);
    SCK_DDR |= _BV(SCK);

    for (;;) {
        uint8_t tx = (BTN_PIN & _BV(BTN)) ? 0 : 1;
        uint8_t rx = spi_transfer(tx);

        if (rx)
            LED_PORT |= _BV(LED);
        else
            LED_PORT &= ~_BV(LED);

        _delay_ms(20);
    }
}