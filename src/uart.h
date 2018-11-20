#ifndef UART_H
#define UART_H
#ifdef __AVR__

#include <stdio.h>

int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);

void uart_init(void);

#endif // __AVR__
#endif /* UART_H */
