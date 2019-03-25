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
	WAIT,
	PI_HEATING,
	P_START,
	DISTILL,
	END
} linDistModes;

  /**
   * Program for linear distillation.
   *
   */
void program_linear_distill(void);


#endif /* LINEAR_DISTILL_H_ */