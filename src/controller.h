#ifndef CONTROLLER_H
#define CONTROLLER_H

float PI_controller(float ref_temp, float current_temp, float Kp, float Ki);
int bangbang_ctr_init(float ref_tem, float hys, float current_temp);
void bangbang_ctr_exit(float ref_temp, float hys, float current_temp);
float bangbang_ctr(float ref_tem, float hys, float current_temp);
int heater_switch(float value);


#endif /* CONTROLLER_H */
