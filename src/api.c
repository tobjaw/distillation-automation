#include "api.h"
#include "api_virtual.h"
#ifdef __AVR__
#include "api_physical.h"
#endif // __AVR__


API initAPI() {
  API api;

  api.getTemperature = &getTemperatureVirtual;
  api.getHeaterStatus = &getHeaterStatusVirtual;
  api.setHeaterStatus = &setHeaterStatusVirtual;

  return api;
}
