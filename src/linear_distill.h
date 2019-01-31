/**
 * @file linear_distill.h
 *
 * Contains linear Distillation
 */

#ifndef LINEAR_DISTILL_H_
#define LINEAR_DISTILL_H_

/**
 * Possible states during the linear distillation.
 */
typedef enum {
  WAIT,       /**< Wait for 60s. */
  PI_HEATING, /**< Heating */
  P_START,    /**< Wait for first outflow */
  DISTILL,    /**< Main distillation */
  END         /**< Finish */
} linDistModes;

/**
 * Program for linear distillation.
 *
 */
void program_linear_distill(void);


#endif /* LINEAR_DISTILL_H_ */
