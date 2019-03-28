/**
 * @file main.c
 *
 * Main entry point into distillation-automation software
 */

#include <stdio.h>
#include "main.h"
#include "errors.h"
#include "gui.h"
#include "live.h"
#include "tests.h"
#include "linear_distill.h"
#include "physical_model_controller.h"


void program_info(void) {
  printf("Distillation Automation Software\n");
  printf(
      "Use Arrow Keys/TAB to step through options and press ENTER to exec.\n");
  _sleep(10000);
}

int main() {

  _clock_setup();
  _stdout_setup();

  menu_item menu[] = {
      {.title = "Info", .exec = program_info},
      {.title = "Live Monitoring", .exec = program_live},
      {.title = "UART-HEATER Test", .exec = program_uart_heater_test},
      {.title = "Scale Test", .exec = program_scale_test},
      {.title = "Distill (Linear)", .exec = program_linear_distill},
      {.title = "Distill (Physical)", .exec = program_distill_physical}};

  GUI(menu, 6, 1);

  return 0;
}
