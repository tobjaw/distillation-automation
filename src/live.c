/**
 * @file live.c
 *
 * Live Monitoring of connected peripherals.
 */

#include <stdio.h>
#include "main.h"
#include "api.h"
#include "compat.h"
#include "errors.h"
#include "gui.h"

void program_live(void) {
  API api;
  float temperature1, temperature2, weight;
  unsigned char status;
  SBStatus sbStatus;
  HeaterStatus heaterStatus;
  unsigned int input = 0;

  /* Interface to Peripherals
   * see "api.h" for documentation
   */
  api = newAPI();
  status = api.init();

  while (status) {
    /* error: freeze */
  }

  screen_clear();
  cursor_hide();
  printf("\n\n\n\n\n\n\n" COLOR_BG_YELLOW COLOR_FG_BLACK "[H]" COLOR_NORMAL
         ": toggle heater power\n" COLOR_BG_YELLOW COLOR_FG_BLACK
         "[Q]" COLOR_NORMAL ": exit");

  /* Main Loop
   *
   * Report sensors.
   */
  while (1) {
    // get current states
    temperature1 = api.getTemperature(SLOT1);
    temperature2 = api.getTemperature(SLOT2);
    weight = api.getWeight();
    sbStatus = api.getSBreadyStatus();
    heaterStatus = api.getHeaterStatus();

    if (STDIN_HAS_DATA) {
      input = getchar();
      switch (input) {
      case 'h':
        heaterStatus = api.setHeaterStatus(
            heaterStatus == HEATER_OFF ? HEATER_ON : HEATER_OFF);
        break;
      case 'q':
        cursor_show();
        api.setHeaterStatus(HEATER_OFF);
        return;
        break;
      }

      input = 0;
    }

    screen_reset();
    printf("time    %08lu\n", elapsedTime);
    printf("temp1   %8.2f\n", temperature1);
    printf("temp2   %8.2f\n", temperature2);

    if (weight < 1) {
      printf("weight  " COLOR_FG_RED "%8.2f\n" COLOR_NORMAL, weight);
    } else {
      printf("weight  %8.2f\n", weight);
    }

    printf("%21s\n" COLOR_NORMAL, sbStatus == SB_NOT_READY
                                      ? COLOR_FG_RED "SB_NOT_READY"
                                      : COLOR_FG_GREEN "SB_READY");
    printf("%21s\n" COLOR_NORMAL, heaterStatus == HEATER_OFF
                                      ? COLOR_FG_RED "HEATER OFF"
                                      : COLOR_FG_GREEN "HEATER  ON");
  }
}
