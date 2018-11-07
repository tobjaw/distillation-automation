/**
 * @file  main.c
 */

#include <stdio.h>
#include "main.h"
#include "api.h"
#include "compat.h"
#include "errors.h"

const int TEMPERATURE_MIN = 45;
const int TEMPERATURE_MAX = 60;

int main() {
  API api;
  float temperature;
  unsigned char status;

  _clock_setup();
  _stdout_setup();

  if (LOG_LEVEL >= LOG_LEVEL_INFO) {
    _log("main()");
  }

  /* Interface to Peripherals
   * see "api.h" for documentation
   */
  api = newAPI();
  status = api.init();
  if (LOG_LEVEL >= LOG_LEVEL_INFO) {
    _log("api.init()=%d", status);
  }

  if (status) {
    switch (status) {
    case ERROR_SPI_INIT:
      _log("Error: Could not init SPI.");
      break;
    case ERROR_AD7792_CONNECTION:
      _log("Error: Could not establish communication with AD7792.");
      break;
    case ERROR_AD7792_CONFIGURATION:
      _log("Error: Could not configure AD7792 correctly.");
      break;
    default:
      _log("Error.");
    }
    while (1) {
    };
  }

  /* Main Loop
   *
   * Check current temperature and
   * switch heater power to stay within
   * TEMPERATURE_MIN and TEMPERATURE_MAX.
   */

  while (1) {
    // get current states
    temperature = api.getTemperature();

    _log("%f", temperature);
    _sleep(500);
  }

  return 0;
}
