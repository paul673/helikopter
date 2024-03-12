/*
 * helicopter.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "helicopter".
 *
 * Model version              : 6.1
 * Simulink Coder version : 9.4 (R2020b) 29-Jul-2020
 * C source code generated on : Tue Mar 12 11:59:56 2024
 *
 * Target selection: quarc_win64.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "helicopter.h"
#include "helicopter_private.h"
#include "helicopter_dt.h"

/* Block signals (default storage) */
B_helicopter_T helicopter_B;

/* Continuous states */
X_helicopter_T helicopter_X;

/* Block states (default storage) */
DW_helicopter_T helicopter_DW;

/* Real-time model */
static RT_MODEL_helicopter_T helicopter_M_;
RT_MODEL_helicopter_T *const helicopter_M = &helicopter_M_;

/*
 * Writes out MAT-file header.  Returns success or failure.
 * Returns:
 *      0 - success
 *      1 - failure
 */
int_T rt_WriteMat4FileHeader(FILE *fp, int32_T m, int32_T n, const char *name)
{
  typedef enum { ELITTLE_ENDIAN, EBIG_ENDIAN } ByteOrder;

  int16_T one = 1;
  ByteOrder byteOrder = (*((int8_T *)&one)==1) ? ELITTLE_ENDIAN : EBIG_ENDIAN;
  int32_T type = (byteOrder == ELITTLE_ENDIAN) ? 0: 1000;
  int32_T imagf = 0;
  int32_T name_len = (int32_T)strlen(name) + 1;
  if ((fwrite(&type, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(&m, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(&n, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(&imagf, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(&name_len, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(name, sizeof(char), name_len, fp) == 0)) {
    return(1);
  } else {
    return(0);
  }
}                                      /* end rt_WriteMat4FileHeader */

/*
 * This function updates continuous states using the ODE1 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE1_IntgData *id = (ODE1_IntgData *)rtsiGetSolverData(si);
  real_T *f0 = id->f[0];
  int_T i;
  int_T nXc = 4;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);
  rtsiSetdX(si, f0);
  helicopter_derivatives();
  rtsiSetT(si, tnew);
  for (i = 0; i < nXc; ++i) {
    x[i] += h * f0[i];
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model output function */
void helicopter_output(void)
{
  /* local block i/o variables */
  real_T rtb_FromWorkspace[2];
  real_T rtb_HILReadEncoderTimebase_o1;
  real_T rtb_HILReadEncoderTimebase_o2;
  real_T rtb_HILReadEncoderTimebase_o3;
  real_T rtb_TmpSignalConversionAtToFile[2];
  real_T tmp[6];
  real_T rtb_Derivative;
  real_T rtb_Sum;
  real_T rtb_Sum1;
  real_T *lastU;
  int32_T i;
  int32_T i_0;
  int8_T rtAction;
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* set solver stop time */
    if (!(helicopter_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&helicopter_M->solverInfo,
                            ((helicopter_M->Timing.clockTickH0 + 1) *
        helicopter_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&helicopter_M->solverInfo,
                            ((helicopter_M->Timing.clockTick0 + 1) *
        helicopter_M->Timing.stepSize0 + helicopter_M->Timing.clockTickH0 *
        helicopter_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(helicopter_M)) {
    helicopter_M->Timing.t[0] = rtsiGetT(&helicopter_M->solverInfo);
  }

  /* Reset subsysRan breadcrumbs */
  srClearBC(helicopter_DW.IfActionSubsystem_SubsysRanBC);
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* S-Function (hil_read_encoder_timebase_block): '<S4>/HIL Read Encoder Timebase' */

    /* S-Function Block: helicopter/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
    {
      t_error result;
      result = hil_task_read_encoder(helicopter_DW.HILReadEncoderTimebase_Task,
        1, &helicopter_DW.HILReadEncoderTimebase_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
      } else {
        rtb_HILReadEncoderTimebase_o1 =
          helicopter_DW.HILReadEncoderTimebase_Buffer[0];
        rtb_HILReadEncoderTimebase_o2 =
          helicopter_DW.HILReadEncoderTimebase_Buffer[1];
        rtb_HILReadEncoderTimebase_o3 =
          helicopter_DW.HILReadEncoderTimebase_Buffer[2];
      }
    }

    /* Gain: '<S4>/Travel: Count to rad' incorporates:
     *  Gain: '<S4>/Travel_gain'
     */
    helicopter_B.TravelCounttorad = helicopter_P.travel_gain *
      rtb_HILReadEncoderTimebase_o1 * helicopter_P.TravelCounttorad_Gain;

    /* Gain: '<S12>/Gain' */
    helicopter_B.Gain = helicopter_P.Gain_Gain * helicopter_B.TravelCounttorad;

    /* Sum: '<Root>/Sum3' incorporates:
     *  Constant: '<Root>/Constant'
     */
    helicopter_B.Sum3 = helicopter_B.Gain + helicopter_P.Constant_Value;

    /* Gain: '<S4>/Pitch: Count to rad' */
    helicopter_B.PitchCounttorad = helicopter_P.PitchCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o2;

    /* Gain: '<S9>/Gain' */
    helicopter_B.Gain_i = helicopter_P.Gain_Gain_a *
      helicopter_B.PitchCounttorad;
  }

  /* Gain: '<S13>/Gain' incorporates:
   *  TransferFcn: '<S4>/Travel: Transfer Fcn'
   */
  helicopter_B.Gain_d = (helicopter_P.TravelTransferFcn_C *
    helicopter_X.TravelTransferFcn_CSTATE + helicopter_P.TravelTransferFcn_D *
    helicopter_B.TravelCounttorad) * helicopter_P.Gain_Gain_l;

  /* Gain: '<S10>/Gain' incorporates:
   *  TransferFcn: '<S4>/Pitch: Transfer Fcn'
   */
  helicopter_B.Gain_b = (helicopter_P.PitchTransferFcn_C *
    helicopter_X.PitchTransferFcn_CSTATE + helicopter_P.PitchTransferFcn_D *
    helicopter_B.PitchCounttorad) * helicopter_P.Gain_Gain_ae;
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* Gain: '<S4>/Elevation: Count to rad' incorporates:
     *  Gain: '<S4>/Elevation_gain'
     */
    helicopter_B.ElevationCounttorad = helicopter_P.elevation_gain *
      rtb_HILReadEncoderTimebase_o3 * helicopter_P.ElevationCounttorad_Gain;

    /* Gain: '<S7>/Gain' */
    helicopter_B.Gain_e = helicopter_P.Gain_Gain_lv *
      helicopter_B.ElevationCounttorad;

    /* Sum: '<Root>/Sum' incorporates:
     *  Constant: '<Root>/elavation_offset [deg]'
     */
    helicopter_B.Sum = helicopter_B.Gain_e +
      helicopter_P.elavation_offsetdeg_Value;
  }

  /* Gain: '<S8>/Gain' incorporates:
   *  TransferFcn: '<S4>/Elevation: Transfer Fcn'
   */
  helicopter_B.Gain_dg = (helicopter_P.ElevationTransferFcn_C *
    helicopter_X.ElevationTransferFcn_CSTATE +
    helicopter_P.ElevationTransferFcn_D * helicopter_B.ElevationCounttorad) *
    helicopter_P.Gain_Gain_n;

  /* Gain: '<S2>/Gain1' */
  helicopter_B.Gain1[0] = helicopter_P.Gain1_Gain * helicopter_B.Sum3;
  helicopter_B.Gain1[1] = helicopter_P.Gain1_Gain * helicopter_B.Gain_d;
  helicopter_B.Gain1[2] = helicopter_P.Gain1_Gain * helicopter_B.Gain_i;
  helicopter_B.Gain1[3] = helicopter_P.Gain1_Gain * helicopter_B.Gain_b;
  helicopter_B.Gain1[4] = helicopter_P.Gain1_Gain * helicopter_B.Sum;
  helicopter_B.Gain1[5] = helicopter_P.Gain1_Gain * helicopter_B.Gain_dg;
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* ToFile: '<Root>/To File' */
    {
      if (!(++helicopter_DW.ToFile_IWORK.Decimation % 1) &&
          (helicopter_DW.ToFile_IWORK.Count * (6 + 1)) + 1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_DW.ToFile_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[6 + 1];
          helicopter_DW.ToFile_IWORK.Decimation = 0;
          u[0] = helicopter_M->Timing.t[1];
          u[1] = helicopter_B.Gain1[0];
          u[2] = helicopter_B.Gain1[1];
          u[3] = helicopter_B.Gain1[2];
          u[4] = helicopter_B.Gain1[3];
          u[5] = helicopter_B.Gain1[4];
          u[6] = helicopter_B.Gain1[5];
          if (fwrite(u, sizeof(real_T), 6 + 1, fp) != 6 + 1) {
            rtmSetErrorStatus(helicopter_M,
                              "Error writing to MAT-file DataFiles/loop_variables.mat");
            return;
          }

          if (((++helicopter_DW.ToFile_IWORK.Count) * (6 + 1))+1 >= 100000000) {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file DataFiles/loop_variables.mat.\n");
          }
        }
      }
    }
  }

  /* FromWorkspace: '<Root>/From Workspace' */
  {
    real_T *pDataValues = (real_T *) helicopter_DW.FromWorkspace_PWORK.DataPtr;
    real_T *pTimeValues = (real_T *) helicopter_DW.FromWorkspace_PWORK.TimePtr;
    int_T currTimeIndex = helicopter_DW.FromWorkspace_IWORK.PrevIndex;
    real_T t = helicopter_M->Timing.t[0];

    /* Get index */
    if (t <= pTimeValues[0]) {
      currTimeIndex = 0;
    } else if (t >= pTimeValues[140]) {
      currTimeIndex = 139;
    } else {
      if (t < pTimeValues[currTimeIndex]) {
        while (t < pTimeValues[currTimeIndex]) {
          currTimeIndex--;
        }
      } else {
        while (t >= pTimeValues[currTimeIndex + 1]) {
          currTimeIndex++;
        }
      }
    }

    helicopter_DW.FromWorkspace_IWORK.PrevIndex = currTimeIndex;

    /* Post output */
    {
      real_T t1 = pTimeValues[currTimeIndex];
      real_T t2 = pTimeValues[currTimeIndex + 1];
      if (t1 == t2) {
        if (t < t1) {
          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 2; ++elIdx) {
              (&rtb_FromWorkspace[0])[elIdx] = pDataValues[currTimeIndex];
              pDataValues += 141;
            }
          }
        } else {
          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 2; ++elIdx) {
              (&rtb_FromWorkspace[0])[elIdx] = pDataValues[currTimeIndex + 1];
              pDataValues += 141;
            }
          }
        }
      } else {
        real_T f1 = (t2 - t) / (t2 - t1);
        real_T f2 = 1.0 - f1;
        real_T d1;
        real_T d2;
        int_T TimeIndex= currTimeIndex;

        {
          int_T elIdx;
          for (elIdx = 0; elIdx < 2; ++elIdx) {
            d1 = pDataValues[TimeIndex];
            d2 = pDataValues[TimeIndex + 1];
            (&rtb_FromWorkspace[0])[elIdx] = (real_T) rtInterpolate(d1, d2, f1,
              f2);
            pDataValues += 141;
          }
        }
      }
    }
  }

  /* FromWorkspace: '<Root>/From Workspace1' */
  {
    real_T *pDataValues = (real_T *) helicopter_DW.FromWorkspace1_PWORK.DataPtr;
    real_T *pTimeValues = (real_T *) helicopter_DW.FromWorkspace1_PWORK.TimePtr;
    int_T currTimeIndex = helicopter_DW.FromWorkspace1_IWORK.PrevIndex;
    real_T t = helicopter_M->Timing.t[0];

    /* Get index */
    if (t <= pTimeValues[0]) {
      currTimeIndex = 0;
    } else if (t >= pTimeValues[140]) {
      currTimeIndex = 139;
    } else {
      if (t < pTimeValues[currTimeIndex]) {
        while (t < pTimeValues[currTimeIndex]) {
          currTimeIndex--;
        }
      } else {
        while (t >= pTimeValues[currTimeIndex + 1]) {
          currTimeIndex++;
        }
      }
    }

    helicopter_DW.FromWorkspace1_IWORK.PrevIndex = currTimeIndex;

    /* Post output */
    {
      real_T t1 = pTimeValues[currTimeIndex];
      real_T t2 = pTimeValues[currTimeIndex + 1];
      if (t1 == t2) {
        if (t < t1) {
          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 6; ++elIdx) {
              (&helicopter_B.FromWorkspace1[0])[elIdx] =
                pDataValues[currTimeIndex];
              pDataValues += 141;
            }
          }
        } else {
          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 6; ++elIdx) {
              (&helicopter_B.FromWorkspace1[0])[elIdx] =
                pDataValues[currTimeIndex + 1];
              pDataValues += 141;
            }
          }
        }
      } else {
        real_T f1 = (t2 - t) / (t2 - t1);
        real_T f2 = 1.0 - f1;
        real_T d1;
        real_T d2;
        int_T TimeIndex= currTimeIndex;

        {
          int_T elIdx;
          for (elIdx = 0; elIdx < 6; ++elIdx) {
            d1 = pDataValues[TimeIndex];
            d2 = pDataValues[TimeIndex + 1];
            (&helicopter_B.FromWorkspace1[0])[elIdx] = (real_T) rtInterpolate(d1,
              d2, f1, f2);
            pDataValues += 141;
          }
        }
      }
    }
  }

  /* Gain: '<Root>/Gain' incorporates:
   *  Sum: '<Root>/Minus2'
   */
  for (i = 0; i < 6; i++) {
    tmp[i] = helicopter_B.Gain1[i] - helicopter_B.FromWorkspace1[i];
  }

  for (i = 0; i < 2; i++) {
    /* Sum: '<Root>/Minus' incorporates:
     *  Gain: '<Root>/Gain'
     */
    rtb_Sum1 = 0.0;
    for (i_0 = 0; i_0 < 6; i_0++) {
      rtb_Sum1 += helicopter_P.K[6 * i + i_0] * tmp[i_0];
    }

    helicopter_B.Minus[i] = rtb_FromWorkspace[i] - rtb_Sum1;

    /* End of Sum: '<Root>/Minus' */
  }

  /* Sum: '<Root>/Add' incorporates:
   *  Constant: '<Root>/pitch_ref'
   */
  helicopter_B.Add = helicopter_B.Minus[0] + helicopter_P.pitch_ref_Value;

  /* Sum: '<S5>/Sum3' incorporates:
   *  Gain: '<S5>/K_pd'
   *  Gain: '<S5>/K_pp'
   *  Sum: '<S5>/Sum2'
   */
  helicopter_B.Sum3_h = (helicopter_B.Add - helicopter_B.Gain1[2]) *
    helicopter_P.K_pp - helicopter_P.K_pd * helicopter_B.Gain1[3];

  /* Sum: '<Root>/Sum1' incorporates:
   *  Constant: '<Root>/Vd_bias'
   */
  rtb_Sum1 = helicopter_B.Sum3_h + helicopter_P.Vd_ff;

  /* Integrator: '<S3>/Integrator' */
  /* Limited  Integrator  */
  if (helicopter_X.Integrator_CSTATE >= helicopter_P.Integrator_UpperSat) {
    helicopter_X.Integrator_CSTATE = helicopter_P.Integrator_UpperSat;
  } else {
    if (helicopter_X.Integrator_CSTATE <= helicopter_P.Integrator_LowerSat) {
      helicopter_X.Integrator_CSTATE = helicopter_P.Integrator_LowerSat;
    }
  }

  /* Sum: '<S3>/Sum' */
  rtb_Sum = helicopter_B.Minus[1] - helicopter_B.Gain1[4];

  /* Sum: '<S3>/Sum1' incorporates:
   *  Gain: '<S3>/K_ed'
   *  Gain: '<S3>/K_ep'
   *  Integrator: '<S3>/Integrator'
   */
  helicopter_B.Sum1 = (helicopter_P.K_ep * rtb_Sum +
                       helicopter_X.Integrator_CSTATE) - helicopter_P.K_ed *
    helicopter_B.Gain1[5];

  /* Sum: '<Root>/Sum2' incorporates:
   *  Constant: '<Root>/Vs_bias'
   */
  rtb_Derivative = helicopter_B.Sum1 + helicopter_P.Vs_ff;

  /* Gain: '<S1>/Front gain' incorporates:
   *  Sum: '<S1>/Add'
   */
  helicopter_B.Frontgain = (rtb_Sum1 + rtb_Derivative) *
    helicopter_P.Frontgain_Gain;

  /* Gain: '<S1>/Back gain' incorporates:
   *  Sum: '<S1>/Subtract'
   */
  helicopter_B.Backgain = (rtb_Derivative - rtb_Sum1) *
    helicopter_P.Backgain_Gain;
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* SignalConversion generated from: '<Root>/To File2' incorporates:
     *  SignalConversion generated from: '<Root>/To File1'
     */
    rtb_TmpSignalConversionAtToFile[0] = helicopter_B.Frontgain;
    rtb_TmpSignalConversionAtToFile[1] = helicopter_B.Backgain;

    /* ToFile: '<Root>/To File1' */
    {
      if (!(++helicopter_DW.ToFile1_IWORK.Decimation % 1) &&
          (helicopter_DW.ToFile1_IWORK.Count * (2 + 1)) + 1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_DW.ToFile1_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[2 + 1];
          helicopter_DW.ToFile1_IWORK.Decimation = 0;
          u[0] = helicopter_M->Timing.t[1];
          u[1] = rtb_TmpSignalConversionAtToFile[0];
          u[2] = rtb_TmpSignalConversionAtToFile[1];
          if (fwrite(u, sizeof(real_T), 2 + 1, fp) != 2 + 1) {
            rtmSetErrorStatus(helicopter_M,
                              "Error writing to MAT-file DataFiles/voltages.mat");
            return;
          }

          if (((++helicopter_DW.ToFile1_IWORK.Count) * (2 + 1))+1 >= 100000000)
          {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file DataFiles/voltages.mat.\n");
          }
        }
      }
    }

    /* SignalConversion generated from: '<Root>/To File2' */
    rtb_TmpSignalConversionAtToFile[0] = helicopter_B.Sum3_h;
    rtb_TmpSignalConversionAtToFile[1] = helicopter_B.Sum1;

    /* ToFile: '<Root>/To File2' */
    {
      if (!(++helicopter_DW.ToFile2_IWORK.Decimation % 1) &&
          (helicopter_DW.ToFile2_IWORK.Count * (2 + 1)) + 1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_DW.ToFile2_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[2 + 1];
          helicopter_DW.ToFile2_IWORK.Decimation = 0;
          u[0] = helicopter_M->Timing.t[1];
          u[1] = rtb_TmpSignalConversionAtToFile[0];
          u[2] = rtb_TmpSignalConversionAtToFile[1];
          if (fwrite(u, sizeof(real_T), 2 + 1, fp) != 2 + 1) {
            rtmSetErrorStatus(helicopter_M,
                              "Error writing to MAT-file DataFiles/controller_output.mat");
            return;
          }

          if (((++helicopter_DW.ToFile2_IWORK.Count) * (2 + 1))+1 >= 100000000)
          {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file DataFiles/controller_output.mat.\n");
          }
        }
      }
    }

    /* ToFile: '<Root>/To File3' */
    {
      if (!(++helicopter_DW.ToFile3_IWORK.Decimation % 1) &&
          (helicopter_DW.ToFile3_IWORK.Count * (1 + 1)) + 1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_DW.ToFile3_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[1 + 1];
          helicopter_DW.ToFile3_IWORK.Decimation = 0;
          u[0] = helicopter_M->Timing.t[1];
          u[1] = helicopter_B.Add;
          if (fwrite(u, sizeof(real_T), 1 + 1, fp) != 1 + 1) {
            rtmSetErrorStatus(helicopter_M,
                              "Error writing to MAT-file DataFiles/u_ts_with_bias.mat");
            return;
          }

          if (((++helicopter_DW.ToFile3_IWORK.Count) * (1 + 1))+1 >= 100000000)
          {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file DataFiles/u_ts_with_bias.mat.\n");
          }
        }
      }
    }

    /* ToFile: '<Root>/To File4' */
    {
      if (!(++helicopter_DW.ToFile4_IWORK.Decimation % 1) &&
          (helicopter_DW.ToFile4_IWORK.Count * (6 + 1)) + 1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_DW.ToFile4_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[6 + 1];
          helicopter_DW.ToFile4_IWORK.Decimation = 0;
          u[0] = helicopter_M->Timing.t[1];
          u[1] = helicopter_B.FromWorkspace1[0];
          u[2] = helicopter_B.FromWorkspace1[1];
          u[3] = helicopter_B.FromWorkspace1[2];
          u[4] = helicopter_B.FromWorkspace1[3];
          u[5] = helicopter_B.FromWorkspace1[4];
          u[6] = helicopter_B.FromWorkspace1[5];
          if (fwrite(u, sizeof(real_T), 6 + 1, fp) != 6 + 1) {
            rtmSetErrorStatus(helicopter_M,
                              "Error writing to MAT-file DataFiles/x_ts.mat");
            return;
          }

          if (((++helicopter_DW.ToFile4_IWORK.Count) * (6 + 1))+1 >= 100000000)
          {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file DataFiles/x_ts.mat.\n");
          }
        }
      }
    }
  }

  /* If: '<S3>/If' incorporates:
   *  Clock: '<S3>/Clock'
   *  Gain: '<S3>/K_ei'
   *  Inport: '<S6>/In1'
   */
  if (rtmIsMajorTimeStep(helicopter_M)) {
    rtAction = (int8_T)!(helicopter_M->Timing.t[0] >= 2.0);
    helicopter_DW.If_ActiveSubsystem = rtAction;
  } else {
    rtAction = helicopter_DW.If_ActiveSubsystem;
  }

  if (rtAction == 0) {
    /* Outputs for IfAction SubSystem: '<S3>/If Action Subsystem' incorporates:
     *  ActionPort: '<S6>/Action Port'
     */
    helicopter_B.In1 = helicopter_P.K_ei * rtb_Sum;
    if (rtmIsMajorTimeStep(helicopter_M)) {
      srUpdateBC(helicopter_DW.IfActionSubsystem_SubsysRanBC);
    }

    /* End of Outputs for SubSystem: '<S3>/If Action Subsystem' */
  }

  /* End of If: '<S3>/If' */
  if (rtmIsMajorTimeStep(helicopter_M)) {
  }

  /* Derivative: '<S4>/Derivative' */
  rtb_Sum1 = helicopter_M->Timing.t[0];
  if ((helicopter_DW.TimeStampA >= rtb_Sum1) && (helicopter_DW.TimeStampB >=
       rtb_Sum1)) {
    rtb_Derivative = 0.0;
  } else {
    rtb_Sum = helicopter_DW.TimeStampA;
    lastU = &helicopter_DW.LastUAtTimeA;
    if (helicopter_DW.TimeStampA < helicopter_DW.TimeStampB) {
      if (helicopter_DW.TimeStampB < rtb_Sum1) {
        rtb_Sum = helicopter_DW.TimeStampB;
        lastU = &helicopter_DW.LastUAtTimeB;
      }
    } else {
      if (helicopter_DW.TimeStampA >= rtb_Sum1) {
        rtb_Sum = helicopter_DW.TimeStampB;
        lastU = &helicopter_DW.LastUAtTimeB;
      }
    }

    rtb_Derivative = (helicopter_B.PitchCounttorad - *lastU) / (rtb_Sum1 -
      rtb_Sum);
  }

  /* End of Derivative: '<S4>/Derivative' */

  /* Gain: '<S11>/Gain' */
  helicopter_B.Gain_l = helicopter_P.Gain_Gain_a1 * rtb_Derivative;
  if (rtmIsMajorTimeStep(helicopter_M)) {
  }

  /* Saturate: '<S4>/Back motor: Saturation' */
  if (helicopter_B.Backgain > helicopter_P.BackmotorSaturation_UpperSat) {
    /* Saturate: '<S4>/Back motor: Saturation' */
    helicopter_B.BackmotorSaturation = helicopter_P.BackmotorSaturation_UpperSat;
  } else if (helicopter_B.Backgain < helicopter_P.BackmotorSaturation_LowerSat)
  {
    /* Saturate: '<S4>/Back motor: Saturation' */
    helicopter_B.BackmotorSaturation = helicopter_P.BackmotorSaturation_LowerSat;
  } else {
    /* Saturate: '<S4>/Back motor: Saturation' */
    helicopter_B.BackmotorSaturation = helicopter_B.Backgain;
  }

  /* End of Saturate: '<S4>/Back motor: Saturation' */
  if (rtmIsMajorTimeStep(helicopter_M)) {
  }

  /* Saturate: '<S4>/Front motor: Saturation' */
  if (helicopter_B.Frontgain > helicopter_P.FrontmotorSaturation_UpperSat) {
    /* Saturate: '<S4>/Front motor: Saturation' */
    helicopter_B.FrontmotorSaturation =
      helicopter_P.FrontmotorSaturation_UpperSat;
  } else if (helicopter_B.Frontgain < helicopter_P.FrontmotorSaturation_LowerSat)
  {
    /* Saturate: '<S4>/Front motor: Saturation' */
    helicopter_B.FrontmotorSaturation =
      helicopter_P.FrontmotorSaturation_LowerSat;
  } else {
    /* Saturate: '<S4>/Front motor: Saturation' */
    helicopter_B.FrontmotorSaturation = helicopter_B.Frontgain;
  }

  /* End of Saturate: '<S4>/Front motor: Saturation' */
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* S-Function (hil_write_analog_block): '<S4>/HIL Write Analog' */

    /* S-Function Block: helicopter/Helicopter_interface/HIL Write Analog (hil_write_analog_block) */
    {
      t_error result;
      helicopter_DW.HILWriteAnalog_Buffer[0] = helicopter_B.FrontmotorSaturation;
      helicopter_DW.HILWriteAnalog_Buffer[1] = helicopter_B.BackmotorSaturation;
      result = hil_write_analog(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILWriteAnalog_channels, 2,
        &helicopter_DW.HILWriteAnalog_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
      }
    }
  }
}

/* Model update function */
void helicopter_update(void)
{
  real_T *lastU;

  /* Update for Derivative: '<S4>/Derivative' */
  if (helicopter_DW.TimeStampA == (rtInf)) {
    helicopter_DW.TimeStampA = helicopter_M->Timing.t[0];
    lastU = &helicopter_DW.LastUAtTimeA;
  } else if (helicopter_DW.TimeStampB == (rtInf)) {
    helicopter_DW.TimeStampB = helicopter_M->Timing.t[0];
    lastU = &helicopter_DW.LastUAtTimeB;
  } else if (helicopter_DW.TimeStampA < helicopter_DW.TimeStampB) {
    helicopter_DW.TimeStampA = helicopter_M->Timing.t[0];
    lastU = &helicopter_DW.LastUAtTimeA;
  } else {
    helicopter_DW.TimeStampB = helicopter_M->Timing.t[0];
    lastU = &helicopter_DW.LastUAtTimeB;
  }

  *lastU = helicopter_B.PitchCounttorad;

  /* End of Update for Derivative: '<S4>/Derivative' */
  if (rtmIsMajorTimeStep(helicopter_M)) {
    rt_ertODEUpdateContinuousStates(&helicopter_M->solverInfo);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++helicopter_M->Timing.clockTick0)) {
    ++helicopter_M->Timing.clockTickH0;
  }

  helicopter_M->Timing.t[0] = rtsiGetSolverStopTime(&helicopter_M->solverInfo);

  {
    /* Update absolute timer for sample time: [0.002s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick1"
     * and "Timing.stepSize1". Size of "clockTick1" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++helicopter_M->Timing.clockTick1)) {
      ++helicopter_M->Timing.clockTickH1;
    }

    helicopter_M->Timing.t[1] = helicopter_M->Timing.clockTick1 *
      helicopter_M->Timing.stepSize1 + helicopter_M->Timing.clockTickH1 *
      helicopter_M->Timing.stepSize1 * 4294967296.0;
  }
}

/* Derivatives for root system: '<Root>' */
void helicopter_derivatives(void)
{
  XDot_helicopter_T *_rtXdot;
  boolean_T lsat;
  boolean_T usat;
  _rtXdot = ((XDot_helicopter_T *) helicopter_M->derivs);

  /* Derivatives for TransferFcn: '<S4>/Travel: Transfer Fcn' */
  _rtXdot->TravelTransferFcn_CSTATE = 0.0;
  _rtXdot->TravelTransferFcn_CSTATE += helicopter_P.TravelTransferFcn_A *
    helicopter_X.TravelTransferFcn_CSTATE;
  _rtXdot->TravelTransferFcn_CSTATE += helicopter_B.TravelCounttorad;

  /* Derivatives for TransferFcn: '<S4>/Pitch: Transfer Fcn' */
  _rtXdot->PitchTransferFcn_CSTATE = 0.0;
  _rtXdot->PitchTransferFcn_CSTATE += helicopter_P.PitchTransferFcn_A *
    helicopter_X.PitchTransferFcn_CSTATE;
  _rtXdot->PitchTransferFcn_CSTATE += helicopter_B.PitchCounttorad;

  /* Derivatives for TransferFcn: '<S4>/Elevation: Transfer Fcn' */
  _rtXdot->ElevationTransferFcn_CSTATE = 0.0;
  _rtXdot->ElevationTransferFcn_CSTATE += helicopter_P.ElevationTransferFcn_A *
    helicopter_X.ElevationTransferFcn_CSTATE;
  _rtXdot->ElevationTransferFcn_CSTATE += helicopter_B.ElevationCounttorad;

  /* Derivatives for Integrator: '<S3>/Integrator' */
  lsat = (helicopter_X.Integrator_CSTATE <= helicopter_P.Integrator_LowerSat);
  usat = (helicopter_X.Integrator_CSTATE >= helicopter_P.Integrator_UpperSat);
  if (((!lsat) && (!usat)) || (lsat && (helicopter_B.In1 > 0.0)) || (usat &&
       (helicopter_B.In1 < 0.0))) {
    _rtXdot->Integrator_CSTATE = helicopter_B.In1;
  } else {
    /* in saturation */
    _rtXdot->Integrator_CSTATE = 0.0;
  }

  /* End of Derivatives for Integrator: '<S3>/Integrator' */
}

/* Model initialize function */
void helicopter_initialize(void)
{
  /* Start for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: helicopter/HIL Initialize (hil_initialize_block) */
  {
    t_int result;
    t_boolean is_switching;
    result = hil_open("q8_usb", "0", &helicopter_DW.HILInitialize_Card);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_M, _rt_error_message);
      return;
    }

    is_switching = false;
    result = hil_set_card_specific_options(helicopter_DW.HILInitialize_Card,
      "update_rate=normal;decimation=1", 32);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_M, _rt_error_message);
      return;
    }

    result = hil_watchdog_clear(helicopter_DW.HILInitialize_Card);
    if (result < 0 && result != -QERR_HIL_WATCHDOG_CLEAR) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_M, _rt_error_message);
      return;
    }

    if ((helicopter_P.HILInitialize_AIPStart && !is_switching) ||
        (helicopter_P.HILInitialize_AIPEnter && is_switching)) {
      {
        int_T i1;
        real_T *dw_AIMinimums = &helicopter_DW.HILInitialize_AIMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMinimums[i1] = (helicopter_P.HILInitialize_AILow);
        }
      }

      {
        int_T i1;
        real_T *dw_AIMaximums = &helicopter_DW.HILInitialize_AIMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMaximums[i1] = helicopter_P.HILInitialize_AIHigh;
        }
      }

      result = hil_set_analog_input_ranges(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_AIChannels, 8U,
        &helicopter_DW.HILInitialize_AIMinimums[0],
        &helicopter_DW.HILInitialize_AIMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_AOPStart && !is_switching) ||
        (helicopter_P.HILInitialize_AOPEnter && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOMinimums = &helicopter_DW.HILInitialize_AOMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMinimums[i1] = (helicopter_P.HILInitialize_AOLow);
        }
      }

      {
        int_T i1;
        real_T *dw_AOMaximums = &helicopter_DW.HILInitialize_AOMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMaximums[i1] = helicopter_P.HILInitialize_AOHigh;
        }
      }

      result = hil_set_analog_output_ranges(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_AOChannels, 8U,
        &helicopter_DW.HILInitialize_AOMinimums[0],
        &helicopter_DW.HILInitialize_AOMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_AOStart && !is_switching) ||
        (helicopter_P.HILInitialize_AOEnter && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_P.HILInitialize_AOInitial;
        }
      }

      result = hil_write_analog(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_AOChannels, 8U,
        &helicopter_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if (helicopter_P.HILInitialize_AOReset) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_P.HILInitialize_AOWatchdog;
        }
      }

      result = hil_watchdog_set_analog_expiration_state
        (helicopter_DW.HILInitialize_Card, helicopter_P.HILInitialize_AOChannels,
         8U, &helicopter_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_EIPStart && !is_switching) ||
        (helicopter_P.HILInitialize_EIPEnter && is_switching)) {
      {
        int_T i1;
        int32_T *dw_QuadratureModes =
          &helicopter_DW.HILInitialize_QuadratureModes[0];
        for (i1=0; i1 < 8; i1++) {
          dw_QuadratureModes[i1] = helicopter_P.HILInitialize_EIQuadrature;
        }
      }

      result = hil_set_encoder_quadrature_mode(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_EIChannels, 8U, (t_encoder_quadrature_mode *)
        &helicopter_DW.HILInitialize_QuadratureModes[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_EIStart && !is_switching) ||
        (helicopter_P.HILInitialize_EIEnter && is_switching)) {
      {
        int_T i1;
        int32_T *dw_InitialEICounts =
          &helicopter_DW.HILInitialize_InitialEICounts[0];
        for (i1=0; i1 < 8; i1++) {
          dw_InitialEICounts[i1] = helicopter_P.HILInitialize_EIInitial;
        }
      }

      result = hil_set_encoder_counts(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_EIChannels, 8U,
        &helicopter_DW.HILInitialize_InitialEICounts[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_POPStart && !is_switching) ||
        (helicopter_P.HILInitialize_POPEnter && is_switching)) {
      uint32_T num_duty_cycle_modes = 0;
      uint32_T num_frequency_modes = 0;

      {
        int_T i1;
        int32_T *dw_POModeValues = &helicopter_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] = helicopter_P.HILInitialize_POModes;
        }
      }

      result = hil_set_pwm_mode(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_POChannels, 8U, (t_pwm_mode *)
        &helicopter_DW.HILInitialize_POModeValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        const uint32_T *p_HILInitialize_POChannels =
          helicopter_P.HILInitialize_POChannels;
        int32_T *dw_POModeValues = &helicopter_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          if (dw_POModeValues[i1] == PWM_DUTY_CYCLE_MODE || dw_POModeValues[i1] ==
              PWM_ONE_SHOT_MODE || dw_POModeValues[i1] == PWM_TIME_MODE ||
              dw_POModeValues[i1] == PWM_RAW_MODE) {
            helicopter_DW.HILInitialize_POSortedChans[num_duty_cycle_modes] =
              (p_HILInitialize_POChannels[i1]);
            helicopter_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes] =
              helicopter_P.HILInitialize_POFrequency;
            num_duty_cycle_modes++;
          } else {
            helicopter_DW.HILInitialize_POSortedChans[7U - num_frequency_modes] =
              (p_HILInitialize_POChannels[i1]);
            helicopter_DW.HILInitialize_POSortedFreqs[7U - num_frequency_modes] =
              helicopter_P.HILInitialize_POFrequency;
            num_frequency_modes++;
          }
        }
      }

      if (num_duty_cycle_modes > 0) {
        result = hil_set_pwm_frequency(helicopter_DW.HILInitialize_Card,
          &helicopter_DW.HILInitialize_POSortedChans[0], num_duty_cycle_modes,
          &helicopter_DW.HILInitialize_POSortedFreqs[0]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_M, _rt_error_message);
          return;
        }
      }

      if (num_frequency_modes > 0) {
        result = hil_set_pwm_duty_cycle(helicopter_DW.HILInitialize_Card,
          &helicopter_DW.HILInitialize_POSortedChans[num_duty_cycle_modes],
          num_frequency_modes,
          &helicopter_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_M, _rt_error_message);
          return;
        }
      }

      {
        int_T i1;
        int32_T *dw_POModeValues = &helicopter_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] = helicopter_P.HILInitialize_POConfiguration;
        }
      }

      {
        int_T i1;
        int32_T *dw_POAlignValues = &helicopter_DW.HILInitialize_POAlignValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POAlignValues[i1] = helicopter_P.HILInitialize_POAlignment;
        }
      }

      {
        int_T i1;
        int32_T *dw_POPolarityVals =
          &helicopter_DW.HILInitialize_POPolarityVals[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POPolarityVals[i1] = helicopter_P.HILInitialize_POPolarity;
        }
      }

      result = hil_set_pwm_configuration(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_POChannels, 8U,
        (t_pwm_configuration *) &helicopter_DW.HILInitialize_POModeValues[0],
        (t_pwm_alignment *) &helicopter_DW.HILInitialize_POAlignValues[0],
        (t_pwm_polarity *) &helicopter_DW.HILInitialize_POPolarityVals[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        real_T *dw_POSortedFreqs = &helicopter_DW.HILInitialize_POSortedFreqs[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POSortedFreqs[i1] = helicopter_P.HILInitialize_POLeading;
        }
      }

      {
        int_T i1;
        real_T *dw_POValues = &helicopter_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_P.HILInitialize_POTrailing;
        }
      }

      result = hil_set_pwm_deadband(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_POChannels, 8U,
        &helicopter_DW.HILInitialize_POSortedFreqs[0],
        &helicopter_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_POStart && !is_switching) ||
        (helicopter_P.HILInitialize_POEnter && is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_P.HILInitialize_POInitial;
        }
      }

      result = hil_write_pwm(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_POChannels, 8U,
        &helicopter_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if (helicopter_P.HILInitialize_POReset) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_P.HILInitialize_POWatchdog;
        }
      }

      result = hil_watchdog_set_pwm_expiration_state
        (helicopter_DW.HILInitialize_Card, helicopter_P.HILInitialize_POChannels,
         8U, &helicopter_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }
  }

  /* Start for S-Function (hil_read_encoder_timebase_block): '<S4>/HIL Read Encoder Timebase' */

  /* S-Function Block: helicopter/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
  {
    t_error result;
    result = hil_task_create_encoder_reader(helicopter_DW.HILInitialize_Card,
      helicopter_P.HILReadEncoderTimebase_SamplesI,
      helicopter_P.HILReadEncoderTimebase_Channels, 3,
      &helicopter_DW.HILReadEncoderTimebase_Task);
    if (result >= 0) {
      result = hil_task_set_buffer_overflow_mode
        (helicopter_DW.HILReadEncoderTimebase_Task, (t_buffer_overflow_mode)
         (helicopter_P.HILReadEncoderTimebase_Overflow - 1));
    }

    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_M, _rt_error_message);
    }
  }

  /* Start for ToFile: '<Root>/To File' */
  {
    FILE *fp = (NULL);
    char fileName[509] = "DataFiles/loop_variables.mat";
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_M,
                        "Error creating .mat file DataFiles/loop_variables.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp, 6 + 1, 0, "y_loop_variables")) {
      rtmSetErrorStatus(helicopter_M,
                        "Error writing mat file header to file DataFiles/loop_variables.mat");
      return;
    }

    helicopter_DW.ToFile_IWORK.Count = 0;
    helicopter_DW.ToFile_IWORK.Decimation = -1;
    helicopter_DW.ToFile_PWORK.FilePtr = fp;
  }

  /* Start for FromWorkspace: '<Root>/From Workspace' */
  {
    static real_T pTimeValues0[] = { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75,
      2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.25, 4.5, 4.75, 5.0,
      5.25, 5.5, 5.75, 6.0, 6.25, 6.5, 6.75, 7.0, 7.25, 7.5, 7.75, 8.0, 8.25,
      8.5, 8.75, 9.0, 9.25, 9.5, 9.75, 10.0, 10.25, 10.5, 10.75, 11.0, 11.25,
      11.5, 11.75, 12.0, 12.25, 12.5, 12.75, 13.0, 13.25, 13.5, 13.75, 14.0,
      14.25, 14.5, 14.75, 15.0, 15.25, 15.5, 15.75, 16.0, 16.25, 16.5, 16.75,
      17.0, 17.25, 17.5, 17.75, 18.0, 18.25, 18.5, 18.75, 19.0, 19.25, 19.5,
      19.75, 20.0, 20.25, 20.5, 20.75, 21.0, 21.25, 21.5, 21.75, 22.0, 22.25,
      22.5, 22.75, 23.0, 23.25, 23.5, 23.75, 24.0, 24.25, 24.5, 24.75, 25.0,
      25.25, 25.5, 25.75, 26.0, 26.25, 26.5, 26.75, 27.0, 27.25, 27.5, 27.75,
      28.0, 28.25, 28.5, 28.75, 29.0, 29.25, 29.5, 29.75, 30.0, 30.25, 30.5,
      30.75, 31.0, 31.25, 31.5, 31.75, 32.0, 32.25, 32.5, 32.75, 33.0, 33.25,
      33.5, 33.75, 34.0, 34.25, 34.5, 34.75, 35.0 } ;

    static real_T pDataValues0[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.52359877559829882,
      0.52359877559829882, 0.52359877559829882, 0.52359877559829882,
      0.52359877559829882, 0.52359877559829882, 0.52359877559829882,
      0.52359877559829882, 0.52359877559829882, 0.52359877559829882,
      0.52359877559829882, 0.52359877559829882, 0.52359877559829882,
      0.0086781982367606682, -0.52359877559829882, -0.52359877559829882,
      -0.52359877559829882, -0.52359877559829882, -0.52359877559829882,
      -0.52359877559829882, -0.52359877559829882, -0.52359877559829882,
      -0.52359877559829882, -0.52359877559829882, -0.52359877559829882,
      -0.52359877559829882, -0.52359877559829882, -0.52359877559829882,
      -0.47305484897074335, -0.19355442912569013, 0.0022231107527107196,
      0.12607800073739317, 0.19193438017770253, 0.21417783515630456,
      0.2062629357334666, 0.17977721095412497, 0.14402883071445219,
      0.10595954400841509, 0.0702841847117688, 0.03982996341499314,
      0.015898058457823331, -0.0013333584949557762, -0.012434118346624929,
      -0.018401091950989105, -0.020414546361924437, -0.019645494192183671,
      -0.017140481111656031, -0.013762894160162974, -0.010171785141191526,
      -0.0067868979218308062, -0.0039028852972235423, -0.0016236941992972187,
      2.2595179541828964E-5, 0.0010935599215435774, 0.0016834935747590033,
      0.0018925238278191627, 0.0018454239185206655, 0.0016278222883152722,
      0.0013220315185050386, 0.00098743582303547684, 0.0006726718412894254,
      0.00039216094692176589, 0.00017869206041098816, 9.4773558753379545E-6,
      -9.8955961092476617E-5, -0.00016250715537290846, -0.00018891649706678678,
      -0.00018702451466721557, -0.00016292466675145743, -0.00013416192303490965,
      -0.00010263067529267327, -6.6800057730211939E-5, -3.6399384888897719E-5,
      -1.1101850096180314E-5, 4.3752065790342975E-6, 1.5297556342168426E-5,
      2.686669023228744E-5, 2.2092488536616195E-5, 2.5886573973936758E-5,
      1.5877761341604023E-5, 1.0632429285495727E-5, 5.4786211875777148E-6,
      4.7562217028814822E-6, -2.6900289914102387E-6, -5.5388605634632641E-6,
      -5.9755107786490667E-6, -2.8123587778239494E-6, 2.0041666164580215E-6,
      2.9681983617391473E-6, 2.2849512855202023E-6, 1.9545059867035312E-6,
      4.2285511777868215E-6, 5.7603572495914943E-6, 6.850145123804601E-6,
      -1.4388561143763507E-7, 8.5260998851760015E-6, 8.19563976410279E-6,
      7.7782700056412675E-6, 9.1892451285059991E-6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.28636533491324984, 0.30386029201045406,
      0.32056623401028828, 0.33583308287787939, 0.34892897035200393,
      0.35890571060574183, 0.36460366031239294, 0.36460631496748858,
      0.35716452092775219, 0.34016105376916489, 0.31100488343359389,
      0.26657836303905574, 0.20310801337577772, 0.11606658893181314,
      1.2393188274576456E-5, 5.5324524405029379E-6, 3.79868551845489E-6,
      3.02233673302477E-6, 1.4631422637046047E-6, -6.188411546869692E-6,
      -6.2202012966025587E-6, -1.0427709833783651E-5, -9.2600848117362162E-6,
      -7.4172027466367269E-6, -6.7263112996235474E-6, -6.6618363353514108E-6,
      -1.0272488898894709E-5, -1.0054556547561274E-5, -1.0840129026086246E-5,
      -1.2584868249779409E-5, -1.2492070191307468E-5, -1.2595482409569387E-5,
      -9.11290907635507E-6, -8.278843632172595E-6, -6.0155405285237043E-6,
      -5.2018128972845323E-6, -7.8115171493627875E-6, -8.1032350518360732E-6,
      -4.0230088432705739E-7, -9.335693501213716E-7, -2.6408893335758081E-6,
      -4.5310077666050313E-6, 4.0654070307695247E-6, 3.4796377215577115E-6,
      2.6168818256489059E-6, 7.5884660211599433E-6, 6.459041182273429E-6,
      7.2483021121294958E-6, 8.68368130275236E-6, 3.6143773975267352E-6,
      7.75514466872279E-6, 7.9200419347245836E-6, 8.0010519444958121E-6,
      5.918670149288755E-6, 8.11135898170969E-6, 6.3451826840736064E-6,
      5.8512587919948261E-6, 8.4764268832998387E-6, 6.3322932158260765E-6,
      8.3019443054207426E-6, 7.0524313575382172E-6, 6.58013628856808E-6,
      5.77471568704647E-6, 7.9888977632902277E-6, 7.1238254805513875E-6,
      7.6372730186958857E-6, 4.2401039026336063E-6, 3.3031165107373364E-6,
      3.1984802003462342E-6, 1.6127468656450176E-6, 1.9056123610336768E-7,
      1.2347887028792492E-7, -1.1028584697237832E-6, 4.5213678375720436E-7,
      1.755652495479337E-7, -3.6613965002526541E-6, -3.60740817725656E-6,
      -5.1822224052572939E-6, -4.9778479736096489E-6, 5.2861093055043568E-8,
      -3.3840151638023416E-6, -2.0863782272761738E-6, -4.7744624618655447E-7,
      9.2792969290986783E-7, 6.1435120302352117E-7, 9.3168265192884275E-7,
      -9.077725571261052E-7, 2.7910612678535259E-6, 1.2087128609643716E-6,
      -2.6718970553466921E-6, 2.6203686971904765E-7, 2.4506120833308153E-6,
      -2.4825804372387488E-6, -1.4911856433466062E-6, -1.2100640236351492E-6,
      -6.71942163596174E-7, 3.74463657650687E-7, 1.0240646847977171E-6,
      1.2278403840818872E-6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } ;

    helicopter_DW.FromWorkspace_PWORK.TimePtr = (void *) pTimeValues0;
    helicopter_DW.FromWorkspace_PWORK.DataPtr = (void *) pDataValues0;
    helicopter_DW.FromWorkspace_IWORK.PrevIndex = 0;
  }

  /* Start for FromWorkspace: '<Root>/From Workspace1' */
  {
    static real_T pTimeValues0[] = { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75,
      2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.25, 4.5, 4.75, 5.0,
      5.25, 5.5, 5.75, 6.0, 6.25, 6.5, 6.75, 7.0, 7.25, 7.5, 7.75, 8.0, 8.25,
      8.5, 8.75, 9.0, 9.25, 9.5, 9.75, 10.0, 10.25, 10.5, 10.75, 11.0, 11.25,
      11.5, 11.75, 12.0, 12.25, 12.5, 12.75, 13.0, 13.25, 13.5, 13.75, 14.0,
      14.25, 14.5, 14.75, 15.0, 15.25, 15.5, 15.75, 16.0, 16.25, 16.5, 16.75,
      17.0, 17.25, 17.5, 17.75, 18.0, 18.25, 18.5, 18.75, 19.0, 19.25, 19.5,
      19.75, 20.0, 20.25, 20.5, 20.75, 21.0, 21.25, 21.5, 21.75, 22.0, 22.25,
      22.5, 22.75, 23.0, 23.25, 23.5, 23.75, 24.0, 24.25, 24.5, 24.75, 25.0,
      25.25, 25.5, 25.75, 26.0, 26.25, 26.5, 26.75, 27.0, 27.25, 27.5, 27.75,
      28.0, 28.25, 28.5, 28.75, 29.0, 29.25, 29.5, 29.75, 30.0, 30.25, 30.5,
      30.75, 31.0, 31.25, 31.5, 31.75, 32.0, 32.25, 32.5, 32.75, 33.0, 33.25,
      33.5, 33.75, 34.0, 34.25, 34.5, 34.75, 35.0 } ;

    static real_T pDataValues0[] = { 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1378421413625261, 3.1262155534579983,
      3.1033093000299643, 3.0666274151911783, 3.0144539223941584,
      2.9456562771175667, 2.8595077632935446, 2.7555515879651526,
      2.633505110490284, 2.4931956060320961, 2.334518576064299,
      2.1574113214711188, 1.9618364840294067, 1.7514602868971698,
      1.5341395501443036, 1.3174077433423959, 1.1071749949490501,
      0.90766357172222456, 0.72173015323724843, 0.55123985133270137,
      0.39738014345948386, 0.26089305848241628, 0.14223561622390354,
      0.041686074756982786, -0.040587730534282447, -0.10448535379451611,
      -0.1499470747864837, -0.17693758612047861, -0.18579815869527372,
      -0.17891691871879739, -0.16021991544334976, -0.13419056134209686,
      -0.10504470623730507, -0.076222881432214618, -0.050177682754625488,
      -0.02837648895447905, -0.011435442532783441, 0.00068427025587445612,
      0.00846726847860537, 0.012653034037643557, 0.014079822813517244,
      0.013569450789870027, 0.011851857340634953, 0.0095241175799686585,
      0.0070366297475535286, 0.0046987080022139471, 0.0026964091659011534,
      0.0011166347208271753, -2.7028828152188515E-5, -0.00077016980441364292,
      -0.00117670332169969, -0.0013224962708241667, -0.0012834601994685709,
      -0.0011278193432824159, -0.00091188278399324536, -0.00067844995361928014,
      -0.00045722935582870517, -0.00026639484671700791, -0.00011476048141446451,
      -4.1274366173348633E-6, 6.8443883875444686E-5, 0.00010874117754876518,
      0.00012377075089976122, 0.00012070083917598489, 0.00010607379594033187,
      8.5373033022504216E-5, 6.2840410295529554E-5, 4.1469689364559976E-5,
      2.3098219360664065E-5, 8.63544532661195E-6, -1.6998980764516162E-6,
      -8.1989627946137864E-6, -1.1465941725018973E-5, -1.2258578585921571E-5,
      -1.1338484510288302E-5, -9.3924505227231376E-6, -7.028428518509369E-6,
      -4.6732139260369708E-6, -2.6395531152310985E-6, -1.0706688442587887E-6,
      8.0716848325651609E-9, 6.4900590756803606E-7, 9.2255309355439689E-7,
      9.4367949189334161E-7, 8.3795218770416258E-7, 7.118453882385787E-7,
      6.2183917631946339E-7, 5.6335266131190643E-7, 5.0735632657850971E-7,
      4.28197488372524E-7, 3.0880658674679922E-7, 1.2187816895145655E-7,
      -1.6843247256106081E-7, -6.0110071347572461E-7, -1.1580585455140955E-6,
      -1.8697437872905431E-6, -2.7727365422225528E-6, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.015002048909068423,
      -0.046506351618112111, -0.091625013712135384, -0.14672753935514368,
      -0.2086939711880792, -0.27519058110636668, -0.34459405529608833,
      -0.41582470131356863, -0.48818590989947458, -0.56123801783275173,
      -0.63470811987118847, -0.708429018372721, -0.782299349766848,
      -0.84150478852894739, -0.86928294701146491, -0.86692722720763082,
      -0.84093099357338319, -0.79804569290730221, -0.74373367393990464,
      -0.68196120761818835, -0.61543883149286993, -0.5459483399082703,
      -0.47462976903405107, -0.402198165867683, -0.32909522116506096,
      -0.25559049304093462, -0.18184688396787041, -0.1079620453359796,
      -0.035442290299180362, 0.027524959905905345, 0.074788013101790521,
      0.1041174164050116, 0.11658342041916717, 0.11528729922036182,
      0.1041807947103565, 0.087204775200585768, 0.067764185686782441,
      0.048478851154631589, 0.031131992890923658, 0.016743062236152746,
      0.0057071551034947416, -0.0020414880945888616, -0.006870373796940301,
      -0.0093109590426651723, -0.00994995132966052, -0.0093516869813583259,
      -0.0080091953452511751, -0.0063190977802959122, -0.004574654195917455,
      -0.0029725639050458179, -0.0016261340691441876, -0.00058317179649790708,
      0.00015614428542238309, 0.00062256342474462036, 0.00086374623715668208,
      0.00093373132149586079, 0.00088488239116230006, 0.000763338036446789,
      0.00060653746121017366, 0.00044253217918851855, 0.00029028528197111821,
      0.0001611891746932819, 6.011829340398419E-5, -1.227964689510537E-5,
      -5.8508172942612016E-5, -8.28030516713106E-5, -9.0130490907898675E-5,
      -8.5482883723878315E-5, -7.348588001558363E-5, -5.7851096136208456E-5,
      -4.1341373612254262E-5, -2.5996258872648686E-5, -1.3067915721620746E-5,
      -3.1705474436103942E-6, 3.6803763025330787E-6, 7.784135950260659E-6,
      9.4560880168550761E-6, 9.42085836988959E-6, 8.13464324322349E-6,
      6.2755370838892392E-6, 4.3149621163654214E-6, 2.5637368909418781E-6,
      1.0941887439454454E-6, 8.4505593355778357E-8, -4.2290921675671465E-7,
      -5.0442719786233594E-7, -3.6002484767646018E-7, -2.339460600302278E-7,
      -2.2398533893358829E-7, -3.1663535282394345E-7, -4.7756360650289776E-7,
      -7.4771367118137145E-7, -1.1612425660500696E-6, -1.730672963658658E-6,
      -2.2278313281534805E-6, -2.8467409671057921E-6, -3.6119710197280395E-6,
      -4.4893653708143876E-6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.10602875205865551, 0.22266037932317656, 0.31888147181640641,
      0.38944360631144165, 0.43795507377677839, 0.46997264230390062,
      0.4905172487754707, 0.5034310014147434, 0.5114213858602934,
      0.51630439857701838, 0.51925862127063693, 0.52103115488680807,
      0.52208728949977667, 0.41844142923943894, 0.19632541503104486,
      -0.016649327869436441, -0.18373145076192665, -0.30309692621627027,
      -0.38385660704107005, -0.43658419962341077, -0.4701547480144837,
      -0.49113225448850578, -0.50405242072163048, -0.51191890786967609,
      -0.51666369344700591, -0.519503345219785, -0.52119166453269972,
      -0.52218982111564027, -0.51254193162263806, -0.44502847579537297,
      -0.33403720913163831, -0.20728902096737972, -0.088104955315812333,
      0.00916048961439048, 0.078496532044889739, 0.11998002235923591,
      0.13739866187125693, 0.13630137895641783, 0.12260096904013756,
      0.10169546640137105, 0.077997576744575761, 0.054764450737390051,
      0.034128719880703345, 0.017249124400637892, 0.00451615343849922,
      -0.0042283039227610127, -0.0094882181552934739, -0.011944964101620619,
      -0.01232906101089769, -0.01132296230041796, -0.0095160518473509717,
      -0.0073712590115673224, -0.0052252037050431648, -0.0032964723404357919,
      -0.0017045880048151155, -0.0004946278471814194, 0.00034524558305744585,
      0.00085902907851941241, 0.0011082065799937466, 0.001159126695905683,
      0.0010760229229092515, 0.00091240198144036514, 0.00071433038763757818,
      0.00051168099158038142, 0.00032672556635682574, 0.00017170692407642894,
      5.1787542005107717E-5, -3.2847512547612418E-5, -8.4790240276088867E-5,
      -0.00011050068117261422, -0.00011668441335312509, -0.00010845341032408971,
      -9.1372591757180128E-5, -6.9950818954337511E-5, -4.8419712510969833E-5,
      -2.9003805869090612E-5, -1.1816718649393092E-5, 2.4898968973228989E-7,
      9.0904772798691226E-6, 1.3139452298386831E-5, 1.3856595081377989E-5,
      1.2376991059738258E-5, 1.0386205048428742E-5, 7.1360548869402195E-6,
      3.5862140843832357E-6, 5.7613795684570662E-7, -1.0205806605050018E-6,
      -8.9107671866857768E-7, -7.0398572480496065E-8, 6.5481491298654049E-7,
      1.1373794347680095E-6, 1.9093174805682134E-6, 2.92266429264999E-6,
      4.0245165716135359E-6, 3.5137254439321466E-6, 4.3742169521604069E-6,
      5.408353752817788E-6, 6.2010881762134E-6, 7.045992122129823E-6, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.42411500823462206,
      0.46652650905808424, 0.38488436997291947, 0.282248537980141,
      0.19404586986134689, 0.12807027410848892, 0.0821784258862804,
      0.051655010557090625, 0.0319615377821998, 0.01953205086689986,
      0.011816890774474433, 0.0070901344646846359, 0.0042245384518745923,
      -0.414583441041351, -0.88846405683357632, -0.85189897160192518,
      -0.66832849156996088, -0.47746190181737458, -0.32303872329919886,
      -0.21091037032936297, -0.13428219356429155, -0.0839100258960885,
      -0.051680664932499121, -0.031465948592182262, -0.018979142309319474,
      -0.011358607091116366, -0.0067532772516588823, -0.0039926263317620267,
      0.038591557972008514, 0.27005382330906047, 0.44396506665493862,
      0.50699275265703447, 0.47673626260626956, 0.38906177972081124,
      0.2773441697219971, 0.16593396125738463, 0.069674558048084062,
      -0.0043891316593564758, -0.054801639665121034, -0.0836220105550661,
      -0.094791558627181152, -0.092932504028742841, -0.08254292342674685,
      -0.067518381920261814, -0.050931883848554682, -0.03497782944504093,
      -0.021039656930129845, -0.0098269837853085743, -0.0015363876371082793,
      0.0040243948419189214, 0.0072276418122679507, 0.0085791713431345954,
      0.0085842212260966288, 0.0077149254584294915, 0.0063675373424827058,
      0.0048398406305347839, 0.003359493720955461, 0.0020551339818478663,
      0.0009967100058973367, 0.00020368046364774582, -0.00033241509198572559,
      -0.00065448376587554536, -0.00079228637521114786, -0.0008105975842287868,
      -0.000739821700894223, -0.00062007456912158718, -0.0004796775282852849,
      -0.00033854021821088054, -0.00020777091091390583, -0.00010284176358610136,
      -2.4734928722043504E-5, 3.2924012116141497E-5, 6.8323274267638321E-5,
      8.5687091211370442E-5, 8.6124425773470738E-5, 7.7663626567516883E-5,
      6.8748348878790088E-5, 4.8262833356501522E-5, 3.5365950360547329E-5,
      1.6195900074070838E-5, 2.8685711319646341E-6, -5.9184160865589224E-6,
      -7.9631440452380633E-6, -1.3000600645954092E-5, -1.4199363210227937E-5,
      -1.2040304510150116E-5, -6.3868744694028322E-6, 5.1801576734569169E-7,
      3.2827125847523294E-6, 2.9008539418681445E-6, 1.9302580871258764E-6,
      3.0877521832008156E-6, 4.053387248327105E-6, 4.4074091158541849E-6,
      -2.0431645107255578E-6, 3.4419660329130412E-6, 4.1365472026295251E-6,
      3.1709376935824493E-6, 3.3796157836656942E-6, -4.6849198443662844E-6, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0044744583580195287, 0.012578119189197521, 0.023594798807147717,
      0.036908167328246012, 0.051976540149458121, 0.068309031379078067,
      0.085443198553838876, 0.1029234689909784, 0.12027931748092545,
      0.1370030411105449, 0.15252592080076993, 0.16619269497357167,
      0.17723312079965747, 0.18473021976231935, 0.18758397011538791,
      0.18683795964097247, 0.18334751160656917, 0.177810379685388,
      0.17079274873724731, 0.1627511416496272, 0.15405120244424739,
      0.14498309851897093, 0.13577482584799702, 0.12660314453661478,
      0.11760279680058908, 0.10887425777399246, 0.10049014929639674,
      0.092500750558035263, 0.084938373544491935, 0.077821069918298721,
      0.071155734923424391, 0.064940582655382578, 0.059167267706968389,
      0.05382245553473565, 0.048889263854818947, 0.044348312948824659,
      0.040178582966641938, 0.036358216477131826, 0.032865144965194153,
      0.029677229611764611, 0.026772733942715454, 0.024130584681248419,
      0.021730712289278063, 0.019553821978995193, 0.017581652755541594,
      0.015797115939311178, 0.0141841009253515, 0.012727622983050508,
      0.011413773631886502, 0.01022957398405017, 0.0091632052093101428,
      0.0082037152854095678, 0.0073410477775252992, 0.0065659565744986556,
      0.005870061040688929, 0.0052456454623345311, 0.0046857055007265939,
      0.0041839197633417148, 0.0037344652539356995, 0.0033321303434587462,
      0.0029721383352482476, 0.0026501826071033394, 0.0023623663794390144,
      0.0021052199319823322, 0.0018755594314842198, 0.0016705393270643287,
      0.001487534884255948, 0.0013242309863597628, 0.001178560306624255,
      0.0010486413868305284, 0.00093279016971354375, 0.00082951866456392658,
      0.00073747295713635008, 0.00065548451206910252, 0.00058247290652043554,
      0.00051741504753753925, 0.00045946414838321552, 0.00040783539167461651,
      0.00036185691795009189, 0.00032100146061636153, 0.00028465297803615915,
      0.00025234336861907551, 0.00022365599867685116, 0.00019821210498696155,
      0.00017564415897776579, 0.00015563569287188408, 0.00013787071936224011,
      0.00012215879186119416, 0.00010823950238382725, 9.584955576148113E-5,
      8.4869947896063592E-5, 7.5175883502029333E-5, 6.65404519512958E-5,
      5.8865955332849151E-5, 5.2051481056905865E-5, 4.6010345701566442E-5,
      4.0672040788198515E-5, 3.5965401462285553E-5, 3.1819106336536515E-5, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.017897833432078115,
      0.032414643324711966, 0.044066718471800777, 0.053253474084393196,
      0.060273491284848407, 0.0653299649184798, 0.068536668699043277,
      0.069921081748558114, 0.069423393959788166, 0.066894894518477785,
      0.062091518760900112, 0.054667096691207007, 0.044161703304343242,
      0.029988395850647522, 0.011415001412274212, -0.002984041897661768,
      -0.013961792137613166, -0.022148527684724841, -0.02807052379256272,
      -0.032166428350480468, -0.034799756821519347, -0.036272415701105822,
      -0.036833090683895557, -0.036686725245529017, -0.036001390944102804,
      -0.034914156106386485, -0.033536433910382864, -0.0319575949534459,
      -0.030249508054173351, -0.028469214504772831, -0.026661339979497326,
      -0.024860609072167264, -0.023093259793656742, -0.021379248688930978,
      -0.01973276671966679, -0.01816380362397715, -0.016678919928730881,
      -0.015281465958040443, -0.013972286047750724, -0.012751661413718165,
      -0.011617982676196606, -0.010568597045868156, -0.00959948956788141,
      -0.0087075612411314871, -0.0078886768938143913, -0.00713814726492167,
      -0.00645206005583871, -0.0058259117692039735, -0.0052553974046560268,
      -0.0047367985913453307, -0.0042654750989601095, -0.0038379596956022973,
      -0.0034506700315370758, -0.0031003648121065742, -0.0027835821352389053,
      -0.0024976623134175904, -0.0022397598464317511, -0.0020071429495395155,
      -0.0017978180376240595, -0.0016093396419078131, -0.001439968032841995,
      -0.0012878229125796324, -0.0011512649106572995, -0.0010285857898267277,
      -0.00091864200199244972, -0.00082008041767956449, -0.00073201777123352253,
      -0.00065321559158474133, -0.00058268271894203107, -0.00051967567917490559,
      -0.00046340486846793873, -0.00041308602059846907, -0.00036818282971030604,
      -0.00032795378026899007, -0.0002920464221946677, -0.00026023143593158546,
      -0.00023180359661729486, -0.00020651502683439592, -0.00018391389489809851,
      -0.00016342182933492148, -0.00014539393032080952, -0.00012923843766833449,
      -0.00011474947976889746, -0.00010177557475955845, -9.0271784036783065E-5,
      -8.0033864423526842E-5, -7.1059894038575877E-5, -6.2847710004183826E-5,
      -5.56771579094676E-5, -4.95597864893845E-5, -4.3918431461670145E-5,
      -3.8776257576137004E-5, -3.4541726202934152E-5, -3.069798647378661E-5,
      -2.7257897103773141E-5, -2.4164541421357689E-5, -2.1353219653471719E-5,
      -1.8826557303651834E-5, -1.6585180502996164E-5, -1.4686722968639971E-5,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0 } ;

    helicopter_DW.FromWorkspace1_PWORK.TimePtr = (void *) pTimeValues0;
    helicopter_DW.FromWorkspace1_PWORK.DataPtr = (void *) pDataValues0;
    helicopter_DW.FromWorkspace1_IWORK.PrevIndex = 0;
  }

  /* Start for ToFile: '<Root>/To File1' */
  {
    FILE *fp = (NULL);
    char fileName[509] = "DataFiles/voltages.mat";
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_M,
                        "Error creating .mat file DataFiles/voltages.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp, 2 + 1, 0, "V_f_b")) {
      rtmSetErrorStatus(helicopter_M,
                        "Error writing mat file header to file DataFiles/voltages.mat");
      return;
    }

    helicopter_DW.ToFile1_IWORK.Count = 0;
    helicopter_DW.ToFile1_IWORK.Decimation = -1;
    helicopter_DW.ToFile1_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<Root>/To File2' */
  {
    FILE *fp = (NULL);
    char fileName[509] = "DataFiles/controller_output.mat";
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_M,
                        "Error creating .mat file DataFiles/controller_output.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp, 2 + 1, 0, "v_d_s")) {
      rtmSetErrorStatus(helicopter_M,
                        "Error writing mat file header to file DataFiles/controller_output.mat");
      return;
    }

    helicopter_DW.ToFile2_IWORK.Count = 0;
    helicopter_DW.ToFile2_IWORK.Decimation = -1;
    helicopter_DW.ToFile2_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<Root>/To File3' */
  {
    FILE *fp = (NULL);
    char fileName[509] = "DataFiles/u_ts_with_bias.mat";
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_M,
                        "Error creating .mat file DataFiles/u_ts_with_bias.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp, 1 + 1, 0, "ans")) {
      rtmSetErrorStatus(helicopter_M,
                        "Error writing mat file header to file DataFiles/u_ts_with_bias.mat");
      return;
    }

    helicopter_DW.ToFile3_IWORK.Count = 0;
    helicopter_DW.ToFile3_IWORK.Decimation = -1;
    helicopter_DW.ToFile3_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<Root>/To File4' */
  {
    FILE *fp = (NULL);
    char fileName[509] = "DataFiles/x_ts.mat";
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_M,
                        "Error creating .mat file DataFiles/x_ts.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp, 6 + 1, 0, "ans")) {
      rtmSetErrorStatus(helicopter_M,
                        "Error writing mat file header to file DataFiles/x_ts.mat");
      return;
    }

    helicopter_DW.ToFile4_IWORK.Count = 0;
    helicopter_DW.ToFile4_IWORK.Decimation = -1;
    helicopter_DW.ToFile4_PWORK.FilePtr = fp;
  }

  /* Start for If: '<S3>/If' */
  helicopter_DW.If_ActiveSubsystem = -1;

  /* InitializeConditions for TransferFcn: '<S4>/Travel: Transfer Fcn' */
  helicopter_X.TravelTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S4>/Pitch: Transfer Fcn' */
  helicopter_X.PitchTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S4>/Elevation: Transfer Fcn' */
  helicopter_X.ElevationTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S3>/Integrator' */
  helicopter_X.Integrator_CSTATE = helicopter_P.Integrator_IC;

  /* InitializeConditions for Derivative: '<S4>/Derivative' */
  helicopter_DW.TimeStampA = (rtInf);
  helicopter_DW.TimeStampB = (rtInf);
}

/* Model terminate function */
void helicopter_terminate(void)
{
  /* Terminate for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: helicopter/HIL Initialize (hil_initialize_block) */
  {
    t_boolean is_switching;
    t_int result;
    t_uint32 num_final_analog_outputs = 0;
    t_uint32 num_final_pwm_outputs = 0;
    hil_task_stop_all(helicopter_DW.HILInitialize_Card);
    hil_monitor_stop_all(helicopter_DW.HILInitialize_Card);
    is_switching = false;
    if ((helicopter_P.HILInitialize_AOTerminate && !is_switching) ||
        (helicopter_P.HILInitialize_AOExit && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_P.HILInitialize_AOFinal;
        }
      }

      num_final_analog_outputs = 8U;
    } else {
      num_final_analog_outputs = 0;
    }

    if ((helicopter_P.HILInitialize_POTerminate && !is_switching) ||
        (helicopter_P.HILInitialize_POExit && is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_P.HILInitialize_POFinal;
        }
      }

      num_final_pwm_outputs = 8U;
    } else {
      num_final_pwm_outputs = 0;
    }

    if (0
        || num_final_analog_outputs > 0
        || num_final_pwm_outputs > 0
        ) {
      /* Attempt to write the final outputs atomically (due to firmware issue in old Q2-USB). Otherwise write channels individually */
      result = hil_write(helicopter_DW.HILInitialize_Card
                         , helicopter_P.HILInitialize_AOChannels,
                         num_final_analog_outputs
                         , helicopter_P.HILInitialize_POChannels,
                         num_final_pwm_outputs
                         , NULL, 0
                         , NULL, 0
                         , &helicopter_DW.HILInitialize_AOVoltages[0]
                         , &helicopter_DW.HILInitialize_POValues[0]
                         , (t_boolean *) NULL
                         , NULL
                         );
      if (result == -QERR_HIL_WRITE_NOT_SUPPORTED) {
        t_error local_result;
        result = 0;

        /* The hil_write operation is not supported by this card. Write final outputs for each channel type */
        if (num_final_analog_outputs > 0) {
          local_result = hil_write_analog(helicopter_DW.HILInitialize_Card,
            helicopter_P.HILInitialize_AOChannels, num_final_analog_outputs,
            &helicopter_DW.HILInitialize_AOVoltages[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_pwm_outputs > 0) {
          local_result = hil_write_pwm(helicopter_DW.HILInitialize_Card,
            helicopter_P.HILInitialize_POChannels, num_final_pwm_outputs,
            &helicopter_DW.HILInitialize_POValues[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_M, _rt_error_message);
        }
      }
    }

    hil_task_delete_all(helicopter_DW.HILInitialize_Card);
    hil_monitor_delete_all(helicopter_DW.HILInitialize_Card);
    hil_close(helicopter_DW.HILInitialize_Card);
    helicopter_DW.HILInitialize_Card = NULL;
  }

  /* Terminate for ToFile: '<Root>/To File' */
  {
    FILE *fp = (FILE *) helicopter_DW.ToFile_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "DataFiles/loop_variables.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M,
                          "Error closing MAT-file DataFiles/loop_variables.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_M,
                          "Error reopening MAT-file DataFiles/loop_variables.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 6 + 1, helicopter_DW.ToFile_IWORK.Count,
           "y_loop_variables")) {
        rtmSetErrorStatus(helicopter_M,
                          "Error writing header for y_loop_variables to MAT-file DataFiles/loop_variables.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M,
                          "Error closing MAT-file DataFiles/loop_variables.mat");
        return;
      }

      helicopter_DW.ToFile_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<Root>/To File1' */
  {
    FILE *fp = (FILE *) helicopter_DW.ToFile1_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "DataFiles/voltages.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M,
                          "Error closing MAT-file DataFiles/voltages.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_M,
                          "Error reopening MAT-file DataFiles/voltages.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2 + 1, helicopter_DW.ToFile1_IWORK.Count,
           "V_f_b")) {
        rtmSetErrorStatus(helicopter_M,
                          "Error writing header for V_f_b to MAT-file DataFiles/voltages.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M,
                          "Error closing MAT-file DataFiles/voltages.mat");
        return;
      }

      helicopter_DW.ToFile1_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<Root>/To File2' */
  {
    FILE *fp = (FILE *) helicopter_DW.ToFile2_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "DataFiles/controller_output.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M,
                          "Error closing MAT-file DataFiles/controller_output.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_M,
                          "Error reopening MAT-file DataFiles/controller_output.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 2 + 1, helicopter_DW.ToFile2_IWORK.Count,
           "v_d_s")) {
        rtmSetErrorStatus(helicopter_M,
                          "Error writing header for v_d_s to MAT-file DataFiles/controller_output.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M,
                          "Error closing MAT-file DataFiles/controller_output.mat");
        return;
      }

      helicopter_DW.ToFile2_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<Root>/To File3' */
  {
    FILE *fp = (FILE *) helicopter_DW.ToFile3_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "DataFiles/u_ts_with_bias.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M,
                          "Error closing MAT-file DataFiles/u_ts_with_bias.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_M,
                          "Error reopening MAT-file DataFiles/u_ts_with_bias.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 1 + 1, helicopter_DW.ToFile3_IWORK.Count,
           "ans")) {
        rtmSetErrorStatus(helicopter_M,
                          "Error writing header for ans to MAT-file DataFiles/u_ts_with_bias.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M,
                          "Error closing MAT-file DataFiles/u_ts_with_bias.mat");
        return;
      }

      helicopter_DW.ToFile3_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<Root>/To File4' */
  {
    FILE *fp = (FILE *) helicopter_DW.ToFile4_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "DataFiles/x_ts.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M,
                          "Error closing MAT-file DataFiles/x_ts.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_M,
                          "Error reopening MAT-file DataFiles/x_ts.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 6 + 1, helicopter_DW.ToFile4_IWORK.Count,
           "ans")) {
        rtmSetErrorStatus(helicopter_M,
                          "Error writing header for ans to MAT-file DataFiles/x_ts.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M,
                          "Error closing MAT-file DataFiles/x_ts.mat");
        return;
      }

      helicopter_DW.ToFile4_PWORK.FilePtr = (NULL);
    }
  }
}

/*========================================================================*
 * Start of Classic call interface                                        *
 *========================================================================*/

/* Solver interface called by GRT_Main */
#ifndef USE_GENERATED_SOLVER

void rt_ODECreateIntegrationData(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void rt_ODEDestroyIntegrationData(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void rt_ODEUpdateContinuousStates(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

#endif

void MdlOutputs(int_T tid)
{
  helicopter_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  helicopter_update();
  UNUSED_PARAMETER(tid);
}

void MdlInitializeSizes(void)
{
}

void MdlInitializeSampleTimes(void)
{
}

void MdlInitialize(void)
{
}

void MdlStart(void)
{
  helicopter_initialize();
}

void MdlTerminate(void)
{
  helicopter_terminate();
}

/* Registration function */
RT_MODEL_helicopter_T *helicopter(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* non-finite (run-time) assignments */
  helicopter_P.Integrator_UpperSat = rtInf;
  helicopter_P.Integrator_LowerSat = rtMinusInf;

  /* initialize real-time model */
  (void) memset((void *)helicopter_M, 0,
                sizeof(RT_MODEL_helicopter_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&helicopter_M->solverInfo,
                          &helicopter_M->Timing.simTimeStep);
    rtsiSetTPtr(&helicopter_M->solverInfo, &rtmGetTPtr(helicopter_M));
    rtsiSetStepSizePtr(&helicopter_M->solverInfo,
                       &helicopter_M->Timing.stepSize0);
    rtsiSetdXPtr(&helicopter_M->solverInfo, &helicopter_M->derivs);
    rtsiSetContStatesPtr(&helicopter_M->solverInfo, (real_T **)
                         &helicopter_M->contStates);
    rtsiSetNumContStatesPtr(&helicopter_M->solverInfo,
      &helicopter_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&helicopter_M->solverInfo,
      &helicopter_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&helicopter_M->solverInfo,
      &helicopter_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&helicopter_M->solverInfo,
      &helicopter_M->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&helicopter_M->solverInfo, (&rtmGetErrorStatus
      (helicopter_M)));
    rtsiSetRTModelPtr(&helicopter_M->solverInfo, helicopter_M);
  }

  rtsiSetSimTimeStep(&helicopter_M->solverInfo, MAJOR_TIME_STEP);
  helicopter_M->intgData.f[0] = helicopter_M->odeF[0];
  helicopter_M->contStates = ((real_T *) &helicopter_X);
  rtsiSetSolverData(&helicopter_M->solverInfo, (void *)&helicopter_M->intgData);
  rtsiSetSolverName(&helicopter_M->solverInfo,"ode1");

  /* Initialize timing info */
  {
    int_T *mdlTsMap = helicopter_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;
    helicopter_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    helicopter_M->Timing.sampleTimes = (&helicopter_M->Timing.sampleTimesArray[0]);
    helicopter_M->Timing.offsetTimes = (&helicopter_M->Timing.offsetTimesArray[0]);

    /* task periods */
    helicopter_M->Timing.sampleTimes[0] = (0.0);
    helicopter_M->Timing.sampleTimes[1] = (0.002);

    /* task offsets */
    helicopter_M->Timing.offsetTimes[0] = (0.0);
    helicopter_M->Timing.offsetTimes[1] = (0.0);
  }

  rtmSetTPtr(helicopter_M, &helicopter_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = helicopter_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    helicopter_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(helicopter_M, -1);
  helicopter_M->Timing.stepSize0 = 0.002;
  helicopter_M->Timing.stepSize1 = 0.002;

  /* External mode info */
  helicopter_M->Sizes.checksums[0] = (191856560U);
  helicopter_M->Sizes.checksums[1] = (3492909835U);
  helicopter_M->Sizes.checksums[2] = (1147499574U);
  helicopter_M->Sizes.checksums[3] = (1712994765U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[2];
    helicopter_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    systemRan[1] = (sysRanDType *)&helicopter_DW.IfActionSubsystem_SubsysRanBC;
    rteiSetModelMappingInfoPtr(helicopter_M->extModeInfo,
      &helicopter_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(helicopter_M->extModeInfo, helicopter_M->Sizes.checksums);
    rteiSetTPtr(helicopter_M->extModeInfo, rtmGetTPtr(helicopter_M));
  }

  helicopter_M->solverInfoPtr = (&helicopter_M->solverInfo);
  helicopter_M->Timing.stepSize = (0.002);
  rtsiSetFixedStepSize(&helicopter_M->solverInfo, 0.002);
  rtsiSetSolverMode(&helicopter_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  helicopter_M->blockIO = ((void *) &helicopter_B);

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      helicopter_B.Gain1[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      helicopter_B.FromWorkspace1[i] = 0.0;
    }

    helicopter_B.TravelCounttorad = 0.0;
    helicopter_B.Gain = 0.0;
    helicopter_B.Sum3 = 0.0;
    helicopter_B.Gain_d = 0.0;
    helicopter_B.PitchCounttorad = 0.0;
    helicopter_B.Gain_i = 0.0;
    helicopter_B.Gain_b = 0.0;
    helicopter_B.ElevationCounttorad = 0.0;
    helicopter_B.Gain_e = 0.0;
    helicopter_B.Sum = 0.0;
    helicopter_B.Gain_dg = 0.0;
    helicopter_B.Minus[0] = 0.0;
    helicopter_B.Minus[1] = 0.0;
    helicopter_B.Add = 0.0;
    helicopter_B.Sum3_h = 0.0;
    helicopter_B.Sum1 = 0.0;
    helicopter_B.Frontgain = 0.0;
    helicopter_B.Backgain = 0.0;
    helicopter_B.Gain_l = 0.0;
    helicopter_B.BackmotorSaturation = 0.0;
    helicopter_B.FrontmotorSaturation = 0.0;
    helicopter_B.In1 = 0.0;
  }

  /* parameters */
  helicopter_M->defaultParam = ((real_T *)&helicopter_P);

  /* states (continuous) */
  {
    real_T *x = (real_T *) &helicopter_X;
    helicopter_M->contStates = (x);
    (void) memset((void *)&helicopter_X, 0,
                  sizeof(X_helicopter_T));
  }

  /* states (dwork) */
  helicopter_M->dwork = ((void *) &helicopter_DW);
  (void) memset((void *)&helicopter_DW, 0,
                sizeof(DW_helicopter_T));

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AIMinimums[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AIMaximums[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AOMinimums[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AOMaximums[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AOVoltages[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_FilterFrequency[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_POSortedFreqs[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_POValues[i] = 0.0;
    }
  }

  helicopter_DW.TimeStampA = 0.0;
  helicopter_DW.LastUAtTimeA = 0.0;
  helicopter_DW.TimeStampB = 0.0;
  helicopter_DW.LastUAtTimeB = 0.0;
  helicopter_DW.HILWriteAnalog_Buffer[0] = 0.0;
  helicopter_DW.HILWriteAnalog_Buffer[1] = 0.0;

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    helicopter_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 16;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.BTransTable = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.PTransTable = &rtPTransTable;
  }

  /* Initialize Sizes */
  helicopter_M->Sizes.numContStates = (4);/* Number of continuous states */
  helicopter_M->Sizes.numPeriodicContStates = (0);
                                      /* Number of periodic continuous states */
  helicopter_M->Sizes.numY = (0);      /* Number of model outputs */
  helicopter_M->Sizes.numU = (0);      /* Number of model inputs */
  helicopter_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  helicopter_M->Sizes.numSampTimes = (2);/* Number of sample times */
  helicopter_M->Sizes.numBlocks = (76);/* Number of blocks */
  helicopter_M->Sizes.numBlockIO = (23);/* Number of block outputs */
  helicopter_M->Sizes.numBlockPrms = (157);/* Sum of parameter "widths" */
  return helicopter_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
