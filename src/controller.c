#include "controller.h"
#include "compat.h"
#include "distill.h"
#include <math.h>

float PI_controller(float ref_temp, float current_temp, float Kp, float Ki) {

  // control part

  float error = ref_temp - current_temp;

  static float esum = 0.0;
  esum = esum + (error * 0.3); // 0.3 = sample time


  float output = Kp * error + Ki * esum;

  if (output >= 1.0) {
    return 1.0;
  } else {
    return output;
  }
}

int bangbang_ctr_init(float ref_temp, float hys, float current_temp) {
  if (current_temp >= ref_temp + hys) {
    return 1;
  } else {
    return 0;
  }
}


float bangbang_ctr(float ref_temp, float hys, float current_temp) {
  if (current_temp >= ref_temp + hys) {
    return 0.0;
  }
  if (current_temp <= ref_temp - hys) {
    return 1.0;
  }
  return 0.0;
}

int heater_switch(float value) // value [0,1] , defines the heat power
{
  float pwm_value = 0.0625 * pwmcounter;
  if (value > pwm_value) {
    return 1;
  } else {
    return 0;
  }
}
