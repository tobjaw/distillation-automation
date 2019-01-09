/**
 * @file AD7792.c
 *
 * Interface to external AD7792 AD converters.
 *
 * Requires populating the first and second hardware extension slot with a
 * AD7792. Each converter needs to be setup with two Pt1000 temperature sensors,
 * even though only the first is actually queried.
 *
 * @todo allow single temperature sensor configuration
 *
 * @todo fix temperature drop when heater is powered on
 */

#ifdef __AVR__
#include <stdio.h>
#include <util/delay.h>

#include "AD7792.h"
#include "SPI.h"
#include "errors.h"

void selectSlave(TempSlot slot) {
  if (slot == SLOT1) {
    SPI_SLAVE_0_SELECT;
  } else {
    SPI_SLAVE_2_SELECT;
  }
}

void deselectSlave(TempSlot slot) {
  if (slot == SLOT1) {
    SPI_SLAVE_0_DESELECT;
  } else {
    SPI_SLAVE_2_DESELECT;
  }
}

unsigned short AD7792_read(unsigned char addr, unsigned char length,
                           TempSlot slot) {
  static unsigned short result;
  static unsigned char reg;
  static unsigned char buf[2];
  static unsigned int i = 0;

  reg = AD7792_COMM_READ | addr;

  selectSlave(slot);

  SPI_tradeByte(reg);
  for (i = 0; i < length; i++) {
    if (i < length - 1) {
      SPI_tradeByte(reg);
    } else {
      SPI_tradeByte(0);
    }
    buf[i] = SPDR;
  }

  deselectSlave(slot);

  if (length == 1) {
    result = buf[0];
  } else if (length == 2) {
    result = (buf[0] << 8) | buf[1];
  }

  if (LOG_LEVEL >= LOG_LEVEL_INFO) {
    printf("%sAD7792  read(" BYTE_TO_BINARY_PATTERN ")=0x%04hhx\n", LOG_INDENT,
           BYTE_TO_BINARY(reg), result);
  }

  return result;
}


void AD7792_write(unsigned char addr, unsigned short data, unsigned int length,
                  TempSlot slot) {
  static unsigned char reg;
  static unsigned int i = 0;
  static unsigned char buf;

  reg = AD7792_COMM_WRITE | addr;

  selectSlave(slot);

  SPI_tradeByte(reg);
  for (i = length; i > 0; i--) {
    buf = ((data >> ((i - 1) * 8)) & 0xFF);
    SPI_tradeByte(buf);
  }

  deselectSlave(slot);

  if (LOG_LEVEL >= LOG_LEVEL_INFO) {
    printf("%sAD7792 write(" BYTE_TO_BINARY_PATTERN ")=0x%04hhx\n", LOG_INDENT,
           BYTE_TO_BINARY(reg), data);
  }
}


int AD7792_write_verify(unsigned char addr, unsigned short data,
                        unsigned int length, TempSlot slot) {
  static unsigned short buf;

  AD7792_write(addr, data, length, slot);
  buf = AD7792_read(addr, length, slot);

  return (buf != data);
}

void AD7792_waitReady(TempSlot slot) {
  while (AD7792_read(AD7792_COMM_ADDR(AD7792_REG_STATUS), 1, slot) &
         AD7792_STATUS_RDY) {
    _delay_ms(10);
  }
}

int AD7792_isConnected(TempSlot slot) {

  static unsigned char id;
  static unsigned int isConnected;
  id = AD7792_read(AD7792_COMM_ADDR(AD7792_REG_ID), 1, slot);
  isConnected = ((id & AD7792_MODEL_MASK) == AD7792_MODEL_ID);
  return isConnected;
}


void AD7792_reset(TempSlot slot) {

  selectSlave(slot);

  SPI_tradeByte(0xFF);
  SPI_tradeByte(0xFF);
  SPI_tradeByte(0xFF);
  SPI_tradeByte(0xFF);

  deselectSlave(slot);

  _delay_ms(10);
}


unsigned char AD7792_init(TempSlot slot) {

  /* reset device */
  AD7792_reset(slot);

  /* check for connection */
  if (!AD7792_isConnected(slot)) {
    return ERROR_AD7792_CONNECTION;
  }

  /* config register setup */
  unsigned short config = 0x0000;
  config |= (AD7792_CONFIG_POL_UNIPOLAR);
  config |= (AD7792_CONFIG_GAIN_SEL(AD7792_CONFIG_GAIN_LEVEL_2));
  if (AD7792_write_verify(AD7792_COMM_ADDR(AD7792_REG_CONFIG), config, 2,
                          slot)) {
    return ERROR_AD7792_CONFIGURATION;
  }

  /* IO register setup */
  unsigned char io = 0x00;
  io |= AD7792_IO_CURRENT_SRC_SEL(AD7792_IO_CURRENT_SRC_1);
  io |= AD7792_IO_EXCITATION_SEL(AD7792_IO_EXCITATION_1);
  if (AD7792_write_verify(AD7792_COMM_ADDR(AD7792_REG_IO), io, 1, slot)) {
    return ERROR_AD7792_CONFIGURATION;
  }

  /* zero calibration */
  unsigned short mode = 0x00;
  mode |= (AD7792_MODE_SEL(AD7792_MODE_CALIBRATION_INTERNAL_ZERO));
  mode |= AD7792_MODE_FILTER_SEL(AD7792_MODE_FILTER_9);
  AD7792_write(AD7792_COMM_ADDR(AD7792_REG_MODE), mode, 2, slot);

  AD7792_waitReady(slot);

  /* full calibration */
  mode = 0x00;
  mode |= (AD7792_MODE_SEL(AD7792_MODE_CALIBRATION_INTERNAL_FULL));
  mode |= AD7792_MODE_FILTER_SEL(AD7792_MODE_FILTER_9);
  AD7792_write(AD7792_COMM_ADDR(AD7792_REG_MODE), mode, 2, slot);

  AD7792_waitReady(slot);

  return 0; // success
}


float AD7792_getTemperature(TempSlot slot) {

  float temp, reading, x1, y1, x2, y2;

  /* perform a single conversion */
  unsigned short mode = 0x00;
  mode |= (AD7792_MODE_SEL(AD7792_MODE_SC));
  mode |= AD7792_MODE_FILTER_SEL(AD7792_MODE_FILTER_9);
  AD7792_write(AD7792_COMM_ADDR(AD7792_REG_MODE), mode, 2, slot);

  /* wait for result */
  AD7792_waitReady(slot);

  /* read result */
  temp = AD7792_read(AD7792_COMM_ADDR(AD7792_REG_DATA), 2, slot);

  /* linear interpolation from dummy values */
  x1 = 23325.0;
  y1 = 0;
  x2 = 30637.0;
  y2 = 81.4;
  reading = y1 + ((y2 - y1) / (x2 - x1)) * (temp - x1);
  return reading;
}
#endif //__AVR__
