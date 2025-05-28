/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: piRegler.h
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

#ifndef piRegler_h_
#define piRegler_h_
#ifndef piRegler_COMMON_INCLUDES_
#define piRegler_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "math.h"
#endif                                 /* piRegler_COMMON_INCLUDES_ */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  real_T Integrator_DSTATE;            /* '<S36>/Integrator' */
} DW;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T sollWert;                     /* '<Root>/In1' */
  real_T istWert;                      /* '<Root>/In2' */
} ExtU;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T y;                            /* '<Root>/y' */
} ExtY;

/* Real-time Model Data Structure */
struct tag_RTM {
  const char_T * volatile errorStatus;
};

/* Block signals and states (default storage) */
extern DW rtDW;

/* External inputs (root inport signals with default storage) */
extern ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY rtY;

/* Model entry point functions */
extern void piRegler_initialize(void);
extern void piRegler_step(void);

/* Real-time Model object */
extern RT_MODEL *const rtM;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('GesamtmodellSimple/piRegler')    - opens subsystem GesamtmodellSimple/piRegler
 * hilite_system('GesamtmodellSimple/piRegler/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'GesamtmodellSimple'
 * '<S1>'   : 'GesamtmodellSimple/piRegler'
 * '<S2>'   : 'GesamtmodellSimple/piRegler/Discrete PI Speed'
 * '<S3>'   : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Anti-windup'
 * '<S4>'   : 'GesamtmodellSimple/piRegler/Discrete PI Speed/D Gain'
 * '<S5>'   : 'GesamtmodellSimple/piRegler/Discrete PI Speed/External Derivative'
 * '<S6>'   : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Filter'
 * '<S7>'   : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Filter ICs'
 * '<S8>'   : 'GesamtmodellSimple/piRegler/Discrete PI Speed/I Gain'
 * '<S9>'   : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Ideal P Gain'
 * '<S10>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Ideal P Gain Fdbk'
 * '<S11>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Integrator'
 * '<S12>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Integrator ICs'
 * '<S13>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/N Copy'
 * '<S14>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/N Gain'
 * '<S15>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/P Copy'
 * '<S16>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Parallel P Gain'
 * '<S17>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Reset Signal'
 * '<S18>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Saturation'
 * '<S19>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Saturation Fdbk'
 * '<S20>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Sum'
 * '<S21>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Sum Fdbk'
 * '<S22>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Tracking Mode'
 * '<S23>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Tracking Mode Sum'
 * '<S24>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Tsamp - Integral'
 * '<S25>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Tsamp - Ngain'
 * '<S26>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/postSat Signal'
 * '<S27>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/preSat Signal'
 * '<S28>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Anti-windup/Back Calculation'
 * '<S29>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/D Gain/Disabled'
 * '<S30>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/External Derivative/Disabled'
 * '<S31>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Filter/Disabled'
 * '<S32>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Filter ICs/Disabled'
 * '<S33>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/I Gain/Internal Parameters'
 * '<S34>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Ideal P Gain/Passthrough'
 * '<S35>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Ideal P Gain Fdbk/Disabled'
 * '<S36>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Integrator/Discrete'
 * '<S37>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Integrator ICs/Internal IC'
 * '<S38>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/N Copy/Disabled wSignal Specification'
 * '<S39>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/N Gain/Disabled'
 * '<S40>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/P Copy/Disabled'
 * '<S41>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Parallel P Gain/Internal Parameters'
 * '<S42>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Reset Signal/Disabled'
 * '<S43>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Saturation/Enabled'
 * '<S44>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Saturation Fdbk/Disabled'
 * '<S45>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Sum/Sum_PI'
 * '<S46>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Sum Fdbk/Disabled'
 * '<S47>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Tracking Mode/Disabled'
 * '<S48>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Tracking Mode Sum/Passthrough'
 * '<S49>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Tsamp - Integral/TsSignalSpecification'
 * '<S50>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/Tsamp - Ngain/Passthrough'
 * '<S51>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/postSat Signal/Forward_Path'
 * '<S52>'  : 'GesamtmodellSimple/piRegler/Discrete PI Speed/preSat Signal/Forward_Path'
 */
#endif                                 /* piRegler_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
