#ifndef PHYSICAL_MODEL_CONTROLLER_H
#define PHYSICAL_MODEL_CONTROLLER_H

#include "api.h"

#define PIC_PROPORTIONAL ((float)0.018487)
#define PIC_INTEGRATION ((float)0.000004114)

#define TEMP_HEAD_MAX ((float)92.0)

/* declare all state that is shared */
typedef struct FSM_Data {
  API api;
  float temp_sump;
  float temp_head;
  float temp_head_initial;
  float temp_boil_real;
  float temp_room;
  float temp_ref;
  float power_hold;
  float power_heater;
  float weight;
  long unsigned timestamp;
} FSM_Data;

/* declare Finite State Machine */
struct FSM_Machine;
typedef void FSM_State(struct FSM_Machine *);
typedef struct FSM_Machine {
  FSM_State *state; /*< next state to transition to */
  FSM_Data *data;   /*< data */
} FSM_Machine;

/* declare states */
FSM_State PMC_Init, PMC_Waiting, PMC_Heating, PMC_Boiling, PMC_Fin;

void program_distill_physical(void);

#endif /* PHYSICAL_MODEL_CONTROLLER_H */
