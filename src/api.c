/**
 * @file  api.c
 */
#include "api_physical.h"
#include "api_virtual.h"

/**
 * Initialize the peripherals Interface.
 *
 * TODO: Use physical device implementation instead of virtual
 */
API initAPI() {
  API api;

  api.getTemperature = &getTemperatureVirtual;
  api.getHeaterStatus = &getHeaterStatusVirtual;
  api.setHeaterStatus = &setHeaterStatusVirtual;


  // NOTE: uncomment one of these to use the physical device implementation
  // in the future, this will probably be switched by some kind of command flag.
  /* api.getTemperature = &getTemperaturePhysical; */
  /* api.getHeaterStatus = &getHeaterStatusPhysical; */
  /* api.setHeaterStatus = &setHeaterStatusPhysical; */

  return api;
}
