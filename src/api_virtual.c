/**
 * @file api_virtual.c
 *
 * Virtual implementation of the peripherals,
 * used for development, simulation and tests.
 */

#include "api_virtual.h"

/* mock status, kept in memory only */
int heater_status_virtual = HEATER_ON;


unsigned char initVirtual() { return 0; }

/**
 * basic temperature simulation
 */
float getTemperatureVirtual(TempSlot _tempSlot) {
  static int t = 23; /**< starting temperature */

  if (heater_status_virtual == HEATER_ON) {
    t += 2;
  } else {
    t -= 1;
  }

  return t;
}


SBStatus getSBreadyStatusVirtual() { return SB_READY; }

/**
 * read power state
 */
HeaterStatus getHeaterStatusVirtual() { return heater_status_virtual; }


/**
 * save power state
 */
HeaterStatus setHeaterStatusVirtual(HeaterStatus power) {
  heater_status_virtual = power;
  return power;
}

float getWeightVirtual() { return -1.0; }
