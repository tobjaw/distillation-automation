/**
 * @file  main.c
 */

#include <stdio.h>
#include "main.h"
#include "compat.h"
#include "errors.h"
#include "gui.h"
#include "live.h"
#include "tests.h"


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
      {.title = "UART-HEATER Test", .exec = program_uart_heater_test}};

  GUI(menu, 3, 1);


  return 0;
}
