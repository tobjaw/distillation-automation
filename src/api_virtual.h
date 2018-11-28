/**
 * @file  api_virtual.h
 */
#ifndef API_VIRTUAL_H
#define API_VIRTUAL_H

#include "api.h"

unsigned char initVirtual();

HeaterStatus getHeaterStatusVirtual();
HeaterStatus setHeaterStatusVirtual(HeaterStatus power);

SBStatus getSBreadyStatusVirtual();

float getTemperatureVirtual(TempSlot _tempSlot);
float getWeightVirtual();

#endif /* API_VIRTUAL_H */
