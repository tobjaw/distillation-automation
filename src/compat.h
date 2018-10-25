#ifndef UTIL_H
#define UTIL_H

#ifdef __x86_64
#include <unistd.h>
#endif // __x86_64

#ifdef __AVR__
#include "uart.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
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

long unsigned elapsedTime = 0;
void _clock_setup() {
#ifdef __AVR__
  // set timer mode to CTC
  TCCR1A &= ~(1 << WGM10); // 0
  TCCR1A &= ~(1 << WGM11); // 0
  TCCR1B |= (1 << WGM12);  // 1

  // set prescaler to F_CPU / 64
  TCCR1B |= ((1 << CS10) | (1 << CS11));
  TCCR1B &= ~(1 << CS12);

  TIMSK1 |= (1 << OCIE1A);
  OCR1A = 24999;
  sei();
#endif //__AVR__
}

#ifdef __AVR__
ISR(TIMER1_COMPA_vect) { elapsedTime += 100; }
#endif //__AVR__
#endif /* UTIL_H */
