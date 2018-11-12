#ifdef __AVR__
#include <stdio.h>
#include <util/delay.h>

#include "AD7792.h"
#include "SPI.h"
#include "errors.h"

inline void selectSlave(TempSlot _tempSlot) {

  if (_tempSlot == SLOT1) {
    SPI_SLAVE_0_SELECT;
  } else {
    SPI_SLAVE_2_SELECT;
  }
}

inline void deselectSlave(TempSlot _tempSlot) {

  if (_tempSlot == SLOT1) {
    SPI_SLAVE_0_DESELECT;
  } else {
    SPI_SLAVE_2_DESELECT;
  }
}

unsigned short AD7792_read(unsigned char addr, unsigned char length,
                           TempSlot _tempSlot) {
  static unsigned short result;
  static unsigned char reg;
  static unsigned char buf[2];
  static unsigned int i = 0;

  reg = AD7792_COMM_READ | addr;

  selectSlave(_tempSlot);

  SPI_tradeByte(reg);
  for (i = 0; i < length; i++) {
    if (i < length - 1) {
      SPI_tradeByte(reg);
    } else {
      SPI_tradeByte(0);
    }
    buf[i] = SPDR;
  }

  deselectSlave(_tempSlot);

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
                  TempSlot _tempSlot) {
  static unsigned char reg;
  static unsigned int i = 0;
  static unsigned char buf;

  reg = AD7792_COMM_WRITE | addr;

  selectSlave(_tempSlot);

  SPI_tradeByte(reg);
  for (i = length; i > 0; i--) {
    buf = ((data >> ((i - 1) * 8)) & 0xFF);
    SPI_tradeByte(buf);
  }

  deselectSlave(_tempSlot);

  if (LOG_LEVEL >= LOG_LEVEL_INFO) {
    printf("%sAD7792 write(" BYTE_TO_BINARY_PATTERN ")=0x%04hhx\n", LOG_INDENT,
           BYTE_TO_BINARY(reg), data);
  }
}


int AD7792_write_verify(unsigned char addr, unsigned short data,
                        unsigned int length, TempSlot _tempSlot) {
  static unsigned short buf;

  AD7792_write(addr, data, length, _tempSlot);
  buf = AD7792_read(addr, length, _tempSlot);

  return (buf != data);
}

void AD7792_waitReady(TempSlot _tempSlot) {
  while (AD7792_read(AD7792_COMM_ADDR(AD7792_REG_STATUS), 1, _tempSlot) &
         AD7792_STATUS_RDY) {
    _delay_ms(10);
  }
}

int AD7792_isConnected(TempSlot _tempSlot) {

  static unsigned char id;
  static unsigned int isConnected;
  id = AD7792_read(AD7792_COMM_ADDR(AD7792_REG_ID), 1, _tempSlot);
  isConnected = ((id & AD7792_MODEL_MASK) == AD7792_MODEL_ID);
  return isConnected;
}


void AD7792_reset(TempSlot _tempSlot) {

  selectSlave(_tempSlot);

  SPI_tradeByte(0xFF);
  SPI_tradeByte(0xFF);
  SPI_tradeByte(0xFF);
  SPI_tradeByte(0xFF);

  deselectSlave(_tempSlot);

  _delay_ms(10);
}


unsigned char AD7792_init(TempSlot _tempSlot) {

  /* reset device */
  AD7792_reset(_tempSlot);

  /* check for connection */
  if (!AD7792_isConnected(_tempSlot)) {
    return ERROR_AD7792_CONNECTION;
  }

  /* config register setup */
  unsigned short config = 0x0000;
  config |= (AD7792_CONFIG_POL_UNIPOLAR);
  config |= (AD7792_CONFIG_GAIN_SEL(AD7792_CONFIG_GAIN_LEVEL_2));
  if (AD7792_write_verify(AD7792_COMM_ADDR(AD7792_REG_CONFIG), config, 2,
                          _tempSlot)) {
    return ERROR_AD7792_CONFIGURATION;
  }

  /* IO register setup */
  unsigned char io = 0x00;
  io |= AD7792_IO_CURRENT_SRC_SEL(AD7792_IO_CURRENT_SRC_1);
  io |= AD7792_IO_EXCITATION_SEL(AD7792_IO_EXCITATION_1);
  if (AD7792_write_verify(AD7792_COMM_ADDR(AD7792_REG_IO), io, 1, _tempSlot)) {
    return ERROR_AD7792_CONFIGURATION;
  }

  /* zero calibration */
  unsigned short mode = 0x00;
  mode |= (AD7792_MODE_SEL(AD7792_MODE_CALIBRATION_INTERNAL_ZERO));
  mode |= AD7792_MODE_FILTER_SEL(AD7792_MODE_FILTER_9);
  AD7792_write(AD7792_COMM_ADDR(AD7792_REG_MODE), mode, 2, _tempSlot);

  AD7792_waitReady(_tempSlot);

  /* full calibration */
  mode = 0x00;
  mode |= (AD7792_MODE_SEL(AD7792_MODE_CALIBRATION_INTERNAL_FULL));
  mode |= AD7792_MODE_FILTER_SEL(AD7792_MODE_FILTER_9);
  AD7792_write(AD7792_COMM_ADDR(AD7792_REG_MODE), mode, 2, _tempSlot);

  AD7792_waitReady(_tempSlot);

  return 0; // success
}


float AD7792_getTemperature(TempSlot _tempSlot) {

  float temp, reading, x1, y1, x2, y2;

  /* perform a single conversion */
  unsigned short mode = 0x00;
  mode |= (AD7792_MODE_SEL(AD7792_MODE_SC));
  mode |= AD7792_MODE_FILTER_SEL(AD7792_MODE_FILTER_9);
  AD7792_write(AD7792_COMM_ADDR(AD7792_REG_MODE), mode, 2, _tempSlot);

  /* wait for result */
  AD7792_waitReady(_tempSlot);

  /* read result */
  temp = AD7792_read(AD7792_COMM_ADDR(AD7792_REG_DATA), 2, _tempSlot);

  /* linear interpolation from dummy values */
  x1 = 23325.0;
  y1 = 0;
  x2 = 30637.0;
  y2 = 81.4;
  reading = y1 + ((y2 - y1) / (x2 - x1)) * (temp - x1);
  return reading;
}
#endif //__AVR__
