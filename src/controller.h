/**
 * @file controller.h
 *
 * Controller implementation for usage in distillation program.
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "api.h"

float PI_controller(float ref_temp, float current_temp, float Kp, float Ki);
int bangbang_ctr_init(float ref_tem, float hys, float current_temp);
void bangbang_ctr_exit(float ref_temp, float hys, float current_temp);
float bangbang_ctr(float ref_tem, float hys, float current_temp);
HeaterStatus heater_switch(float value);
void outputLivePlotting(API api, float temp1, float temp2, float weight,
float PI_value);


#endif /* CONTROLLER_H */
