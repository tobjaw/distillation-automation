/**
 * @file  main.c
 */
#include <stdio.h>
#include "main.h"
#include "api.h"
#include "compat.h"

const int TEMPERATURE_MIN = 45;
const int TEMPERATURE_MAX = 60;

int main() {
  API api;
  int temperature;
  HeaterStatus heater_status;

  _clock_setup();
  _stdout_setup();

  /* Interface to Peripherals
   * see "api.h" for documentation
   */
  api = initAPI();


  /* Main Loop
   *
   * Check current temperature and
   * switch heater power to stay within
   * TEMPERATURE_MIN and TEMPERATURE_MAX.
   */

  while (1) {
    // get current states
    temperature = api.getTemperature();
    heater_status = api.getHeaterStatus();

    _log("current temperature: %d", temperature);

    // controller logic
    if (temperature > TEMPERATURE_MAX && heater_status == HEATER_ON) {
      _log("temperature high, turning off heater");
      api.setHeaterStatus(HEATER_OFF);
    } else if (temperature < TEMPERATURE_MIN && heater_status == HEATER_OFF) {
      _log("temperature low, turning on heater");
      api.setHeaterStatus(HEATER_ON);
    }

    _sleep(1000);
  }

  return 0;
}
