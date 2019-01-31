/**
 * @file main.h
 *
 * Main entry point into distillation-automation software
 */

#ifndef MAIN_H
#define MAIN_H

#include "compat.h"

#ifdef __AVR__
#define _log(f_, ...) printf("%08lu," f_ "\n", elapsedTime, ##__VA_ARGS__)
#endif //__AVR__
#ifdef __x86_64
#define _log(f_, ...) printf("%08lu," f_ "\n", 0L, ##__VA_ARGS__)
#endif // __x86_64

/**
 * Print basic usage instructions for this software.
 */
void program_info(void);

#endif /* MAIN_H */
