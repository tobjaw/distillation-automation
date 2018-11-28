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
  HEATER_OFF, /**< Heater is switched off. */
  HEATER_ON   /**< Heater is switched on. */
} HeaterStatus;


/**
 * Possible states of the security board (SB).
 */
typedef enum {
  SB_NOT_READY, /**< Security board is not ready. */
  SB_READY      /**< Security board is ready. */
} SBStatus;

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
  float (*getTemperature)(TempSlot);

  /**
   * Get the current heater status.
   *
   * @return  current heater status HEATER_OFF or HEATER_ON
   */
  HeaterStatus (*getHeaterStatus)(void);

  /**
   * Set the heater status.
   *
   * @return  new heater status HEATER_OFF or HEATER_ON
   */
  HeaterStatus (*setHeaterStatus)(HeaterStatus);

  /**
   * Get the ready status of the security board.
   *
   * @return  SB_NOT_READY or SB_READY
   */
  SBStatus (*getSBreadyStatus)(void);

  /**
   * Get the current weight measured by the scale.
   *
   * @return  Return the current weight or -1 on error.
   */
  float (*getWeight)(void);
} API;


/**
 * Create a new API
 *
 * Uses either virtual or physical implementations
 * behind a common Interface.
 */
API newAPI();


#endif /* API_H */
