#include <stdio.h>
#include "physical_model_controller.h"
#include "api.h"
#include "errors.h"
#include "stdio.h"
#include "compat.h"
#include "controller.h"
#include "physical_model.h"

void PMC_Init(struct FSM_Machine *machine) {
  printf("PMC_Init\n");
  unsigned char status;

  /* Interface to Peripherals
   * see "api.h" for documentation
   */

  machine->data->api = newAPI();
  status = machine->data->api.init();

  if (status)
    machine->state = PMC_Fin;
  while (status) {
    /* error: freeze */
  }

  machine->data->timestamp = elapsedTime;

  machine->data->temp_room = machine->data->api.getTemperature(SLOT1);
  machine->data->temp_head_initial = machine->data->api.getTemperature(SLOT2);
  machine->data->temp_ref = 92.0;

  machine->state = PMC_Waiting;
}

void PMC_Waiting(struct FSM_Machine *machine) {
  printf("PMC_Waiting\n");
  if (elapsedTime >= (machine->data->timestamp + 60000L)) {
    machine->data->timestamp = elapsedTime;
    PWM_COUNTER = 0;
    PWM_SWITCHED = 0;
    machine->state = PMC_Heating;
  } else {
    _sleep(1000);
    machine->state = PMC_Waiting;
  }
}

void PMC_Heating(struct FSM_Machine *machine) {
  printf("PMC_Heating\n");
  HeaterStatus switchOn;

  machine->data->temp_sump = machine->data->api.getTemperature(SLOT1);
  machine->data->temp_head = machine->data->api.getTemperature(SLOT2);

  machine->data->power_heater =
      PI_controller(machine->data->temp_ref - machine->data->temp_room,
                    machine->data->temp_sump - machine->data->temp_room,
                    PIC_PROPORTIONAL, PIC_INTEGRATION);

  if (PWM_SWITCHED == 0) {
    switchOn = heater_switch(machine->data->power_heater);
    if (switchOn == HEATER_ON) {
      machine->data->api.setHeaterStatus(HEATER_ON);
    } else {
      machine->data->api.setHeaterStatus(HEATER_OFF);
      PWM_SWITCHED = 1;
    }
  }

  if ((machine->data->temp_sump >= machine->data->temp_ref) &&
      elapsedTime >= machine->data->timestamp + 16000L) {

    if (machine->data->temp_head >= machine->data->temp_head_initial + 2.0) {
      machine->data->temp_boil_real = machine->data->temp_sump;
      machine->data->power_hold = machine->data->power_heater;
      machine->state = PMC_Boiling;
    } else {
      machine->data->temp_ref += 0.5;
      machine->data->timestamp = elapsedTime;
      machine->state = PMC_Heating;
    }

  } else {
    machine->state = PMC_Heating;
  }
}


void PMC_Boiling(struct FSM_Machine *machine) {
  printf("PMC_Boiling\n");
  HeaterStatus switchOn;

  machine->data->temp_sump = machine->data->api.getTemperature(SLOT1);
  machine->data->temp_head = machine->data->api.getTemperature(SLOT2);
  machine->data->weight = machine->data->api.getWeight();

  /* clang-format off */
  machine->data->power_heater = physicalModel(machine->data->power_hold,
                               machine->data->temp_head_initial,
                               machine->data->temp_boil_real,
                               machine->data->temp_head,
                               machine->data->temp_sump,
                               machine->data->weight);
  /* clang-format on */

  if (PWM_SWITCHED == 0) {
    switchOn = heater_switch(machine->data->power_heater);
    if (switchOn == HEATER_ON) {
      machine->data->api.setHeaterStatus(HEATER_ON);
    } else {
      machine->data->api.setHeaterStatus(HEATER_OFF);
      PWM_SWITCHED = 1;
    }
  }

  if (machine->data->temp_head >= TEMP_HEAD_MAX) {
    machine->state = PMC_Fin;
  } else {
    machine->state = PMC_Boiling;
  }
}

void PMC_Fin(struct FSM_Machine *machine) {
  printf("PMC_Fin\n");
  machine->data->api.setHeaterStatus(HEATER_OFF);
  machine->state = NULL;
}


void program_distill_physical(void) {
  FSM_Data data = {};
  FSM_Machine machine = {PMC_Init, &data};

  // run machine until state is NULL
  while (machine.state) {
    machine.state(&machine);
    /* clang-format off */
    printf("  temp_sump=%f\n  temp_head=%f\n  temp_head_initial=%f\n"
         "  temp_boil_real=%f\n"
         "  temp_room=%f\n  temp_ref=%f\n  power_hold=%f\n  power_heater=%f\n  "
         "weight=%f\n  timestamp=%lu\n  time=%lu\n\n",
         machine.data->temp_sump,
         machine.data->temp_head,
         machine.data->temp_head_initial,
         machine.data->temp_boil_real,
         machine.data->temp_room,
         machine.data->temp_ref,
         machine.data->power_hold,
         machine.data->power_heater,
         machine.data->weight,
         machine.data->timestamp,
         elapsedTime
         );
    /* clang-format on */
  }
}
