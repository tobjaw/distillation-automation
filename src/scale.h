/**
 * @file scale.h
 *
 * Communication with scale peripheral
 *
 * Uses USART channel 1.
 */

#include <stdint.h>

#ifndef SCALE_H
#define SCALE_H

#define SCALE_PIN_RX RXD1
#define SCALE_PIN_TX TXD1

#define SCALE_BAUD 9600

#define SCALE_USART_HAS_DATA bit_is_set(UCSR1A, RXC1)

/* controls the weight of previous vs. current measurement
 * lower values cause a slower response
 */
#define SCALE_LOWPASS_ALPHA 0.95

/**
 * Setup USART communication for scale peripheral
 *
 * @return   0 for success
 */
int scale_USARTinit(void);

/**
 * Init scale
 *
 * Power cycle the scale.
 *
 * @return   0 for success
 */
int scale_init(void);

/**
 * Transmit a byte to the scale
 *
 * @param   data byte to transfer
 */
void transmitByte(uint8_t data);

/**
 * Receive a byte from the scale
 *
 * @return  received byte
 */
uint8_t receiveByte(void);

/**
 * Get the current weight reading
 *
 * @return  weight in g
 */
float scale_getWeight(void);

/**
 * Convert hexadecimal char into decimal int.
 *
 * @param  hex hex value
 * @return decimal value
 */
int hexToDec(char hex);

/**
 * Convert scale word to float value.
 *
 * @param  word scale word
 * @return float value
 */
float scale_wordToFloat(char *word);

/**
 * Validate scale word.
 *
 * @param  word scale word
 * @return 1 if valid, 0 if invalid
 */
uint8_t scale_word_validate(char *word);

#endif /* SCALE_H */
