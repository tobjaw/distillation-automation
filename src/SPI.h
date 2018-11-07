#include <avr/io.h>

/* #define PORT_SPI    PORTB */
/* #define DDR_SPI     DDRB */
/* #define DD_MISO     DDB4 */
/* #define DD_MOSI     DDB3 */
/* #define DD_SS       DDB2 */
/* #define DD_SCK      DDB5 */

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

char SPI_init();
void SPI_tradeByte(uint8_t byte);

#define SPI_SLAVE_0_SELECT SPI_SS_0_PORT &= ~(1 << SPI_SS_0)
#define SPI_SLAVE_0_DESELECT SPI_SS_0_PORT |= (1 << SPI_SS_0)
