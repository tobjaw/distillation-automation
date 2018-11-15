/**
 * @file  main.c
 */

#include <stdio.h>
#include "main.h"
#include "compat.h"
#include "errors.h"
#include "gui.h"
#include "live.h"


void program_info(void) {
  printf("Distillation Automation Software\n");
  printf(
      "Use Arrow Keys/TAB to step through options and press ENTER to exec.\n");
  _sleep(10000);
}


int main() {
  _clock_setup();
  _stdout_setup();

  menu_item menu[] = {{.title = "Info", .exec = program_info},
                      {.title = "Live Reporting", .exec = program_live}};

  GUI(menu, 2, 0);


  return 0;
}
