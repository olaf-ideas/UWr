#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "i2c.h"

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

FILE uart_file;

void uart_init()
{
  UBRR0 = UBRR_VALUE;
  UCSR0A = 0;
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

int uart_transmit(char data, FILE *stream)
{
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
}

int uart_receive(FILE *stream)
{
  while (!(UCSR0A & _BV(RXC0)));
  uint8_t data = UDR0;
//   if (data == '\r')
//     uart_transmit('\n', stream);
  uart_transmit(data, stream);
  return data;
}

const uint8_t eeprom_addr = 0xa0;

void read_seq(uint16_t addr, uint8_t *buf, uint16_t len) {
    i2cStart();
    i2cSend(eeprom_addr | ((addr & 0x100) >> 7));
    i2cSend(addr & 0xff);

    i2cStart();
    i2cSend(eeprom_addr | 0x1 | ((addr & 0x100) >> 7));

    while (len--) {
        *(buf++) = (len ? i2cReadAck() : i2cReadNoAck());
    }
}

void write_seq(uint16_t addr, uint8_t *buf, uint16_t len) {
    while (len) {
        uint8_t left = 0x10 - (addr & 0xf);
        // printf("left: %d len: %d\n", left, len);
        if (len < left)
            left = len;

        i2cStart();
        i2cSend(eeprom_addr | ((addr & 0x100) >> 7));
        i2cSend(addr & 0xff);

        len -= left;
        while (left--) {
            // printf("Here, left: %d\n", left);
            i2cSend(*(buf++));
        }
        // printf("Hello len: %d\n", len);
        i2cStop();
    }
}

int main() {
    uart_init();
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;

    i2cInit();

    char line[100];
    uint8_t buf[512], tmp[512];

    while (1) {
        
        printf("$ ");
        if (!fgets(line, sizeof(line), stdin))
            continue;
        

        char* cmd = strtok(line, " \t\r\n");

        if (strcmp(cmd, "read") == 0) {
            char* addr_s = strtok(NULL, " \t\r\n");
            char* length_s = strtok(NULL, " \t\r\n");

            uint16_t addr = atol(addr_s);
            int length = (length_s ? atoi(length_s) : 1);
            read_seq(addr, buf, length);

            if (length_s) {
                uint8_t csum = 0;

                uint8_t ah = addr >> 8;
                uint8_t al = addr & 0xff;

                printf(":%02X%02X%02X%02X", length, ah, al, 0x00);

                csum += length;
                csum += ah;
                csum += al;
                csum += 0x00; // type
                for (uint8_t i = 0; i < length; i++) {
                    printf("%02X", buf[i]);
                    csum += buf[i];
                }
                csum = ~csum + 1;
                printf("%02X\r\n", csum);
            }
            else {
                printf("%02X\r\n", buf[0]);
            }
        }
        else if (strcmp(cmd, "write") == 0) {
            char* addr_s = strtok(NULL, " \t\r\n");
            char* value_s = strtok(NULL, " \t\r\n");
        
            if (!addr_s && !value_s) {
                while (1) {
                    if (!fgets(buf, sizeof(buf), stdin))
                        break;

                    if (buf[0] != ':') break;
                    
                    char* line = buf;

                    uint8_t length, ah, al, type, n;
                    sscanf(line, ":%02hhX%02hhX%02hhX%02hhX%n", &length, &ah, &al, &type, &n);
                    line += n;

                    // printf("buf: %s \r\n", buf);
                    // printf("n: %d\n", n);
                    // printf("> %d %d %d %d\r\n", length, ah, al, type);
                    uint8_t *data = tmp;
                    for (uint8_t i = 0; i < length; i++) {
                        sscanf(line, "%02hhX%n", data++, &n);
                        line += n;
                    }

                    uint16_t addr = (((uint16_t) ah) << 8) | al;
                    // printf("Here\n");
                    write_seq(addr, tmp, length);
                }
            }
            else {
                // printf("here\r\n");
                uint16_t addr = atol(addr_s);
                uint8_t value = atoi(value_s); 
                write_seq(addr, &value, 1);
            }
        }
        else {
            printf("Unknown command\r\n");
        }

        // scanf("%s%d", op, &addr);
        // printf("%s %d: ", op, addr);

        // if (strcmp(op, "read") == 0) {
            
        //     i2cStart();
        //     i2cSend(eeprom_addr | ((addr & 0x100) >> 7));
        //     i2cSend(addr & 0xff);

        //     i2cStart();
        //     i2cSend(eeprom_addr | 0x1 | ((addr & 0x100) >> 7));
            
        //     uint8_t data = i2cReadNoAck();
        //     i2cStop();

        //     printf("%x\r\n", data);
        // }
        // else
        // if (strcmp(op, "write") == 0) {
        //     int value;
        //     scanf("%d", &value);

        //     i2cStart();
        //     i2cSend(eeprom_addr | ((addr & 0x100) >> 7));
        //     i2cSend(addr & 0xff);
        //     i2cSend(value);
        //     i2cStop();

        //     printf("%d\r\n", value);
        // }
        // else {
        //     printf("Unknown: %s\r\n", op);
        // }
    }


    return 0;
}