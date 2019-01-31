/**
 * @file api_physical.c
 *
 * Physical implementation of the API.
 */

#ifdef __AVR__
#include <avr/io.h>
#include <stdio.h>
#include "api_physical.h"
#include "SPI.h"
#include "AD7792.h"
#include "errors.h"
#include "scale.h"


unsigned char initPhysical() {
  setHeaterStatusPhysical(HEATER_OFF);

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


SBStatus getSBreadyStatusPhysical() {
  // PL2 := input
  DDRL &= ~(1 << PL2);

  if (PINL & (1 << PL2)) {
    return SB_READY;
  } else {
    return SB_NOT_READY;
  }
}


HeaterStatus setHeaterStatusPhysical(HeaterStatus power) {
  // PL5 := output
  DDRL |= (1 << PL5);

  if (getSBreadyStatusPhysical() == SB_NOT_READY || power == HEATER_OFF) {
    PORTL &= ~(1 << PL5);
    heaterStatus = HEATER_OFF;
  } else {
    PORTL |= (1 << PL5);
    heaterStatus = HEATER_ON;
  }

  return heaterStatus;
}


HeaterStatus getHeaterStatusPhysical() {
  // PL2 := input
  DDRL &= ~(1 << PL2);

  if (getSBreadyStatusPhysical() == SB_READY && heaterStatus == HEATER_ON) {
    return HEATER_ON;
  } else {
    return HEATER_OFF;
  }
}


float getWeightPhysical() { return scale_getWeight(); }

#endif //__AVR__
