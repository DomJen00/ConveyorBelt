/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: piRegler.c
 *
 * Code generated for Simulink model 'piRegler'.
 *
 * Model version                  : 11.0
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 * C/C++ source code generated on : Sat May 25 21:15:59 2024
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-A (32-bit)
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "piRegler.h"
#include "rtwtypes.h"

/* Block signals and states (default storage) */
DW rtDW;

/* External inputs (root inport signals with default storage) */
ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
ExtY rtY;

/* Real-time model */
static RT_MODEL rtM_;
RT_MODEL *const rtM = &rtM_;

/* Model step function */
void piRegler_step(void)
{
  real_T rtb_Saturation;
  real_T rtb_Sum;
  real_T rtb_regelAbw;

  /* Sum: '<S1>/Sum1' incorporates:
   *  Inport: '<Root>/In1'
   *  Inport: '<Root>/In2'
   */
  rtb_regelAbw = rtU.sollWert - rtU.istWert;

  /* Sum: '<S45>/Sum' incorporates:
   *  DiscreteIntegrator: '<S36>/Integrator'
   *  Gain: '<S41>/Proportional Gain'
   */
  rtb_Sum = 0.0008 * rtb_regelAbw + rtDW.Integrator_DSTATE;

  /* Saturate: '<S43>/Saturation' */
  if (rtb_Sum > 7.0) {
    rtb_Saturation = 7.0;
  } else if (rtb_Sum < -7.0) {
    rtb_Saturation = -7.0;
  } else {
    rtb_Saturation = rtb_Sum;
  }

  /* End of Saturate: '<S43>/Saturation' */

  /* Outport: '<Root>/y' */
  rtY.y = rtb_Saturation;

  /* Update for DiscreteIntegrator: '<S36>/Integrator' incorporates:
   *  Gain: '<S28>/Kb'
   *  Gain: '<S33>/Integral Gain'
   *  Sum: '<S28>/SumI2'
   *  Sum: '<S28>/SumI4'
   */
  rtDW.Integrator_DSTATE += ((rtb_Saturation - rtb_Sum) * 0.04 + 0.04 *
    rtb_regelAbw) * 0.02;
}

/* Model initialize function */
void piRegler_initialize(void)
{
  /* (no initialization code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
