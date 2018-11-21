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
#include <stdio.h>
#include "api_physical.h"
#include "SPI.h"
#include "AD7792.h"
#include "errors.h"
#include "scale.h"


int heater_status = HEATER_OFF;

unsigned char initPhysical() {
  if (SPI_init()) {
    return ERROR_SPI_INIT;
  }

  unsigned char AD7792_status = AD7792_init(SLOT1);
  if (AD7792_status) {
    if (LOG_LEVEL >= LOG_LEVEL_WARNING) {
      printf("%sinit physical: Error SLOT1, error code: %d", LOG_INDENT,
             (int)AD7792_status);
    }
    return ERROR_AD7792_SLOT1;
  }

  AD7792_status = AD7792_init(SLOT2);
  if (AD7792_status) {
    if (LOG_LEVEL >= LOG_LEVEL_WARNING) {
      printf("%sinit physical: Error SLOT2, error code: %d", LOG_INDENT,
             (int)AD7792_status);
    }
    return ERROR_AD7792_SLOT2;
  }

  if (scale_USARTinit()) {
    return ERROR_SCALE_USART_INIT;
  }

  if (scale_init()) {
    return ERROR_SCALE_INIT;
  }

  return 0;
}

float getTemperaturePhysical(TempSlot _tempSlot) {
  return AD7792_getTemperature(_tempSlot);
}

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

/**
 * Gives the actual heater status
 *
 * @return  Return the new heater
 */
int getActualHeaterStatusPhysical() {
  // PL2 := input
  DDRL &= ~(1 << PL2);

  if ((PINL & (1 << PL2)) && heater_status) {
    return HEATER_ON;
  } else {
    return HEATER_OFF;
  }
}

/**
 * Gives the heater variable
 *
 * @return  Return the heater
 */
int getHeaterVariablePhysical() { return heater_status; }

/**
 * Gives the heater status of the security board
 *
 * @return  Return the heater
 */
int getSBHeaterStatusPhysical() {

  // PL2 := input
  DDRL &= ~(1 << PL2);

  if (PINL & (1 << PL2)) {
    return HEATER_ON;
  } else {
    return HEATER_OFF;
  }
}

float getWeightPhysical() { return scale_getWeight(); }

#endif //__AVR__
