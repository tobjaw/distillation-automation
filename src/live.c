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
#include "controller.h"

const int pwm_step = 1;
int pwm_enabled = 0;
int pwm_target = 0;

void PWMenable(void) { pwm_enabled = 1; }
void PWMdisable(void) { pwm_enabled = 0; }
void PWMdec(void) {
  PWMenable();
  if (pwm_target - pwm_step <= 0) {
    pwm_target = 0;
  } else {
    pwm_target -= pwm_step;
  }
}
void PWMinc(void) {
  PWMenable();
  if (pwm_target + pwm_step >= 100) {
    pwm_target = 100;
  } else {
    pwm_target += pwm_step;
  }
}
void PWMset(int v) {
  PWMenable();
  pwm_target = v;
}

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

  PWMGen();


  screen_clear();
  cursor_hide();
  printf("\n\n\n\n\n\n\n\n\n" COLOR_BG_YELLOW COLOR_FG_BLACK "[H]" COLOR_NORMAL
         ": toggle heater power\n" COLOR_BG_YELLOW COLOR_FG_BLACK
         "[Q]" COLOR_NORMAL ": exit\n" COLOR_BG_YELLOW COLOR_FG_BLACK
         "[-]" COLOR_NORMAL ": PWM decrease\n" COLOR_BG_YELLOW COLOR_FG_BLACK
         "[+]" COLOR_NORMAL ": PWM increase\n" COLOR_BG_YELLOW COLOR_FG_BLACK
         "[0]" COLOR_NORMAL ": PWM set 0\n" COLOR_BG_YELLOW COLOR_FG_BLACK
         "[1]" COLOR_NORMAL ": PWM set 1\n");

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

    if (pwm_enabled == 1) {
      if (heater_switch((float)pwm_target / 100) == HEATER_ON) {
        api.setHeaterStatus(HEATER_ON);
      } else {
        api.setHeaterStatus(HEATER_OFF);
      }
    }

    if (STDIN_HAS_DATA) {
      input = getchar();
      switch (input) {
      case 'h':
        heaterStatus = api.setHeaterStatus(
            heaterStatus == HEATER_OFF ? HEATER_ON : HEATER_OFF);
        PWMdisable();
        break;

      case 'q':
        cursor_show();
        api.setHeaterStatus(HEATER_OFF);
        return;
        break;

      case KEY_PLUS:
        PWMinc();
        break;

      case KEY_MINUS:
        PWMdec();
        break;

      case KEY_ONE:
        PWMset(100);
        break;

      case KEY_ZERO:
        PWMset(0);
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

    if (pwm_enabled == 0) {
      printf("PWM %17s\n" COLOR_NORMAL, COLOR_FG_RED "DISABLED");
    } else {
      printf("PWM %13s%.2f\n" COLOR_NORMAL, COLOR_FG_GREEN "",
             (float)pwm_target / 100);
    }
    printf("cycle %10.2f\n" COLOR_NORMAL, PWM_COUNTER);

    printf("%21s\n" COLOR_NORMAL, sbStatus == SB_NOT_READY
                                      ? COLOR_FG_RED "SB_NOT_READY"
                                      : COLOR_FG_GREEN "SB_READY");
    printf("%21s\n" COLOR_NORMAL, heaterStatus == HEATER_OFF
                                      ? COLOR_FG_RED "HEATER OFF"
                                      : COLOR_FG_GREEN "HEATER  ON");
  }
}
