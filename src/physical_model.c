#include <stdio.h>
#include "physical_model.h"
#include "compat.h"


/*
 * TODO: add comments
 * TODO: identifier names
 * TODO: maybe use for loop for headTemp[]
 */

float physicalModel(float lastContrValue, float initialHeadTemp,
                    float boilingTemp, float headTemp_curr, float sumpTemp_curr,
                    float weight_curr) {
  float Qv; // enthalpy of vaporization of our solution [kJ/kg]
  float Cp; // heating capacity of the mixture
  float Lv; // power for vaporization

  float heatE;        // heat energy to compasate the tempDiff
  float totalE;       // total needed energy
  float totalL = 0.0; // total power to get the required outflow

  float alcConcentrationVine; // alc concentration of vine
  float alcConcentrationHead; // alc concentration in the head

  float tempDiff;  // temp diff
  float flow_curr; // curretn outflow [g/min]

  switch (PMC_state) {
  case PMC_STATE_INIT:
    // interpolate the alc concentration at the beginning of boil phase in the
    // sump
    alcConcentrationVine = (-1 * (SUMP_A_ALCOHOL_VOL_PERCENTAGE) /
                            (SUMP_B_BOILING_TEMP - SUMP_A_BOILING_TEMP) *
                            (boilingTemp - SUMP_A_BOILING_TEMP)) +
                           SUMP_A_ALCOHOL_VOL_PERCENTAGE;

    m_e_e = alcConcentrationVine * SUMP_VOLUME * DENSITY_E;
    m_w_e = (1 - alcConcentrationVine) * SUMP_VOLUME * DENSITY_W1;
    m_e = m_e_e + m_w_e;


    Cp = (m_e_e / m_e * CP_E) + (m_w_e / m_e * CP_W);

    tempDiff = sumpTemp_curr - boilingTemp;
    heatE = Cp * m_e / 1000 * tempDiff;

    Qv = (m_e_e / m_e * QV_E) + (m_w_e / m_e * QV_W);
    Lv = Qv / 1000 * FLOW_REF / 60;

    L0 = L_100 * lastContrValue; // base-power to keep reference temperature

    totalE = (Lv * PWM_PERIOD) + heatE + (L0 * PWM_PERIOD);
    totalL = totalE / L_100 / PWM_PERIOD;

    headTemp[0] = headTemp_curr;
    headTemp[1] = headTemp[2] = headTemp[3] = initialHeadTemp;
    sumpTemp_prev = sumpTemp_curr;
    weight_prev = weight_curr;
    deltaE[3] = DELTA_E_INIT;
    timestamp = elapsedTime;
    PWM_COUNTER = 0; // synchronize the system with control cycle
    PWM_SWITCHED = 0;
    PMC_state = 1;

    break;

  case PMC_STATE_MAIN:
    if (elapsedTime >= timestamp + DELTA_STEP) {

      // interpolate alc concentration in the head
      alcConcentrationHead =
          (-1 * (HEAD_A_ALCOHOL_VOL_PERCENTAGE) / (100 - HEAD_A_CONDENSE_TEMP) *
           (((headTemp[2] + headTemp[3]) / 2) - HEAD_A_CONDENSE_TEMP)) +
          HEAD_A_ALCOHOL_VOL_PERCENTAGE;

      // calculate outflow
      flow_curr = weight_curr - weight_prev;
      // mass of ethanol and water in the product
      m_e_p = m_e_p + (flow_curr * alcConcentrationHead / 100);
      m_w_p = m_w_p + (flow_curr * (1 - (alcConcentrationHead / 100)));

      // assumption: amount of ethanol lost in the educt is equal to the ethanol
      // in the product
      m_e_e = m_e_e - m_e_p;
      m_w_e = m_w_e - m_w_p;
      m_e = m_e_e + m_w_e;

      // approximation of heat capacity based on mass share
      Cp = (m_e_e / m_e * CP_E) + (m_w_e / m_e * CP_W);
      // calculate heat energy to reach current swamp temperature
      tempDiff = sumpTemp_curr - boilingTemp;
      heatE = Cp * m_e / 1000 * tempDiff;
      // calculate vaporisation heat based on mass share
      Qv = (m_e_e / m_e * QV_E) + (m_w_e / m_e * QV_W);
      // calculate needed power from vaporisation heat
      Lv = Qv / 1000 * FLOW_REF / 60;

      if (weight_curr > 0.2) {
        // interpolate correction energy from prior values
        deltaE_curr = (deltaE[3] - deltaE[4]) / (flow_curr - flow_prev) *
                      (FLOW_REF - flow_curr);
        // shift values
        deltaE[4] = deltaE[3];
        deltaE[3] = deltaE[2];
        deltaE[2] = deltaE[1];
        deltaE[0] = deltaE_curr;
      }

      // calculate total needed energy from vaporisation power, heat energy,
      // base power and correction energy
      totalE = (Lv * PWM_PERIOD) + heatE + (L0 * PWM_PERIOD) + deltaE_curr;
      // calculate needed pwm value per pwm period for total energy
      totalL = totalE / L_100 / PWM_PERIOD;

      // shift values
      headTemp[3] = headTemp[2];
      headTemp[2] = headTemp[1];
      headTemp[1] = headTemp[0];
      headTemp[0] = headTemp_curr;
      sumpTemp_prev = sumpTemp_curr;
      weight_prev = weight_curr;
      flow_prev = flow_curr;

      timestamp = elapsedTime;
    }
    break;
  }

  return totalL;
}
