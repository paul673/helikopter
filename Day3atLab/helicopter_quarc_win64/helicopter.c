/*
 * helicopter.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "helicopter".
 *
 * Model version              : 6.0
 * Simulink Coder version : 9.4 (R2020b) 29-Jul-2020
 * C source code generated on : Tue Feb 27 11:53:15 2024
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
  real_T rtb_Integrator;
  real_T rtb_HILReadEncoderTimebase_o1;
  real_T rtb_HILReadEncoderTimebase_o2;
  real_T rtb_HILReadEncoderTimebase_o3;
  real_T rtb_TmpSignalConversionAtToFile[2];
  real_T rtb_Derivative;
  real_T rtb_Sum;
  real_T rtb_Sum1;
  real_T *lastU;
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

    /* Gain: '<S4>/Pitch: Count to rad' */
    helicopter_B.PitchCounttorad = helicopter_P.PitchCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o2;

    /* Gain: '<S9>/Gain' */
    helicopter_B.Gain_i = helicopter_P.Gain_Gain_a *
      helicopter_B.PitchCounttorad;
  }

  /* Integrator: '<S3>/Integrator' incorporates:
   *  TransferFcn: '<S4>/Travel: Transfer Fcn'
   */
  rtb_Integrator = 0.0;
  rtb_Integrator += helicopter_P.TravelTransferFcn_C *
    helicopter_X.TravelTransferFcn_CSTATE;
  rtb_Integrator += helicopter_P.TravelTransferFcn_D *
    helicopter_B.TravelCounttorad;

  /* Gain: '<S13>/Gain' */
  helicopter_B.Gain_d = helicopter_P.Gain_Gain_l * rtb_Integrator;

  /* Integrator: '<S3>/Integrator' incorporates:
   *  TransferFcn: '<S4>/Pitch: Transfer Fcn'
   */
  rtb_Integrator = 0.0;
  rtb_Integrator += helicopter_P.PitchTransferFcn_C *
    helicopter_X.PitchTransferFcn_CSTATE;
  rtb_Integrator += helicopter_P.PitchTransferFcn_D *
    helicopter_B.PitchCounttorad;

  /* Gain: '<S10>/Gain' */
  helicopter_B.Gain_b = helicopter_P.Gain_Gain_ae * rtb_Integrator;
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

  /* Integrator: '<S3>/Integrator' incorporates:
   *  TransferFcn: '<S4>/Elevation: Transfer Fcn'
   */
  rtb_Integrator = 0.0;
  rtb_Integrator += helicopter_P.ElevationTransferFcn_C *
    helicopter_X.ElevationTransferFcn_CSTATE;
  rtb_Integrator += helicopter_P.ElevationTransferFcn_D *
    helicopter_B.ElevationCounttorad;

  /* Gain: '<S8>/Gain' */
  helicopter_B.Gain_dg = helicopter_P.Gain_Gain_n * rtb_Integrator;

  /* Gain: '<S2>/Gain1' */
  helicopter_B.Gain1[0] = helicopter_P.Gain1_Gain * helicopter_B.Gain;
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
          rtb_Integrator = pDataValues[currTimeIndex];
        } else {
          rtb_Integrator = pDataValues[currTimeIndex + 1];
        }
      } else {
        real_T f1 = (t2 - t) / (t2 - t1);
        real_T f2 = 1.0 - f1;
        real_T d1;
        real_T d2;
        int_T TimeIndex= currTimeIndex;
        d1 = pDataValues[TimeIndex];
        d2 = pDataValues[TimeIndex + 1];
        rtb_Integrator = (real_T) rtInterpolate(d1, d2, f1, f2);
        pDataValues += 141;
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
            for (elIdx = 0; elIdx < 4; ++elIdx) {
              (&helicopter_B.FromWorkspace1[0])[elIdx] =
                pDataValues[currTimeIndex];
              pDataValues += 141;
            }
          }
        } else {
          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 4; ++elIdx) {
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
          for (elIdx = 0; elIdx < 4; ++elIdx) {
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

  /* Sum: '<Root>/Add' incorporates:
   *  Constant: '<Root>/pitch_ref'
   *  Gain: '<Root>/Gain'
   *  Sum: '<Root>/Minus'
   *  Sum: '<Root>/Minus2'
   */
  helicopter_B.Add = (rtb_Integrator - ((((helicopter_B.Gain1[0] -
    helicopter_B.FromWorkspace1[0]) * helicopter_P.K[0] + (helicopter_B.Gain1[1]
    - helicopter_B.FromWorkspace1[1]) * helicopter_P.K[1]) +
    (helicopter_B.Gain1[2] - helicopter_B.FromWorkspace1[2]) * helicopter_P.K[2])
    + (helicopter_B.Gain1[3] - helicopter_B.FromWorkspace1[3]) * helicopter_P.K
    [3])) + helicopter_P.pitch_ref_Value;

  /* Sum: '<S5>/Sum3' incorporates:
   *  Gain: '<S5>/K_pd'
   *  Gain: '<S5>/K_pp'
   *  Sum: '<S5>/Sum2'
   */
  helicopter_B.Sum3 = (helicopter_B.Add - helicopter_B.Gain1[2]) *
    helicopter_P.K_pp - helicopter_P.K_pd * helicopter_B.Gain1[3];

  /* Sum: '<Root>/Sum1' incorporates:
   *  Constant: '<Root>/Vd_bias'
   */
  rtb_Sum1 = helicopter_B.Sum3 + helicopter_P.Vd_ff;

  /* Integrator: '<S3>/Integrator' */
  /* Limited  Integrator  */
  if (helicopter_X.Integrator_CSTATE >= helicopter_P.Integrator_UpperSat) {
    helicopter_X.Integrator_CSTATE = helicopter_P.Integrator_UpperSat;
  } else {
    if (helicopter_X.Integrator_CSTATE <= helicopter_P.Integrator_LowerSat) {
      helicopter_X.Integrator_CSTATE = helicopter_P.Integrator_LowerSat;
    }
  }

  /* Integrator: '<S3>/Integrator' */
  rtb_Integrator = helicopter_X.Integrator_CSTATE;

  /* Sum: '<S3>/Sum' incorporates:
   *  Constant: '<Root>/elevation_ref'
   */
  rtb_Sum = helicopter_P.elevation_ref_Value - helicopter_B.Gain1[4];

  /* Sum: '<S3>/Sum1' incorporates:
   *  Gain: '<S3>/K_ed'
   *  Gain: '<S3>/K_ep'
   */
  helicopter_B.Sum1 = (helicopter_P.K_ep * rtb_Sum + rtb_Integrator) -
    helicopter_P.K_ed * helicopter_B.Gain1[5];

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
    rtb_TmpSignalConversionAtToFile[0] = helicopter_B.Sum3;
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
          (helicopter_DW.ToFile4_IWORK.Count * (4 + 1)) + 1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_DW.ToFile4_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[4 + 1];
          helicopter_DW.ToFile4_IWORK.Decimation = 0;
          u[0] = helicopter_M->Timing.t[1];
          u[1] = helicopter_B.FromWorkspace1[0];
          u[2] = helicopter_B.FromWorkspace1[1];
          u[3] = helicopter_B.FromWorkspace1[2];
          u[4] = helicopter_B.FromWorkspace1[3];
          if (fwrite(u, sizeof(real_T), 4 + 1, fp) != 4 + 1) {
            rtmSetErrorStatus(helicopter_M,
                              "Error writing to MAT-file DataFiles/x_ts.mat");
            return;
          }

          if (((++helicopter_DW.ToFile4_IWORK.Count) * (4 + 1))+1 >= 100000000)
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
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.52359877559828927,
      0.52359877559828771, 0.52359877559828549, 0.52359877559828327,
      0.52359877559827972, 0.52359877559827528, 0.52359877559826928,
      0.52359877559826062, 0.52359877559824752, 0.52359877559822954,
      0.52359877559816181, 0.52359877559762691, 0.52359877559764423,
      0.0086840244985586512, -0.52359877559751156, -0.52359877559792034,
      -0.52359877559801171, -0.52359877559804435, -0.52359877559805268,
      -0.52359877559804446, -0.52359877559802159, -0.52359877559796864,
      -0.52359877559785539, -0.52359877559763213, -0.52359877559690493,
      -0.52359877559657042, -0.52359877555376955, -0.5235987755605489,
      -0.47307630402384093, -0.19354597381621746, 0.002217996064190153,
      0.12605626202399578, 0.19193445751226135, 0.21420857678809146,
      0.20629374389778532, 0.17978711814445714, 0.14401542129181777,
      0.10593114551580773, 0.070262865020159659, 0.039825795865342628,
      0.015911104823629674, -0.0013096278797929406, -0.012409256112789979,
      -0.018386086852103634, -0.020413359369669926, -0.019658509200709551,
      -0.017166135562638463, -0.013794311288597982, -0.010192099147420808,
      -0.0068061941855279695, -0.0039058654280305083, -0.0016172829367279151,
      3.9533087760168506E-5, 0.0011158157974178184, 0.0017040458386062829,
      0.0019141884106602669, 0.0018564193877376178, 0.001629840515312897,
      0.0013162481386098657, 0.00097783116252947444, 0.00065766529475796975,
      0.00038198028473135359, 0.0001633525098612143, 4.17604353286638E-6,
      -0.00010003791553736541, -0.00015780928242936643, -0.00017941942043764314,
      -0.00017525198743417736, -0.00015470069635747929, -0.00012555787230073,
      -9.3778727526583339E-5, -6.351460519937735E-5, -3.7318216248749536E-5,
      -1.6440134907313997E-5, -1.1546240257231943E-6, 8.9285746395217913E-6,
      1.4593332274270843E-5, 1.6800451452247955E-5, 1.6528392789760815E-5,
      1.4667640295340512E-5, 1.1960786207465546E-5, 8.9785153492005776E-6,
      6.1214282438548651E-6, 3.6385087057722032E-6, 1.6545425720604356E-6,
      2.0055718319422766E-7, -7.5689293099756583E-7, -1.2901942505472164E-6,
      -1.4901980518455815E-6, -1.4511124335481185E-6, -1.260385512624751E-6,
      -9.9277577136902551E-7, -7.0759256132202353E-7, -4.4804221488448093E-7,
      -2.4169700452159049E-7, -1.0130744676484227E-7, -2.5550311200106535E-8,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } ;

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
      3.1415926535897931, 3.1378421413625768, 3.1262155534627625,
      3.103309300047894, 3.0666274152144117, 3.0144539224191527,
      2.9456562771441384, 2.8595077633243942, 2.7555515880053818,
      2.6335051105396707, 2.4931956060890483, 2.3345185761273548,
      2.1574113215405029, 1.96183648410571, 1.7514602452473496,
      1.5341394208623884, 1.317407488555022, 1.1071745868831813,
      0.90766299128576566, 0.72172938782800156, 0.55123889286403149,
      0.39737898685106426, 0.26089170058611405, 0.14223405511662712,
      0.041684309277369672, -0.040589701080501479, -0.10448752982567838,
      -0.14994945655428654, -0.17694017376590387, -0.18580079862056217,
      -0.17891950249312077, -0.16022233361375074, -0.13419257059990625,
      -0.10504607260616031, -0.076223641339396936, -0.050178167725060938,
      -0.028377123611972938, -0.01143649762564922, 0.0006828287700690398,
      0.00846571265987079, 0.012651682706088437, 0.014078850922726191,
      0.013568784364557185, 0.011851183997383614, 0.0095230085937741873,
      0.0070346995146793644, 0.00469575527356884, 0.0026925129742936356,
      0.0011121679195260606, -3.1534499791496878E-5, -0.00077410400300195075,
      -0.0011795341741293612, -0.0013238692691218488, -0.001283267601886404,
      -0.0011262092903151816, -0.00090919803691593608, -0.000675177919654153,
      -0.00045388125767218871, -0.00026340004508870963, -0.00011239520217466901,
      -2.4968488885708539E-6, 6.94151435349544E-5, 0.00010924074616760461,
      0.00012409210821313625, 0.00012114802572982867, 0.00010690022870386139,
      8.6731847762348311E-5, 6.4755370627443381E-5, 4.3836285668438989E-5,
      2.5735394662510002E-5, 1.1314020725278018E-5, 7.5934429656800985E-7,
      -6.2000667989043248E-6, -1.0107053480247671E-5, -1.1626181193221468E-5,
      -1.1433751910783898E-5, -1.0144892211873902E-5, -8.2722363659002612E-6,
      -6.2094157648383638E-6, -4.2324043307741912E-6, -2.51237140342526E-6,
      -1.1347320826829877E-6, -1.2030239389714184E-7, 5.5432327686716086E-7,
      9.3897298292909124E-7, 1.0955884363426593E-6, 1.0876473599453274E-6,
      9.7309604895925942E-7, 8.0029342802534288E-7, 6.06338177340887E-7,
      4.1713030202601795E-7, 2.4857128944674278E-7, 1.0840114874101746E-7,
      -1.7190632309294776E-9, -8.4162683028693663E-8, -1.4352089114727942E-7,
      -1.8513165529882585E-7, -2.140205524991135E-7, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.01500204893642601,
      -0.046506351597030128, -0.091625013646656983, -0.14672753933610141,
      -0.20869397117638169, -0.275190581109079, -0.34459405527521969,
      -0.415824701274718, -0.48818590986559146, -0.56123801778452431,
      -0.63470811985826081, -0.70842901833653527, -0.78229934974141158,
      -0.841504955431532, -0.86928329754158418, -0.86692772922984307,
      -0.84093160668819078, -0.79804638239535342, -0.74373441383505334,
      -0.6819619798493205, -0.61543962405404717, -0.54594914505657111,
      -0.474630581885821, -0.40219898334654591, -0.32909604141677673,
      -0.25559131500011129, -0.18184770692439339, -0.10796286885489861,
      -0.035442499402444032, 0.027525184506860961, 0.074788675508506652,
      0.10411905205189879, 0.11658599197545222, 0.11528972506928502,
      0.10418189446143505, 0.087204176453070312, 0.067762503945041158,
      0.048477305582626889, 0.031131535559371412, 0.016743880184305476,
      0.0057086728669557786, -0.002040266232991932, -0.0068704014688945351,
      -0.0093127016140631558, -0.0099532363168967474, -0.0093557769643020355,
      -0.0080129691970954632, -0.0063213802190948262, -0.0045748096772931287,
      -0.0029702780129528352, -0.0016217206845277, -0.00057734037993546566,
      0.00016240666893674915, 0.00062823324619725081, 0.00086804501364449321,
      0.00093608046900596964, 0.00088518664792491461, 0.00076192485034455235,
      0.000604019371653286, 0.00043959341314354764, 0.00028764796968979177,
      0.00015930241053946378, 5.9405448188539E-5, -1.1776329924747946E-5,
      -5.6991188116689134E-5, -8.0673523774264552E-5, -8.7905908541320362E-5,
      -8.36763398381475E-5, -7.2403564021761537E-5, -5.7685495749479128E-5,
      -4.22187057151031E-5, -2.7837644381848531E-5, -1.5627946725654192E-5,
      -6.0765108521054913E-6, 7.697171300100636E-7, 5.1554387955251325E-6,
      7.4906233834596989E-6, 8.2512824044415432E-6, 7.9080457361812365E-6,
      6.8801317093736323E-6, 5.510557282910203E-6, 4.0577187551554031E-6,
      2.6985026830727103E-6, 1.5385988242234373E-6, 6.2646181371538086E-7,
      -3.1764305607285639E-8, -4.5820524393620606E-7, -6.912104837444515E-7,
      -7.75821002705959E-7, -7.5683150124390327E-7, -6.7423605033813624E-7,
      -5.6068056282826745E-7, -4.4048084788880849E-7, -3.2977447919168251E-7,
      -2.3743283247473132E-7, -1.6644305660768041E-7, -1.1555558880173611E-7,
      -8.1053781425853258E-8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.10602875204606033, 0.22266037931560179, 0.31888147180085236,
      0.38944360629515917, 0.43795507376531229, 0.4699726422958268,
      0.49051724877100911, 0.503431001412265, 0.51142138585888286,
      0.516304398576258, 0.51925862127024736, 0.52103115488641916,
      0.522087289499341, 0.41844260905716724, 0.19632671283178427,
      -0.016648257185276516, -0.18373066559101559, -0.3030963864078271,
      -0.3838562507684683, -0.43658397101305008, -0.4701546043168684,
      -0.49113216557530109, -0.50405236638530571, -0.51191887499588207,
      -0.51666367372255861, -0.51950333346686461, -0.52119165756193153,
      -0.52218981699514311, -0.51254627384713125, -0.44503154128876582,
      -0.33404030337575608, -0.20729589941367144, -0.0881115699390449,
      0.0091615194390997079, 0.078505904431026674, 0.11999202666724496,
      0.13740631605510711, 0.13630041656023983, 0.12259327777668394,
      0.10168645320695568, 0.0779926307270471, 0.054766542059200174,
      0.034137551107215947, 0.017261244657551966, 0.0045270546410112189,
      -0.0042226145171136986, -0.0094904524490746489, -0.011955504838545905,
      -0.0123440935339032, -0.011340216996119867, -0.0095310881509383938,
      -0.0073812811193642025, -0.0052282496152775693, -0.0032922843392722,
      -0.0016948979822414278, -0.0004808486146821167, 0.00035969808951452631,
      0.0008711673087546945, 0.0011160156156174716, 0.0011620998766933566,
      0.0010738923628246244, 0.00090709739406802381, 0.00070603357695808722,
      0.00050308562176593075, 0.00031956134901778288, 0.00016737770364427806,
      5.1115733290529874E-5, -2.9892976188272691E-5, -7.9671674140358029E-5,
      -0.0001040216853984699, -0.0001093133648348843, -0.00010163984903732715,
      -8.629347986999214E-5, -6.7505900840814093E-5, -4.83865246296844E-5,
      -3.0996605723920823E-5, -1.6504192810296026E-5, -5.3760474405528669E-6,
      2.4258656802222944E-6, 7.2649037590188925E-6, 9.6796289735801011E-6,
      1.0268107841993057E-5, 9.6064200810763722E-6, 8.1977574798441921E-6,
      6.4466360241155485E-6, 4.6520908184577436E-6, 3.0139216844471406E-6,
      1.6467920445073503E-6, 5.9799483398403908E-7, -1.3421054378959241E-7,
      -5.837531017460762E-7, -8.02566816227035E-7, -8.4952567813711966E-7,
      -7.8243033263536432E-7, -6.5263549164917833E-7, -5.0172862320341238E-7,
      -3.5965318723185646E-7, -2.4384559749179857E-7, -1.5943506803672847E-7,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.42411500823609,
      0.46652650905681942, 0.38488436998130227, 0.28224853798457733,
      0.19404586987852507, 0.12807027412283581, 0.082178425900338734,
      0.05165501056382682, 0.031961537786847877, 0.019532050869783116,
      0.011816890775762628, 0.0070901344648337779, 0.0042245384516978457,
      -0.41457872176919081, -0.88846358490601984, -0.8518998800609654,
      -0.66832963363416908, -0.47746288327526887, -0.3230394574325669,
      -0.21091088098519192, -0.13428253321041017, -0.0839102450338389,
      -0.051680803239834004, -0.031466034442610161, -0.018979194907767487,
      -0.011358638977280268, -0.0067532963803303772, -0.00399263773153859,
      0.038574172591949743, 0.27005893023427352, 0.44396495165077221,
      0.50697761584987378, 0.47673731790373575, 0.38909235751557081,
      0.27737753995902564, 0.16594448894546207, 0.06965715755211159,
      -0.0044235979772020689, -0.054828555136071044, -0.0836272982770191,
      -0.094775289919433792, -0.09290435467228228, -0.082515963807307649,
      -0.0675052257985892, -0.050936760066245659, -0.0349986766317783,
      -0.021071351728059344, -0.0098602095577233536, -0.0015543547814295517,
      0.0040155061507915236, 0.0072365153808381143, 0.0085992281262065474,
      0.0086121260159645868, 0.007743861104187429, 0.0063895454280907607,
      0.0048561974702787739, 0.0033621868167190445, 0.0020458768769669438,
      0.00097939322746508329, 0.00018433704430530711, -0.00035283005546938109,
      -0.00066717987503710457, -0.00080425526840733046, -0.00081179182077168366,
      -0.00073409709099937456, -0.00060873458149656725, -0.00046504788140728717,
      -0.00032403483791353875, -0.0001991147918053042, -9.740004503138282E-5,
      -2.1166717746092561E-5, 3.0694063186283714E-5, 6.1385476673189333E-5,
      7.51503161130431E-5, 7.64775048448932E-5, 6.9559675623889316E-5,
      5.796965165444138E-5, 4.4512581477952556E-5, 3.1207652483640991E-5,
      1.9356152314411713E-5, 9.6589008584263315E-6, 2.3539154740432589E-6,
      -2.6467510436444267E-6, -5.6346504047122485E-6, -7.0044858228438033E-6,
      -7.1781808226500387E-6, -6.5526765359040739E-6, -5.4685185595327451E-6,
      -4.195188842295933E-6, -2.9288215114482016E-6, -1.798170231849755E-6,
      -8.75254857485803E-7, -1.8783544798213411E-7, 2.6838138227589844E-7,
      5.1917936384305024E-7, 6.03627473843113E-7, 5.6830174352346244E-7,
      4.6323035907738565E-7, 3.3764211767828461E-7, 2.312791457361853E-7, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0 } ;

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

    if (rt_WriteMat4FileHeader(fp, 4 + 1, 0, "ans")) {
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

      if (rt_WriteMat4FileHeader(fp, 4 + 1, helicopter_DW.ToFile4_IWORK.Count,
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
  helicopter_M->Sizes.checksums[0] = (928694755U);
  helicopter_M->Sizes.checksums[1] = (1918594728U);
  helicopter_M->Sizes.checksums[2] = (1947028336U);
  helicopter_M->Sizes.checksums[3] = (1780592844U);

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

    helicopter_B.TravelCounttorad = 0.0;
    helicopter_B.Gain = 0.0;
    helicopter_B.Gain_d = 0.0;
    helicopter_B.PitchCounttorad = 0.0;
    helicopter_B.Gain_i = 0.0;
    helicopter_B.Gain_b = 0.0;
    helicopter_B.ElevationCounttorad = 0.0;
    helicopter_B.Gain_e = 0.0;
    helicopter_B.Sum = 0.0;
    helicopter_B.Gain_dg = 0.0;
    helicopter_B.FromWorkspace1[0] = 0.0;
    helicopter_B.FromWorkspace1[1] = 0.0;
    helicopter_B.FromWorkspace1[2] = 0.0;
    helicopter_B.FromWorkspace1[3] = 0.0;
    helicopter_B.Add = 0.0;
    helicopter_B.Sum3 = 0.0;
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
  helicopter_M->Sizes.numBlocks = (71);/* Number of blocks */
  helicopter_M->Sizes.numBlockIO = (21);/* Number of block outputs */
  helicopter_M->Sizes.numBlockPrms = (149);/* Sum of parameter "widths" */
  return helicopter_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
