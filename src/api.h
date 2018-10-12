#ifndef API_H
#define API_H


typedef enum
  {
   HEATER_ON,
   HEATER_OFF
  }
  HeaterStatus;


typedef struct {
  int (*getTemperature)(void);
  int (*getHeaterStatus)(void);
  int (*setHeaterStatus)(HeaterStatus power);
} API;


API initAPI();


#endif /* API_H */
