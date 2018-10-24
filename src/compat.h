#ifndef UTIL_H
#define UTIL_H

#ifdef __x86_64
#include <unistd.h>
#endif // __x86_64

#ifdef __AVR__
#include "uart.h"
#include <avr/io.h>
#include <util/delay.h>
#endif // __AVR__

void _sleep(int time) {
#ifdef __x86_64
  usleep(time * 1000);
#endif // __x86_64
#ifdef __AVR__
  while (time--) {
    _delay_ms(1);
  }
#endif //__AVR__
}

void _stdout_setup() {
#ifdef __AVR__
  uart_init();
  stdout = &uart_output;
#endif //__AVR__
}

#endif /* UTIL_H */
