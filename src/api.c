#include "api_virtual.h"
#include "api_physical.h"

API initAPI() {
  API api;

  api.getTemperature  = &getTemperatureVirtual;
  api.getHeaterStatus = &getHeaterStatusVirtual;
  api.setHeaterStatus = &setHeaterStatusVirtual;

  return api;
}
