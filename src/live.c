#include <stdio.h>
#include "main.h"
#include "api.h"
#include "compat.h"
#include "errors.h"

void program_live(void) {
  API api;
  float temperature1, temperature2, weight;
  unsigned char status;

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
    case ERROR_SCALE_USART_INIT:
      _log("Error: Scale USART");
      break;
    case ERROR_SCALE_INIT:
      _log("Error: Scale");
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
  while (1) {
    // get current states
    temperature1 = api.getTemperature(SLOT1);
    temperature2 = api.getTemperature(SLOT2);
    weight = api.getWeight();

    _log("%.2f,%.2f,%.1f", temperature1, temperature2, weight);
  }
}
