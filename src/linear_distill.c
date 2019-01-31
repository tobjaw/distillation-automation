/**
 * @file linear_distill.c
 *
 * Contains linear Distillation
 */
#include <stdio.h>
#include <math.h>
#include "main.h"
#include "api.h"
#include "compat.h"
#include "errors.h"
#include "controller.h"
#include "linear_distill.h"

void program_linear_distill(void) {
  API api;
  float sumpTemp, headTemp;
  unsigned char status;

  HeaterStatus switchOn = 0;
  int initialcdn = 0;
  linDistModes modeA = WAIT;
  long unsigned timestamp;
  float weightstamp;
  float weight;
  float roomtemperature;
  float heatingPower = 0;

  float maxHeadTemp = 92;
  float heatingSumpTemp = 93.0;
  float requiredOutflow = 2.5 / 60000; // 1 g/min but in g/ms
  float n = 8.02617 * pow(10, -7);     // constant (0.0482 g/min/%)
  float b = -1.59108 * pow(10, -5);
  float currentOutflow = requiredOutflow;
  // at the beginning, they are equal because no outflow can be measured
  long unsigned dstepsize =
      60000L; // step size for outflow derivation - 60 seconds

  HeaterStatus realHS;

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
    sumpTemp = api.getTemperature(SLOT1); // sump temp
    headTemp = api.getTemperature(SLOT2); // head temp
    weight = api.getWeight();

    // In the very first run, some values have to be saved
    if (initialcdn == 0) {
      timestamp = elapsedTime;
      roomtemperature = sumpTemp;
      initialcdn = 1;
    }

    // Before begin, wait 1 minute
    switch (modeA) {
    case WAIT:
      // sleep for 1 minute
      if (elapsedTime >= timestamp + 60000L) {
        modeA = PI_HEATING;
      }
      break;
    // heating
    case PI_HEATING:
      heatingPower = PI_controller(heatingSumpTemp + 2 - roomtemperature,
                                   sumpTemp - roomtemperature, 0.018487,
                                   4.1145 * pow(10, -6));
      // heating finished
      if (sumpTemp >= heatingSumpTemp) {
        // P_start is calculated
        heatingPower = ((requiredOutflow - b) / n) / 100;
        if (heatingPower > 1) {
          heatingPower = 1;
        }
        if (heatingPower < 0) {
          heatingPower = 0;
        }
        modeA = P_START;
      }
      break;
    // wait till some outflow (0.3 g)
    case P_START:
      if (weight >= 0.3) {
        // take first timestamp for calculation of outflow
        timestamp = elapsedTime;
        weightstamp = weight;
        modeA = DISTILL;
      }
      break;
    // distillation
    case DISTILL:
      heatingPower = ((2 * requiredOutflow - currentOutflow - b) / n) / 100;
      if (heatingPower > 1) {
        heatingPower = 1;
      }
      if (heatingPower < 0) {
        heatingPower = 0;
      }
      // Calculate derivation of weight
      if (elapsedTime >= timestamp + dstepsize) {
        currentOutflow = (weight - weightstamp) / (elapsedTime - timestamp);
        weightstamp = weight;
        timestamp = elapsedTime;
      }
      // end of distillation
      if (maxHeadTemp <= headTemp) {
        heatingPower = 0;
        modeA = END;
      }
      break;
    case END:
      // do nothing
      break;
    }


    if (PWM_SWITCHED == 0) {
      switchOn = heater_switch(heatingPower);
      if (switchOn == HEATER_ON) {
        realHS = api.setHeaterStatus(HEATER_ON);
      } else {
        realHS = api.setHeaterStatus(HEATER_OFF);
        PWM_SWITCHED = 1;
      }
      if (realHS != switchOn && LOG_LEVEL >= LOG_LEVEL_INFO) {
        _log("Warning: Heater could not be set!");
      }
    }
    outputLivePlotting(api, sumpTemp, headTemp, weight, heatingPower);
  }
}
