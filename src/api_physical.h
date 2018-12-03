/**
 * @file  api_physical.h
 */
#ifndef API_PHYSICAL_H
#define API_PHYSICAL_H

#include "api.h"
#include "AD7792.h"

unsigned char initPhysical();

HeaterStatus getHeaterStatusPhysical();
HeaterStatus setHeaterStatusPhysical(HeaterStatus power);
HeaterStatus heaterStatus;

SBStatus getSBreadyStatusPhysical();

float getTemperaturePhysical(TempSlot _tempSlot);
float getWeightPhysical();


#endif /* API_PHYSICAL_H */
