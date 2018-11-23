/**
 * @file  api_virtual.h
 */
#include "api.h"

unsigned char initVirtual();
int getHeaterStatusVirtual();
int setHeaterStatusVirtual(HeaterStatus power);
float getTemperatureVirtual(TempSlot _tempSlot);
float getWeightVirtual();
