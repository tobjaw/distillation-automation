#ifndef UART_H
#define UART_H
#ifdef __AVR__

int uart_putchar(char c, FILE *stream);

void uart_init(void);

static FILE uart_output =
    FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

#endif // __AVR__
#endif /* UART_H */
