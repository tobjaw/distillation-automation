/**
 * @file  api.h
 */
// make sure we only include this file once.
#ifndef API_H
#define API_H


/**
 * Possible states of the heater unit.
 */
typedef enum {
  HEATER_ON, /**< Heater is switched on. */
  HEATER_OFF /**< Heater is switched off. */
} HeaterStatus;

typedef enum { SLOT1, SLOT2 } TempSlot;


/**
 * API to interact with the peripherals.
 *
 * All function calls return a non-negative int on success,
 * so we can easily check for errors.
 */
typedef struct {
  /**
   * Init call, used for setting up devices.
   *
   * @return  0 on success.
   */
  unsigned char (*init)();

  /**
   * Get the current temperature.
   *
   * @return  Return the current temperature in degrees celsius
   *          or 0 on error.
   */
  float (*getTemperature)(TempSlot _tempSlot);

  /**
   * Get the current heater status.
   *
   * @return  Return the current heater status or -1 on error.
   */
  int (*getHeaterStatus)(void);

  /**
   * Set the heater status.
   *
   * @return  Return the new heater status or -1 on error.
   */
  int (*setHeaterStatus)(HeaterStatus power);
} API;


/**
 * Create a new API
 *
 * Uses either virtual or physical implementations
 * behind a common Interface.
 */
API newAPI();


#endif /* API_H */
