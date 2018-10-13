/**
 * @file  api_virtual.c
 *
 * Virtual implementation of the peripherals,
 * used for development, simulation and tests.
 */

#include "api_virtual.h"

/* mock status, kept in memory only */
int heater_status_virtual = HEATER_ON;


/**
 * basic temperature simulation
 */
int getTemperatureVirtual() {
  static int t = 23; /**< starting temperature */

  if (heater_status_virtual == HEATER_ON) {
    t += 2;
  } else {
    t -= 1;
  }

  return t;
}


/**
 * read power state
 */
int getHeaterStatusVirtual() { return heater_status_virtual; }


/**
 * save power state
 */
int setHeaterStatusVirtual(HeaterStatus power) {
  heater_status_virtual = power;
  return power;
}
