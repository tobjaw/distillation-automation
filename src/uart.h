#ifndef UART_H
#define UART_H
#ifdef __AVR__

int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);

void uart_init(void);

static FILE uart_output =
  FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

static FILE uart_input =
  FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);


#endif // __AVR__
#endif /* UART_H */
