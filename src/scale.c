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

uint8_t scale_isParity(char word[6]) {
  int i;
  int res = hexToDec(word[0]);
  for (i = 1; i < 4; i++) {
    res ^= hexToDec(word[i]);
  }
  return (res + 48 == word[4]);
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
  static float weight = -1.0;

  if (SCALE_USART_HAS_DATA) {

    // read word
    for (i = 0; i < 6; i++)
      word[i] = receiveByte();

    // word is garbage, try again
    while (!scale_isParity(word)) {
      retries += 1;

      while (receiveByte() != 0x00) {
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

    weight = scale_wordToFloat(word);
  }

  return weight;
}

#endif //__AVR__
