/**
 * @file scale.c
 *
 * Communication with scale peripheral
 *
 * Uses USART channel 1.
 */

#ifdef __AVR__
#include <stdio.h>
#include <avr/io.h>
#include <util/setbaud.h>
#include <math.h>
#include "compat.h"

#include "scale.h"

int scale_USARTinit(void) {
  UBRR1H = UBRRH_VALUE;
  UBRR1L = UBRRL_VALUE;
#if USE_2X
  UCSR1A |= (1 << U2X0);
#else
  UCSR1A &= ~(1 << U2X0);
#endif
  /* Enable USART */
  UCSR1B = (1 << TXEN1) | (1 << RXEN1);
  /* 8 data bits, 1 stop bit */
  UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);

  return 0;
}

void transmitByte(uint8_t data) {
  loop_until_bit_is_set(UCSR1A, UDRE1);
  UDR1 = data;
}

uint8_t receiveByte(void) {
  loop_until_bit_is_set(UCSR1A, RXC1);
  return UDR1;
}

int hexToDec(char hex) { return ((hex & 0xF0) + (hex & 0x0F)); }

float scale_wordToFloat(char word[6]) {
  int i;
  float result = 0.0;
  for (i = 0; i < 4; i++) {
    result += ((word[i] - 48) * pow(10, 2 - i));
  }
  return roundf(result * 10) / 10;
}

uint8_t scale_word_validate(char word[6]) {
  // null byte
  if (word[6] != '\0')
    return 0;

  // parity byte
  int res = hexToDec(word[0]);
  for (int i = 1; i < 4; ++i) {
    res ^= hexToDec(word[i]);
  }
  if (!(res + 48 == word[4]))
    return 0;

  // digits
  for (int i = 0; i < 4; ++i) {
    if (word[i] < 48 || word[i] > 57)
      return 0;
  }

  // word valid
  return 1;
}

int scale_init(void) {
  transmitByte(0x00);
  _sleep(3000);

  transmitByte(0xFF);
  _sleep(3000);

  return 0;
}

float scale_getWeight(void) {
  uint8_t i;
  char word[6];
  int retries = 0;
  static float weight = 0;
  static float weight_filtered = 0;

  if (SCALE_USART_HAS_DATA) {

    // read word
    for (i = 0; i < 6; i++)
      word[i] = receiveByte();

    // word is garbage, try again
    while (!scale_word_validate(word)) {
      retries += 1;

      // try to find end of word
      while (receiveByte() != '\0') {
      }

      if (retries > 4) {
        // give up
        return -1.0;
      }
      if (retries > 3) {
        // last hope: reset scale
        scale_init();
      }

      for (i = 0; i < 6; i++)
        word[i] = receiveByte();
    }
    retries = 0;

    // simple EMS implementing a first-order lowpass filter
    // see:
    // https://en.wikipedia.org/wiki/Moving_average#Exponential_moving_average
    weight = scale_wordToFloat(word);
    weight_filtered = (SCALE_LOWPASS_ALPHA * weight) +
                      ((1 - SCALE_LOWPASS_ALPHA) * weight_filtered);
  }

  return weight_filtered;
}

#endif //__AVR__
