#ifdef __AVR__
#include <stdio.h>
#include <avr/io.h>
#include "SPI.h"
#include "errors.h"

char SPI_init() {
  // SS Output
  SPI_SS_DDR |= (1 << SPI_SS);
  SPI_SS_0_DDR |= (1 << SPI_SS_0);

  // SS Set High
  // (not selected)
  SPI_SS_PORT |= (1 << SPI_SS);
  SPI_SS_0_PORT |= (1 << SPI_SS_0);

  // MOSI output
  SPI_MOSI_DDR |= (1 << SPI_MOSI);

  // MISO pullup
  SPI_MISO_PORT |= (1 << SPI_MISO);

  // SCK output
  SPI_SCK_DDR |= (1 << SPI_SCK);


  /* SPCR =
   * (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPR1)|(0<<DORD)|(1<<CPOL)|(1<<CPHA); */


  // MSB First
  SPCR &= ~(1 << DORD);

  // Trailing (Rising) Edge
  SPCR |= (1 << CPOL); //
  SPCR |= (1 << CPHA); //

  // Clock / 64
  SPCR |= (1 << SPR1);
  SPCR |= (1 << SPR0);

  // Master
  SPCR |= (1 << MSTR);
  // Enable
  SPCR |= (1 << SPE);

  /* return SPCR; */
  return 0;
}

void SPI_tradeByte(uint8_t byte) {
  SPDR = byte;                       /* SPI starts sending immediately */
  loop_until_bit_is_set(SPSR, SPIF); /* wait until done */
                                     /* SPDR now contains the received byte */
  if (LOG_LEVEL >= LOG_LEVEL_DEBUG) {
    printf("%sSPI tradeByte(" BYTE_TO_BINARY_PATTERN
           "/0x%02hhx)=" BYTE_TO_BINARY_PATTERN "/0x%02hhx\n",
           LOG_INDENT, BYTE_TO_BINARY(byte), byte, BYTE_TO_BINARY(SPDR), SPDR);
  }
}
#endif //__AVR__
