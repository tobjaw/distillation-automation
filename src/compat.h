#ifndef COMPAT_H
#define COMPAT_H

#ifdef __AVR__
#include <avr/interrupt.h>
#endif // __AVR__

void _sleep(int time);
void _stdout_setup();
long unsigned elapsedTime;
void _clock_setup();


#endif /* COMPAT_H */
