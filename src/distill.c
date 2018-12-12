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

void outputLivePlotting(API api, float temp1, float temp2, float weight,
                        float PI_value) {
  SBStatus sbStatus;
  HeaterStatus heaterStatus;

  sbStatus = api.getSBreadyStatus();
  heaterStatus = api.getHeaterStatus();

  _log("%f,%f,%f,%d,%d,%f", temp1, temp2, weight, sbStatus, heaterStatus,
       PI_value);
}

void program_distill(void) {
  API api;
  float temperature1, temperature2;
  unsigned char status;
  int start = 0;
  float PI_value = 0.0;
  int switch_ON = 0;
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
      // heanting control part (PID)
      case 0:
        if (switched == 0) {
          switch_ON = heater_switch(PI_value);
          if (switch_ON == 1) {
            api.setHeaterStatus(HEATER_ON);
          } else {
            api.setHeaterStatus(HEATER_OFF);
            switched = 1;
          }
        }
        break;

      // boiling control part
      case 1:
        if (switched == 0) {
          switch_ON = heater_switch(1.0);
          if (switch_ON == 1) {
            api.setHeaterStatus(HEATER_ON);
          } else {
            api.setHeaterStatus(HEATER_OFF);
            switched = 1;
          }
        }
        break;
      }
    }
    _sleep(100);
    outputLivePlotting(api, temperature1, temperature2, weight, PI_value);
    _sleep(100);
  }
}
