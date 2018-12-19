#include "compat.h"


#ifdef __x86_64
#include <unistd.h>
#endif // __x86_64

#ifdef __AVR__
#include "uart.h"
#include <avr/io.h>
#include <util/delay.h>
#endif // __AVR__

volatile long unsigned elapsedTime = 0;

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
#endif //__AVR__
}

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


void PWMGen() {
#ifdef __AVR__
  // set Timer for PWM

  TCCR4A &= ~(1 << WGM40); // 0
  TCCR4A &= ~(1 << WGM41); // 0
  TCCR4B |= (1 << WGM42);  // 1
                           // TCCR4B &= ~(1 << WGM43); // 0

  // set prescaler to F_CPU / 256
  TCCR4B |= ((1 << CS40) | (1 << CS41));
  TCCR4B &= ~(1 << CS42);

  TIMSK4 |= (1 << OCIE4A);
  OCR4A = 24999; // 31249 for 0.5s
  sei();

#endif //__AVR__
}

#ifdef __AVR__
ISR(TIMER4_COMPA_vect) { // pwmcounter = 1;
  pwmcounter += 0.1;
  if (pwmcounter >= 16) {
    pwmcounter = 0;
    switched = 0;
  }
}
#endif //__AVR__
