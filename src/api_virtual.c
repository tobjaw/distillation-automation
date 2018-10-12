#include "api_virtual.h"

int heater_status_virtual = HEATER_ON;

int getHeaterStatusVirtual() {
  return heater_status_virtual;
}
int setHeaterStatusVirtual(HeaterStatus power) {
  heater_status_virtual = power;
  return power;
}
int getTemperatureVirtual() {
  static int t = 23;

  if (heater_status_virtual == HEATER_ON) {
    t += 2;
  } else {
    t -= 1;
  }

  return t;
}
