/**
 * @file controller.c
 *
 * Controller implementation for usage in distillation program.
 */

#include "controller.h"
#include "compat.h"
#include "distill.h"
#include "api.h"
#include "main.h"
#include <math.h>
#include <stdio.h>

float PI_controller(float ref_temp, float current_temp, float Kp, float Ki) {

  // control part

  float error = ref_temp - current_temp;

  static float esum = 0.0;
  esum = esum + (error * 0.4); // 0.4 = sample time


  float output = Kp * error + Ki * esum;

  if (output >= 1.0) {
    output = 1.0;
  }
  if (output <= 0.0) {
    output = 0.0;
  }

  return output;
}

float setNewRefTemp(float currentRefTemp) { return currentRefTemp + 0.5; }

HeaterStatus heater_switch(float value) // value [0,1] , defines the heat power
{
  float pwm_value = 0.0625 * PWM_COUNTER;
  if (value > pwm_value) {
    return HEATER_ON;
  } else {
    return HEATER_OFF;
  }
}

void outputLivePlotting(API api, float temp1, float temp2, float weight,
                        float PI_value) {
  SBStatus sbStatus;
  HeaterStatus heaterStatus;

  sbStatus = api.getSBreadyStatus();
  heaterStatus = api.getHeaterStatus();

  _log("%f,%f,%f,%d,%d,%f", temp1, temp2, weight, sbStatus, heaterStatus,
       PI_value);
}
