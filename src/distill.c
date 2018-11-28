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

  int switch_ON = 0;
  int initialcdn;
  float roomtemperature = 0.0;
  int modeA;
  int check;

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

    if (initialcdn == 0) {
      roomtemperature = temperature1;
      initialcdn = 1;
    }

    start++;


    if (bangbang_ctr_init(85.0 - roomtemperature, 0.5,
                          temperature2 - roomtemperature) == 1) {
      modeA = 1;
    }

    if (start >= 240) {

      switch (modeA) {
      // control part (PID)
      case 0:
        if (switched == 0) {
          switch_ON = heater_switch(0.5);
          if (switch_ON == 1) {
            api.setHeaterStatus(HEATER_ON);
          } else {
            api.setHeaterStatus(HEATER_OFF);
            switched = 1;
          }
        }
        break;

      // control part bang-bang
      case 1:
        if (switched == 0) {
          check = bangbang_ctr(85.0 - roomtemperature, 0.5,
                               temperature2 - roomtemperature);
          if (check == 1) {
            switch_ON = heater_switch(0.1);
            if (switch_ON == 1) {
              api.setHeaterStatus(HEATER_ON);
            } else {
              api.setHeaterStatus(HEATER_OFF);
              switched = 1;
            }
          }
        }
        break;
      }
    }
    _sleep(100);
    _log("%.2f,%.2f,%.2f,%.2f,%d", temperature1, temperature2, pwmcounter, 0.5,
         switch_ON);
    _sleep(100);
  }
  // time to get Temp-value 300ms + sleep time 200ms = 500ms;
}
