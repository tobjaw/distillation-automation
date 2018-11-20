/**
 * @file  main.h
 */


extern const int TEMPERATURE_MIN;
extern const int TEMPERATURE_MAX;

#ifdef __AVR__
#define _log(f_, ...) printf("%08lu," f_ "\n", elapsedTime, ##__VA_ARGS__)
#endif //__AVR__
#ifdef __x86_64
#define _log(f_, ...) printf("%08lu," f_ "\n", 0L, ##__VA_ARGS__)
#endif // __x86_64
