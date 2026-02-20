// 345615, Olaf Surgut
#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

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
  return UDR0;
}

FILE uart_file;

int main()
{
  uart_init();
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;

  while (1) {
	int8_t a8, b8;
	int16_t a16, b16;
	int32_t a32, b32;
	int64_t a64, b64;
	float af, bf;
  
	puts("int8:");
	scanf("%"SCNd8"%" SCNd8, &a8, &b8);
	printf("a+b=%"PRId8"\r\n", a8+b8);
	printf("a*b=%"PRId8"\r\n", a8*b8);
	printf("a/b=%"PRId8"\r\n", a8/b8);
	
	puts("int16:");
	scanf("%"SCNd16"%" SCNd16, &a16, &b16);
	printf("a+b=%"PRId16"\r\n", a16+b16);
	printf("a*b=%"PRId16"\r\n", a16*b16);
	printf("a/b=%"PRId16"\r\n", a16/b16);
	
	puts("int32:");
	scanf("%"SCNd32"%" SCNd32, &a32, &b32);
	printf("a+b=%"PRId32"\r\n", a32+b32);
	printf("a*b=%"PRId32"\r\n", a32*b32);
	printf("a/b=%"PRId32"\r\n", a32/b32);

	puts("int64:");
	scanf("%"SCNd32"%" SCNd32, &a64, &b64);
	printf("a+b=%"PRId32"\r\n", a64+b64);
	printf("a*b=%"PRId32"\r\n", a64*b64);
	printf("a/b=%"PRId32"\r\n", a64/b64);
	
	puts("float:");
	scanf("%f%f", &af, &bf);
	printf("a+b=%f\r\n", af+bf);
	printf("a*b=%f\r\n", af*bf);
	printf("a/b=%f\r\n", af/bf);
  }
}

