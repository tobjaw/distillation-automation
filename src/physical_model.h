/**
 * @file physical_model.h
 * function to calculate the heating power to get the required outflow
 *
 */

#ifndef PHYSICAL_MODEL_H_
#define PHYSICAL_MODEL_H_

/**
 * @defgroup PM_CONSTANTS Constants for operation of the physical model
 * controller.
 *
 * @{
 */
#define QV_W 2256  /**< enthalpy of vaporization water at 100°C [kJ/kg] */
#define QV_E 845   /**< enthalpy of vaporization ethanol at 78°C [kJ/kg] */
#define CP_W 4.217 /**< heating capacity water at 100°C [kJ/(kg*K)] */
#define CP_E 2.248 /**< heating capacity ethanol at ?°C [kJ/(kg*K)] */
#define DENSITY_W1 0.997048 /**< density of water at 25°C [g/ml] */
#define DENSITY_W2 0.96189  /**< density of water at 95°C [g/ml] */
#define DENSITY_E 0.7859    /**< density of ethanol at 25°C [g/ml] */

#define L_50 0.0712      /**< heating power for 50% */
#define L_100 (2 * L_50) /**< max heating power */

#define SUMP_VOLUME 400

#define PWM_PERIOD 16
#define DELTA_STEP 16000

#define P_CONTROL_FACTOR 0.05

#define FLOW_REF 1.0 /**< ref outflow [g/min] */
#define DELTA_E_INIT (FLOW_REF / 60 * P_CONTROL_FACTOR)

#define TEMP_WATER_MAX 100
#define TEMP_WINE_BOILING 94

#define SUMP_A_BOILING_TEMP 94
#define SUMP_A_ALCOHOL_VOL_PERCENTAGE 10

#define SUMP_B_BOILING_TEMP 100
#define SUMP_B_ALCOHOL_VOL_PERCENTAGE 0

#define HEAD_A_CONDENSE_TEMP 83
#define HEAD_A_ALCOHOL_VOL_PERCENTAGE 80

#define HEAD_B_CONDENSE_TEMP 92
#define HEAD_B_ALCOHOL_VOL_PERCENTAGE 50
/** @} */

/**
 * State of the Physical Model Controller.
 */
typedef enum {
  PMC_STATE_INIT, /**< Initialize */
  PMC_STATE_MAIN  /**< Body */
} PMC_STATE;

float headTemp[4];   /**< array to save the old value */
float sumpTemp_prev; /**< previous sump temperature */
float weight_prev;   /**< previous weight */
float flow_prev;     /**< previous outflow */

long unsigned timestamp;
PMC_STATE PMC_state;

float deltaE[5];   /**< hold the old vlaue. For more information look at the
                      bottom. */
float deltaE_curr; /**< additional energy to compensate the error whitch occurs
                      by the no linearity of the system */
volatile float m_e_p; /**< mass ethanol in product [g] */
volatile float m_w_p; /**< mass water in product [g] */
volatile float m_e_e; /**< mass ethanol in educt [g] */
volatile float m_w_e; /**< mass water in educt [g] */
volatile float m_e;   /**< total mass of ethanol in educt [g] */


float L0; /**< base power of the heater */

/**
 * Calculate the heating power by using the physical theorem
 *
 * @param lastContrValue  heater power if the temperature reached first time the
 * boiling temperatur
 * @param initialHeadTemp  initil head temperature
 * @param boilingTemp  boiling temperature of wine
 * @param headTemp_curr  current head temperature
 * @paran sumpTemp_curr current sump temperature
 * @param weight_curr  current weight
 *
 * @return heater power
 */
float physicalModel(float lastContrValue, float initialHeadTemp,
                    float boilingTemp, float headTemp_curr, float sumpTemp_curr,
                    float weight_curr);

#endif /* PHYSICAL_MODEL_H_ */
