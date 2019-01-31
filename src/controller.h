/**
 *@file controller.h
 *  This part include functions for heatinge wine untill it boils
 *
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "api.h"


/*PI Controller
 *@param ref_temp  reference temperature
 *Qparam current_temp  current temperature
 *@param Kp  propotional parameter
 *@param Ki  integrational parameter
 *
 * @return heater poer
 */
float PI_controller(float ref_temp, float current_temp, float Kp, float Ki);

/*
 *Sets new reference value. Increase the old value by 0.5
 *
 *@param currentRefTemp  current reference temperature
 *@return new reference temperature
 */
float setNewRefTemp(float currentRefTemp);

/*
 *switch for the heater. Realize the PWM.
 *Chechs whether the the carrier counter is smaller than the pwm input and
 calculate the duty cycle
 *
 *@param value pwm input
 @return 1 if true
 *
 */
HeaterStatus heater_switch(float value);

void outputLivePlotting(API api, float temp1, float temp2, float weight,
                        float PI_value);


#endif /* CONTROLLER_H */
