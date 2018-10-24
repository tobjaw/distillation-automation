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


int heater_status = HEATER_OFF;

int getTemperaturePhysical() {
  // TODO: implement
  return -1;
}
int setHeaterStatusPhysical(HeaterStatus power) {

  // PA0 := output
  DDRA |= (1 << PA0);

  if (power == HEATER_ON) {
    PORTA |= (1 << PA0); // PA0 High
    heater_status = HEATER_ON;
  } else if (power == HEATER_OFF) {
    PORTA &= ~(1 << PA0); // PA0 Low
    heater_status = HEATER_OFF;
  } else {
    return -1;
  }

  return 0;
}
int getHeaterStatusPhysical() { return heater_status; }

#endif //__AVR__
