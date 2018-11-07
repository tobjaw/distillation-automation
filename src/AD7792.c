#include <stdio.h>
#include <util/delay.h>

#include "AD7792.h"
#include "SPI.h"
#include "errors.h"


unsigned short AD7792_read(unsigned char addr, unsigned char length) {
  static unsigned short result;
  static unsigned char reg;
  static unsigned char buf[2];
  static unsigned int i = 0;

  reg = AD7792_COMM_READ | addr;

  SPI_SLAVE_0_SELECT;
  SPI_tradeByte(reg);
  for (i = 0; i < length; i++) {
    if (i < length - 1) {
      SPI_tradeByte(reg);
    } else {
      SPI_tradeByte(0);
    }
    buf[i] = SPDR;
  }
  SPI_SLAVE_0_DESELECT;

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


void AD7792_write(unsigned char addr, unsigned short data,
                  unsigned int length) {
  static unsigned char reg;
  static unsigned int i = 0;
  static unsigned char buf;

  reg = AD7792_COMM_WRITE | addr;

  SPI_SLAVE_0_SELECT;
  SPI_tradeByte(reg);
  for (i = length; i > 0; i--) {
    buf = ((data >> ((i - 1) * 8)) & 0xFF);
    SPI_tradeByte(buf);
  }
  SPI_SLAVE_0_DESELECT;

  if (LOG_LEVEL >= LOG_LEVEL_INFO) {
    printf("%sAD7792 write(" BYTE_TO_BINARY_PATTERN ")=0x%04hhx\n", LOG_INDENT,
           BYTE_TO_BINARY(reg), data);
  }
}


int AD7792_write_verify(unsigned char addr, unsigned short data,
                        unsigned int length) {
  static unsigned short buf;

  AD7792_write(addr, data, length);
  buf = AD7792_read(addr, length);

  return (buf != data);
}

void AD7792_waitReady() {
  while (AD7792_read(AD7792_COMM_ADDR(AD7792_REG_STATUS), 1) &
         AD7792_STATUS_RDY) {
    _delay_ms(10);
  }
}

int AD7792_isConnected() {
  static unsigned char id;
  static unsigned int isConnected;
  id = AD7792_read(AD7792_COMM_ADDR(AD7792_REG_ID), 1);
  isConnected = ((id & AD7792_MODEL_MASK) == AD7792_MODEL_ID);
  return isConnected;
}


void AD7792_reset() {
  SPI_SLAVE_0_SELECT;
  SPI_tradeByte(0xFF);
  SPI_tradeByte(0xFF);
  SPI_tradeByte(0xFF);
  SPI_tradeByte(0xFF);
  SPI_SLAVE_0_DESELECT;
  _delay_ms(10);
}


unsigned char AD7792_init() {
  /* reset device */
  AD7792_reset();

  /* check for connection */
  if (!AD7792_isConnected()) {
    return ERROR_AD7792_CONNECTION;
  }

  /* config register setup */
  unsigned short config = 0x0000;
  config |= (AD7792_CONFIG_POL_UNIPOLAR);
  config |= (AD7792_CONFIG_GAIN_SEL(AD7792_CONFIG_GAIN_LEVEL_2));
  if (AD7792_write_verify(AD7792_COMM_ADDR(AD7792_REG_CONFIG), config, 2)) {
    return ERROR_AD7792_CONFIGURATION;
  }

  /* IO register setup */
  unsigned char io = 0x00;
  io |= AD7792_IO_CURRENT_SRC_SEL(AD7792_IO_CURRENT_SRC_1);
  io |= AD7792_IO_EXCITATION_SEL(AD7792_IO_EXCITATION_1);
  if (AD7792_write_verify(AD7792_COMM_ADDR(AD7792_REG_IO), io, 1)) {
    return ERROR_AD7792_CONFIGURATION;
  }

  /* zero calibration */
  unsigned short mode = 0x00;
  mode |= (AD7792_MODE_SEL(AD7792_MODE_CALIBRATION_INTERNAL_ZERO));
  mode |= AD7792_MODE_FILTER_SEL(AD7792_MODE_FILTER_9);
  AD7792_write(AD7792_COMM_ADDR(AD7792_REG_MODE), mode, 2);

  AD7792_waitReady();

  /* full calibration */
  mode = 0x00;
  mode |= (AD7792_MODE_SEL(AD7792_MODE_CALIBRATION_INTERNAL_FULL));
  mode |= AD7792_MODE_FILTER_SEL(AD7792_MODE_FILTER_9);
  AD7792_write(AD7792_COMM_ADDR(AD7792_REG_MODE), mode, 2);

  AD7792_waitReady();

  return 0; // success
}


unsigned int AD7792_getTemperature() {
  unsigned int temp = 0;

  /* perform a single conversion */
  unsigned short mode = 0x00;
  mode |= (AD7792_MODE_SEL(AD7792_MODE_SC));
  mode |= AD7792_MODE_FILTER_SEL(AD7792_MODE_FILTER_9);
  AD7792_write(AD7792_COMM_ADDR(AD7792_REG_MODE), mode, 2);

  /* wait for result */
  AD7792_waitReady();

  /* read result */
  temp = AD7792_read(AD7792_COMM_ADDR(AD7792_REG_DATA), 2);

  /* linear interpolation from dummy values */
  x1 = 30637.0;
  y1 = 81.4;
  x2 = 32440.0;
  y2 = 100.9;
  reading = y1 + ((y2 - y1) / (x2-x1)) * (temp - x1);
  return reading;
}
