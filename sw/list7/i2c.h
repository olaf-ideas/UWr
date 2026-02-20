#include <inttypes.h>
#include <avr/io.h>

void i2cInit();
void i2cWaitForComplete();
void i2cStart();
void i2cStop();
void i2cReset();
uint8_t i2cReadAck();
uint8_t i2cReadNoAck();
void i2cSend(uint8_t data);