/**
 * @file  api_physical.c
 *
 * Missing implementation.
 *
 * We need to read the actual hardware sensors
 * and turn actuators (like a heater) on and off.
 */
#ifdef __AVR__
#include <avr/io.h>
#include "api_physical.h"
#include "SPI.h"
#include "AD7792.h"
#include "errors.h"


int heater_status = HEATER_OFF;

unsigned char initPhysical() {
  if (SPI_init()) {
    return ERROR_SPI_INIT;
  }

  unsigned char AD7792_status = AD7792_init();
  if (AD7792_status) {
    return AD7792_status;
  }

  return 0;
}

float getTemperaturePhysical() { return AD7792_getTemperature(); }

/**
 * Set the heater status physically.
 * PL5
 *
 * @return  Return the new heater status or -1 on error.
 */
int setHeaterStatusPhysical(HeaterStatus power) {

  // PL5 := output
  DDRL |= (1 << PL5);

  if (power == HEATER_ON) {
    PORTL |= (1 << PL5); // PL5 High
    heater_status = HEATER_ON;
  } else if (power == HEATER_OFF) {
    PORTL &= ~(1 << PL5); // PL5 Low
    heater_status = HEATER_OFF;
  } else {
    return -1;
  }

  return 0;
}
int getHeaterStatusPhysical() { return heater_status; }

#endif //__AVR__
