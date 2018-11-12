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
  float temperature1, temperature2;
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
   * Check current temperature and
   * switch heater power to stay within
   * TEMPERATURE_MIN and TEMPERATURE_MAX.
   */

  while (1) {
    // get current states
    temperature1 = api.getTemperature(SLOT1);
    temperature2 = api.getTemperature(SLOT2);

    _log("%f,%f", temperature1, temperature2);
  }

  return 0;
}
