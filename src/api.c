#include "api.h"

#ifdef __x86_64
#include "api_virtual.h"
#endif // __x86_64
#ifdef __AVR__
#include "api_physical.h"
#endif // __AVR__


API newAPI() {
  API api;

#ifdef __x86_64
  api.init = &initVirtual;
  api.getTemperature = &getTemperatureVirtual;
  api.getActualHeaterStatus = &getHeaterStatusVirtual;
  api.getHeaterVariable = &getHeaterStatusVirtual;
  api.getSBHeaterStatus = &getHeaterStatusVirtual;
  api.setHeaterStatus = &setHeaterStatusVirtual;
  api.getWeight = &getWeightVirtual;
#endif // __x86_64

#ifdef __AVR__
  api.init = &initPhysical;
  api.getTemperature = &getTemperaturePhysical;
  api.getActualHeaterStatus = &getActualHeaterStatusPhysical;
  api.getHeaterVariable = &getHeaterVariablePhysical;
  api.getSBHeaterStatus = &getSBHeaterStatusPhysical;
  api.setHeaterStatus = &setHeaterStatusPhysical;
  api.getWeight = &getWeightPhysical;
#endif // __AVR__

  return api;
}
