#include <stdint.h>

#ifndef SCALE_H
#define SCALE_H

#define SCALE_PIN_RX RXD1
#define SCALE_PIN_TX TXD1

#define SCALE_BAUD 9600

#define SCALE_USART_HAS_DATA bit_is_set(UCSR1A, RXC1)

int scale_USARTinit(void);
int scale_init(void);

void transmitByte(uint8_t data);
uint8_t receiveByte(void);

float scale_getWeight(void);
void scale_reset(void);


#endif /* SCALE_H */
