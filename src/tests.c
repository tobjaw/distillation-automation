/**
 * @file tests.c
 *
 * Test programs.
 */

#include <stdio.h>
#include "tests.h"
#include "main.h"
#include "api.h"
#include "compat.h"
#include "errors.h"

void program_scale_test(void) {
  API api;
  float weight = 0.0;

  api = newAPI();
  unsigned char status = api.init();

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

  while (1) {
    weight = api.getWeight();
    _log("%f", weight);
  }
}

void program_uart_heater_test(void) {
  API api;
  float t1;
  float t2;
  int h;

  api = newAPI();
  unsigned char status = api.init();

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

  _sleep(100);
  api.setHeaterStatus(HEATER_OFF);

  // shows temperatures and sets heater on and off
  // and sends the temperatures to UART
  while (1) {
    api.setHeaterStatus(HEATER_ON);
    t1 = api.getTemperature(SLOT1);
    t2 = api.getTemperature(SLOT2);
    _sleep(100);
    h = api.getHeaterStatus();
    _log("%f,%f,%d", t1, t2, h);
    _sleep(100);
    api.setHeaterStatus(HEATER_OFF);
    t1 = api.getTemperature(SLOT1);
    t2 = api.getTemperature(SLOT2);
    _sleep(100);
    h = api.getHeaterStatus();
    _log("%f, %f, %d", t1, t2, h);
    _sleep(100);
  }
}
