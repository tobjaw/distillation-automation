/**
 * @file  api_physical.h
 */
#include "api.h"


unsigned char initPhysical();
int getHeaterStatusPhysical();
int setHeaterStatusPhysical(HeaterStatus power);
float getTemperaturePhysical();
