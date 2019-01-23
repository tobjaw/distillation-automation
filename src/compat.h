/**
 * @file compat.h
 *
 * Compatability functions for cross-platform support.
 */

#ifndef COMPAT_H
#define COMPAT_H

#ifdef __AVR__
#include <avr/interrupt.h>
#endif // __AVR__

void _sleep(int time);
void _stdout_setup();
volatile long unsigned elapsedTime;
void _clock_setup();

int PWM_SWITCHED;
void PWMGen();
volatile float PWM_COUNTER;

#endif /* COMPAT_H */
