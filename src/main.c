#include "main.h"
#include "api.h"

int main () {

  API api;
  int temp;
  HeaterStatus heater_status;

  api = initAPI();

  while (1) {
    temp = api.getTemperature();
    heater_status = api.getHeaterStatus();

    printf("current temperature: %d\n", temp);

    if (temp > 60 && heater_status == HEATER_ON) {
      printf("temperature high, turning off heater\n");
      api.setHeaterStatus(HEATER_OFF);
    } else if (temp < 45 && heater_status == HEATER_OFF) {
      printf("temperature low, turning on heater\n");
      api.setHeaterStatus(HEATER_ON);
    }

    sleep(1);
  }


  return EXIT_SUCCESS;
}
