/**
 * @file uart.h
 *
 * Serial communication between hardware and host computer.
 */

#ifndef UART_H
#define UART_H
#ifdef __AVR__
#include <stdio.h>

/**
 * Output a single char
 *
 * Output a single char to an output file descriptor.
 *
 * @param  c char to output
 * @param  *stream output file descriptor
 *
 */
int uart_putchar(char c, FILE *stream);

/**
 * Read a single char
 *
 * Read a single char from an input file descriptor.
 *
 * @param   *stream input file descriptor
 *
 * @return  read char
 */
int uart_getchar(FILE *stream);

/**
 * Setup UART communication with host computer
 *
 * Uses hardware USART Channel 0.
 */
void uart_init(void);

#endif // __AVR__
#endif /* UART_H */
