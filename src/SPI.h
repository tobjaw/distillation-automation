/**
 * @file SPI.h
 *
 * Low level abstractions over SPI communication.
 */

#include <avr/io.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)                                                   \
  (byte & 0x80 ? '1' : '0'), (byte & 0x40 ? '1' : '0'),                        \
      (byte & 0x20 ? '1' : '0'), (byte & 0x10 ? '1' : '0'),                    \
      (byte & 0x08 ? '1' : '0'), (byte & 0x04 ? '1' : '0'),                    \
      (byte & 0x02 ? '1' : '0'), (byte & 0x01 ? '1' : '0')

#define SPI_SS PB0
#define SPI_SS_PORT PORTB
#define SPI_SS_PIN PINB
#define SPI_SS_DDR DDRB

#define SPI_SS_0 PG0
#define SPI_SS_0_PORT PORTG
#define SPI_SS_0_PIN PING
#define SPI_SS_0_DDR DDRG

#define SPI_SS_2 PF2
#define SPI_SS_2_PORT PORTF
#define SPI_SS_2_PIN PINF
#define SPI_SS_2_DDR DDRF

#define SPI_MOSI PB2
#define SPI_MOSI_PORT PORTB
#define SPI_MOSI_PIN PINB
#define SPI_MOSI_DDR DDRB

#define SPI_MISO PB3
#define SPI_MISO_PORT PORTB
#define SPI_MISO_PIN PINB
#define SPI_MISO_DDR DDRB

#define SPI_SCK PB1
#define SPI_SCK_PORT PORTB
#define SPI_SCK_PIN PINB
#define SPI_SCK_DDR DDRB

#define SPI_SLAVE_0_SELECT SPI_SS_0_PORT &= ~(1 << SPI_SS_0)
#define SPI_SLAVE_0_DESELECT SPI_SS_0_PORT |= (1 << SPI_SS_0)

#define SPI_SLAVE_2_SELECT SPI_SS_2_PORT &= ~(1 << SPI_SS_2)
#define SPI_SLAVE_2_DESELECT SPI_SS_2_PORT |= (1 << SPI_SS_2)

/**
 * Init SPI communication
 *
 * @return  0
 */
char SPI_init();

/**
 * Trade a single byte with current SPI slave
 *
 * @param  byte single byte to trade
 */
void SPI_tradeByte(uint8_t byte);
