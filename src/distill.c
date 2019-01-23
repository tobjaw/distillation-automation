/**
 * @file distill.c
 *
 * Main distillation program
 */

#include <stdio.h>
#include <math.h>
#include "main.h"
#include "api.h"
#include "compat.h"
#include "errors.h"
#include "controller.h"
#include "distill.h"


void program_distill(void) {
  API api;
  float temperature1, temperature2;
  unsigned char status;
  int start = 0;
  float PI_value = 0.0;
  HeaterStatus switch_ON = HEATER_OFF;
  int initialcdn = 0;
  float roomtemperature = 0.0;
  int mode_change = 0;
  float weight = 0.0;
  float ref = 95.0;

  /* Interface to Peripherals
   * see "api.h" for documentation
   */
  api = newAPI();
  status = api.init();

  if (status) {
    switch (status) {
    case ERROR_SPI_INIT:
      _log("Error: Could not init SPI.");
      break;
    case ERROR_AD7792_SLOT1:
      _log("Error: AD7792 on SLOT1.");
      break;
    case ERROR_AD7792_SLOT2:
      _log("Error: AD7792 on SLOT2");
      break;
    default:
      _log("Error.");
    }
    while (1) {
    };
  }

  /* Main Loop
   *
   * Report sensors.
   */
  PWMGen();

  while (1) {
    // get current states
    temperature1 = api.getTemperature(SLOT1); // swamp temp
    temperature2 = api.getTemperature(SLOT2); // head temp
    weight = api.getWeight();

    if (initialcdn == 0) {
      roomtemperature = temperature1;
      initialcdn = 1;
    }

    start++;

    // 0.01321, 5.3447*pow(10,-6)  parameter for water system
    // 0.018487, 4.1145*pow(10,-6) parameter for wine

    if (start >= 60) {

      PI_value =
          PI_controller(ref - roomtemperature, temperature1 - roomtemperature,
                        0.018487, 4.1145 * pow(10, -6));

      switch (mode_change) {
      // heating control part (PID)
      case 0:
        if (PWM_SWITCHED == HEATER_OFF) {
          switch_ON = heater_switch(PI_value);
          if (switch_ON == HEATER_ON) {
            api.setHeaterStatus(HEATER_ON);
          } else {
            api.setHeaterStatus(HEATER_OFF);
            PWM_SWITCHED = 1;
          }
        }
        break;

      // boiling control part
      case 1:
        if (PWM_SWITCHED == HEATER_OFF) {
          switch_ON = heater_switch(1.0);
          if (switch_ON == HEATER_ON) {
            api.setHeaterStatus(HEATER_ON);
          } else {
            api.setHeaterStatus(HEATER_OFF);
            PWM_SWITCHED = 1;
          }
        }
        break;
      }
    }

    _log("%.2f,%.2f,%.2f,%.2f,%.2f,%d", temperature1, temperature2, PWM_COUNTER,
         PI_value, weight, switch_ON);
  }
}
