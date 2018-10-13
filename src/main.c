/**
 * @file  main.c
 */
#include "main.h"
#include "api.h"

const int TEMPERATURE_MIN = 45;
const int TEMPERATURE_MAX = 60;

int main () {
  API api;
  int temperature;
  HeaterStatus heater_status;


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

    printf("current temperature: %d\n", temperature);

    // controller logic
    if (temperature > TEMPERATURE_MAX && heater_status == HEATER_ON) {
      printf("temperature high, turning off heater\n");
      api.setHeaterStatus(HEATER_OFF);
    } else if (temperature < TEMPERATURE_MIN && heater_status == HEATER_OFF) {
      printf("temperature low, turning on heater\n");
      api.setHeaterStatus(HEATER_ON);
    }

    sleep(1);
  }


  return 0;
}
