/**
 * @file  api_physical.h
 */
#include "api.h"
#include "AD7792.h"

unsigned char initPhysical();
int getHeaterStatusPhysical();
int setHeaterStatusPhysical(HeaterStatus power);
float getTemperaturePhysical(TempSlot _tempSlot);
