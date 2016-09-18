/**************************************************************************/
/*!
@file     configuration_tool.ino
@author   Moritz Walter
@license  GPLv3 (see license.txt)

Tool to send SPI configuration to Trinamic TMC2130 Motor Drivers

@section  HISTORY

v0.1 - it works
*/
/**************************************************************************/

#include <SPI.h>
#include <Trinamic_TMC2130.h>

#define BAUDRATE 125000

    ////////////////////
   //
  //    HOW TO USE
 //
////////////////////

// This tool uses hardware SPI
// Just wire the drivers to your SPI pins and define the CS pins below.
// Then, uncomment configuration below to enable specific settings.
// Once flashed, the sketch sends your configuration to the drivers in regular intervals.

    ////////////////////
   //
  //    CONFIGURATION
 //
////////////////////

//////////
// STEPPER PIN CONFIGURATION
// enter your pin configuration here
//////////

// enable one or two stepper drivers
#define STEPPERX_ENABLE
//#define STEPPERY_ENABLE

// cable select pins for the steppers
#define X_CS_PIN 53
#define Y_CS_PIN 49

//////////
// TMC2130 CONFIGURATION
// uncomment below settings to send them to the drivers
//////////

//#define GLOBAL_EN_PWM_MODE 0
#define GLOBAL_I_SCALE_ANALOG 1 // [0,1] 0: Normal, 1: AIN
//#define GLOBAL_INTERNAL_RSENSE 0 // [0,1]0: Normal, 1: Internal
#define GLOBAL_EN_PWM_MODE 0 //[0,1] 0: Normal, 1: stealthChop with velocity threshold
//#define GLOBAL_ENC_COMMUTATION 0 //
#define GLOBAL_SHAFT 0 // 0: normal, 1: invert
//#define GLOBAL_DIAG0_ERROR 0 //
//#define GLOBAL_DIAG0_OTPW 0 //
//#define GLOBAL_DIAG0_STALL 0 //
//#define GLOBAL_DIAG1_STALL 0 //
//#define GLOBAL_DIAG1_INDEX 0 //
//#define GLOBAL_DIAG1_ONSTATE 0 //
//#define GLOBAL_DIAG1_ONSTATE 0 //
//#define GLOBAL_DIAG0_INT_PUSHPULL 0 //
//#define GLOBAL_DIAG1_INT_PUSHPULL 0 //
//#define GLOBAL_SMALL_HYSTERESIS 0 //
//#define GLOBAL_STOP_ENABLE 0 //
//#define GLOBAL_DIRECT_MODE 0 //

/* VELOCITY DEPENDEND DRIVE FEATURES */

#define GLOBAL_IHOLD 22 // [0-31] 0: min, 31: max
#define GLOBAL_IRUN 31 // [0-31] 0: min, 31: max
#define GLOBAL_IHOLDDELAY 15   // [0-15]  0: min, 15: about 4 seconds // delay before IHOLD is applied
//#define GLOBAL_TPOWERDOWN 0 // [0-255] 0: min, 255: about 4 seconds // delay to standstill current reduction
//#define GLOBAL_TPWMTHRS 0 // [0-1048576] e.g. 20 corresponds with 2000 steps/s
//#define GLOBAL_TCOOLTHRS 0 // [0-1048576] e.g. 20 corresponds with 2000 steps/s
#define GLOBAL_THIGH 10 // [0-1048576] e.g. 20 corresponds with 2000 steps/s

/* SPI MODE CONFIGURATION */

//#define GLOBAL_XDIRECT 0 //

/* DCSTEP MINIMUM VELOCITY */

//#define GLOBAL_VDCMIN 0 //

/* MOTOR DRIVER CONFIGURATION*/

//#define GLOBAL_DEDGE 0 //
//#define GLOBAL_DISS2G 0 //
#define GLOBAL_INTPOL 1 // 0: off 1: 256 microstep interpolation
#define GLOBAL_MRES 16 // number of microsteps
#define GLOBAL_SYNC 1 // {0-15}
#define GLOBAL_VHIGHCHM 1 // [0,1] 0: normal, 1: high velocity stepper mode
#define GLOBAL_VHIGHFS 0 // [0,1] 0: normal, 1: switch to full steps for high velocities
// #define GLOBAL_VSENSE 0 // [0,1] 0: normal, 1: high sensitivity (not recommended)
#define GLOBAL_TBL 1    // 0-3: set comparator blank time to 16, 24, 36 or 54 clocks, 1 or 2 is recommended
#define GLOBAL_CHM 0 // [0,1] 0: spreadCycle, 1: Constant off time with fast decay time.
//#define GLOBAL_RNDTF 0 //
//#define GLOBAL_DISFDCC 0 //
//#define GLOBAL_FD 0 //
//#define GLOBAL_HEND 0 //
//#define GLOBAL_HSTRT 0 //
#define GLOBAL_TOFF 10    // 0: driver disable, 1: use only with TBL>=2, 2-15: off time setting during slow decay phase

//#define GLOBAL_SFILT 0 //
//#define GLOBAL_SGT 0 //
//#define GLOBAL_SEIMIN 0 //
//#define GLOBAL_SEDN 0 //
//#define GLOBAL_SEMAX 0 //
//#define GLOBAL_SEUP 0 //
//#define GLOBAL_SEMIN 0 //

//#define GLOBAL_DC_TIME 0 //
//#define GLOBAL_DC_SG 0 //

//#define GLOBAL_FREEWHEEL 0 //
//#define GLOBAL_PWM_SYMMETRIC 0 //
//#define GLOBAL_PWM_AUTOSCALE 0 //
//#define GLOBAL_PWM_FREQ 0 //
//#define GLOBAL_PWM_GRAD 0 //
//#define GLOBAL_PWM_AMPL 0 //

//#define GLOBAL_ENCM_CTRL 0 //




    ////////////////////
   //
  //    NO CHANGES REQUIRED BELOW
 //
////////////////////

#ifdef STEPPERX_ENABLE
 Trinamic_TMC2130 stepperX(X_CS_PIN);
#endif
#ifdef STEPPERY_ENABLE
  Trinamic_TMC2130 stepperY(Y_CS_PIN);
#endif

void setup(){
  Serial.begin(BAUDRATE);
}


void loop(){
  configure();
  #ifdef STEPPERX_ENABLE
    uint32_t xbuf;
    stepperX.read_REG(TMC_REG_GCONF,&xbuf);
    Serial.print("X GCONF: ");
    Serial.println(xbuf,BIN);
  #endif
  #ifdef STEPPERY_ENABLE
    uint32_t ybuf;
    stepperY.read_REG(TMC_REG_GCONF,&ybuf);
    Serial.print("Y GCONF: ");
    Serial.println(ybuf,BIN);
  #endif
  delay(1000);
}

void configure(){
  #ifdef STEPPERX_ENABLE
    stepperX.init();
    #ifdef GLOBAL_I_SCALE_ANALOG
      stepperX.set_I_scale_analog(GLOBAL_I_SCALE_ANALOG);
    #endif
    #ifdef GLOBAL_INTERNAL_RSENSE
    stepperX.set_internal_Rsense(GLOBAL_INTERNAL_RSENSE);
    #endif
    #ifdef GLOBAL_EN_PWM_MODE
    stepperX.set_en_pwm_mode(GLOBAL_EN_PWM_MODE);
    #endif
    #ifdef GLOBAL_ENC_COMMUTATION
    stepperX.set_enc_commutation(GLOBAL_ENC_COMMUTATION);
    #endif
    #ifdef GLOBAL_SHAFT
    stepperX.set_shaft(GLOBAL_SHAFT);
    #endif
    #ifdef GLOBAL_DIAG0_ERROR
    stepperX.set_diag0_error(GLOBAL_DIAG0_ERROR);
    #endif
    #ifdef GLOBAL_DIAG0_OTPW
    stepperX.set_diag0_otpw(GLOBAL_DIAG0_OTPW);
    #endif
    #ifdef GLOBAL_DIAG0_STALL
    stepperX.set_diag0_stall(GLOBAL_DIAG0_STALL);
    #endif
    #ifdef GLOBAL_DIAG1_STALL
    stepperX.set_diag1_stall(GLOBAL_DIAG1_STALL);
    #endif
    #ifdef GLOBAL_DIAG1_INDEX
    stepperX.set_diag1_index(GLOBAL_DIAG1_INDEX);
    #endif
    #ifdef GLOBAL_DIAG1_ONSTATE
    stepperX.set_diag1_onstate(GLOBAL_DIAG1_ONSTATE);
    #endif
    #ifdef GLOBAL_DIAG1_ONSTATE
    stepperX.set_diag1_steps_skipped(GLOBAL_DIAG1_ONSTATE);
    #endif
    #ifdef GLOBAL_DIAG0_INT_PUSHPULL
    stepperX.set_diag0_int_pushpull(GLOBAL_DIAG0_INT_PUSHPULL);
    #endif
    #ifdef GLOBAL_DIAG1_INT_PUSHPULL
    stepperX.set_diag1_int_pushpull(GLOBAL_DIAG1_INT_PUSHPULL);
    #endif
    #ifdef GLOBAL_SMALL_HYSTERESIS
    stepperX.set_small_hysteresis(GLOBAL_SMALL_HYSTERESIS);
    #endif
    #ifdef GLOBAL_STOP_ENABLE
    stepperX.set_stop_enable(GLOBAL_STOP_ENABLE);
    #endif
    #ifdef GLOBAL_DIRECT_MODE
    stepperX.set_direct_mode(GLOBAL_DIRECT_MODE);
    #endif

    #ifdef GLOBAL_IHOLD
    stepperX.set_IHOLD_IRUN(GLOBAL_IHOLD,GLOBAL_IRUN,GLOBAL_IHOLDDELAY);
    #endif
    #ifdef GLOBAL_TPOWERDOWN
    stepperX.set_TPOWERDOWN(GLOBAL_TPOWERDOWN);
    #endif
    #ifdef GLOBAL_TPWMTHRS
    stepperX.set_TPWMTHRS(GLOBAL_TPWMTHRS);
    #endif
    #ifdef GLOBAL_TCOOLTHRS
    stepperX.set_TCOOLTHRS(GLOBAL_TCOOLTHRS);
    #endif
    #ifdef GLOBAL_THIGH
    stepperX.set_THIGH(GLOBAL_THIGH);
    #endif
    #ifdef GLOBAL_XDIRECT
    stepperX.set_XDIRECT(GLOBAL_XDIRECT);
    #endif
    #ifdef GLOBAL_VDCMIN
    stepperX.set_VDCMIN(GLOBAL_VDCMIN);
    #endif

    #ifdef GLOBAL_DEDGE
    stepperX.set_dedge(GLOBAL_DEDGE);
    #endif
    #ifdef GLOBAL_DISS2G
    stepperX.set_diss2g(GLOBAL_DISS2G);
    #endif
    #ifdef GLOBAL_INTPOL
    stepperX.set_intpol(GLOBAL_INTPOL);
    #endif
    #ifdef GLOBAL_MRES
    stepperX.set_mres(GLOBAL_MRES);
    #endif
    #ifdef GLOBAL_SYNC
    stepperX.set_sync(GLOBAL_SYNC);
    #endif
    #ifdef GLOBAL_VHIGHCHM
    stepperX.set_vhighchm(GLOBAL_VHIGHCHM);
    #endif
    #ifdef GLOBAL_VHIGHFS
    stepperX.set_vhighfs(GLOBAL_VHIGHFS);
    #endif
    #ifdef GLOBAL_VSENSE
    stepperX.set_vsense(GLOBAL_VSENSE);
    #endif
    #ifdef GLOBAL_TBL
    stepperX.set_tbl(GLOBAL_TBL);
    #endif
    #ifdef GLOBAL_CHM
    stepperX.set_chm(GLOBAL_CHM);
    #endif
    #ifdef GLOBAL_RNDTF
    stepperX.set_rndtf(GLOBAL_RNDTF);
    #endif
    #ifdef GLOBAL_DISFDCC
    stepperX.set_disfdcc(GLOBAL_DISFDCC);
    #endif
    #ifdef GLOBAL_FD
    stepperX.set_fd(GLOBAL_FD);
    #endif
    #ifdef GLOBAL_HEND
    stepperX.set_hend(GLOBAL_HEND);
    #endif
    #ifdef GLOBAL_HSTRT
    stepperX.set_hstrt(GLOBAL_HSTRT);
    #endif
    #ifdef GLOBAL_TOFF
    stepperX.set_toff(GLOBAL_TOFF);
    #endif

    #ifdef GLOBAL_SFILT
    stepperX.set_sfilt(GLOBAL_SFILT);
    #endif
    #ifdef GLOBAL_SGT
    stepperX.set_sgt(GLOBAL_SGT);
    #endif
    #ifdef GLOBAL_SEIMIN
    stepperX.set_seimin(GLOBAL_SEIMIN);
    #endif
    #ifdef GLOBAL_SEDN
    stepperX.set_sedn(GLOBAL_SEDN);
    #endif
    #ifdef GLOBAL_SEMAX
    stepperX.set_semax(GLOBAL_SEMAX);
    #endif
    #ifdef GLOBAL_SEUP
    stepperX.set_seup(GLOBAL_SEUP);
    #endif
    #ifdef GLOBAL_SEMIN
    stepperX.set_semin(GLOBAL_SEMIN);
    #endif

    #ifdef GLOBAL_DC_TIME
    stepperX.set_DCCTRL(GLOBAL_DC_TIME, GLOBAL_DC_SG);
    #endif

    #ifdef GLOBAL_FREEWHEEL
    stepperX.set_freewheel(GLOBAL_FREEWHEEL);
    #endif
    #ifdef GLOBAL_PWM_SYMMETRIC
    stepperX.set_pwm_symmetric(GLOBAL_PWM_SYMMETRIC);
    #endif
    #ifdef GLOBAL_PWM_AUTOSCALE
    stepperX.set_pwm_autoscale(GLOBAL_PWM_AUTOSCALE);
    #endif
    #ifdef GLOBAL_PWM_FREQ
    stepperX.set_pwm_freq(GLOBAL_PWM_FREQ);
    #endif
    #ifdef GLOBAL_PWM_GRAD
    stepperX.set_PWM_GRAD(GLOBAL_PWM_GRAD);
    #endif
    #ifdef GLOBAL_PWM_AMPL
    stepperX.set_PWM_AMPL(GLOBAL_PWM_AMPL);
    #endif

    #ifdef GLOBAL_ENCM_CTRL
    stepperX.set_ENCM_CTRL(GLOBAL_ENCM_CTRL);
    #endif
  #endif

  #ifdef STEPPERY_ENABLE
    stepperY.init();
    #ifdef GLOBAL_I_SCALE_ANALOG
      stepperY.set_I_scale_analog(GLOBAL_I_SCALE_ANALOG);
    #endif
    #ifdef GLOBAL_INTERNAL_RSENSE
    stepperY.set_internal_Rsense(GLOBAL_INTERNAL_RSENSE);
    #endif
    #ifdef GLOBAL_EN_PWM_MODE
    stepperY.set_en_pwm_mode(GLOBAL_EN_PWM_MODE);
    #endif
    #ifdef GLOBAL_ENC_COMMUTATION
    stepperY.set_enc_commutation(GLOBAL_ENC_COMMUTATION);
    #endif
    #ifdef GLOBAL_SHAFT
    stepperY.set_shaft(GLOBAL_SHAFT);
    #endif
    #ifdef GLOBAL_DIAG0_ERROR
    stepperY.set_diag0_error(GLOBAL_DIAG0_ERROR);
    #endif
    #ifdef GLOBAL_DIAG0_OTPW
    stepperY.set_diag0_otpw(GLOBAL_DIAG0_OTPW);
    #endif
    #ifdef GLOBAL_DIAG0_STALL
    stepperY.set_diag0_stall(GLOBAL_DIAG0_STALL);
    #endif
    #ifdef GLOBAL_DIAG1_STALL
    stepperY.set_diag1_stall(GLOBAL_DIAG1_STALL);
    #endif
    #ifdef GLOBAL_DIAG1_INDEX
    stepperY.set_diag1_index(GLOBAL_DIAG1_INDEX);
    #endif
    #ifdef GLOBAL_DIAG1_ONSTATE
    stepperY.set_diag1_onstate(GLOBAL_DIAG1_ONSTATE);
    #endif
    #ifdef GLOBAL_DIAG1_ONSTATE
    stepperY.set_diag1_steps_skipped(GLOBAL_DIAG1_ONSTATE);
    #endif
    #ifdef GLOBAL_DIAG0_INT_PUSHPULL
    stepperY.set_diag0_int_pushpull(GLOBAL_DIAG0_INT_PUSHPULL);
    #endif
    #ifdef GLOBAL_DIAG1_INT_PUSHPULL
    stepperY.set_diag1_int_pushpull(GLOBAL_DIAG1_INT_PUSHPULL);
    #endif
    #ifdef GLOBAL_SMALL_HYSTERESIS
    stepperY.set_small_hysteresis(GLOBAL_SMALL_HYSTERESIS);
    #endif
    #ifdef GLOBAL_STOP_ENABLE
    stepperY.set_stop_enable(GLOBAL_STOP_ENABLE);
    #endif
    #ifdef GLOBAL_DIRECT_MODE
    stepperY.set_direct_mode(GLOBAL_DIRECT_MODE);
    #endif

    #ifdef GLOBAL_IHOLD
    stepperY.set_IHOLD_IRUN(GLOBAL_IHOLD,GLOBAL_IRUN,GLOBAL_IHOLDDELAY);
    #endif
    #ifdef GLOBAL_TPOWERDOWN
    stepperY.set_TPOWERDOWN(GLOBAL_TPOWERDOWN);
    #endif
    #ifdef GLOBAL_TPWMTHRS
    stepperY.set_TPWMTHRS(GLOBAL_TPWMTHRS);
    #endif
    #ifdef GLOBAL_TCOOLTHRS
    stepperY.set_TCOOLTHRS(GLOBAL_TCOOLTHRS);
    #endif
    #ifdef GLOBAL_THIGH
    stepperY.set_THIGH(GLOBAL_THIGH);
    #endif
    #ifdef GLOBAL_XDIRECT
    stepperY.set_XDIRECT(GLOBAL_XDIRECT);
    #endif
    #ifdef GLOBAL_VDCMIN
    stepperY.set_VDCMIN(GLOBAL_VDCMIN);
    #endif

    #ifdef GLOBAL_DEDGE
    stepperY.set_dedge(GLOBAL_DEDGE);
    #endif
    #ifdef GLOBAL_DISS2G
    stepperY.set_diss2g(GLOBAL_DISS2G);
    #endif
    #ifdef GLOBAL_INTPOL
    stepperY.set_intpol(GLOBAL_INTPOL);
    #endif
    #ifdef GLOBAL_MRES
    stepperY.set_mres(GLOBAL_MRES);
    #endif
    #ifdef GLOBAL_SYNC
    stepperY.set_sync(GLOBAL_SYNC);
    #endif
    #ifdef GLOBAL_VHIGHCHM
    stepperY.set_vhighchm(GLOBAL_VHIGHCHM);
    #endif
    #ifdef GLOBAL_VHIGHFS
    stepperY.set_vhighfs(GLOBAL_VHIGHFS);
    #endif
    #ifdef GLOBAL_VSENSE
    stepperY.set_vsense(GLOBAL_VSENSE);
    #endif
    #ifdef GLOBAL_TBL
    stepperY.set_tbl(GLOBAL_TBL);
    #endif
    #ifdef GLOBAL_CHM
    stepperY.set_chm(GLOBAL_CHM);
    #endif
    #ifdef GLOBAL_RNDTF
    stepperY.set_rndtf(GLOBAL_RNDTF);
    #endif
    #ifdef GLOBAL_DISFDCC
    stepperY.set_disfdcc(GLOBAL_DISFDCC);
    #endif
    #ifdef GLOBAL_FD
    stepperY.set_fd(GLOBAL_FD);
    #endif
    #ifdef GLOBAL_HEND
    stepperY.set_hend(GLOBAL_HEND);
    #endif
    #ifdef GLOBAL_HSTRT
    stepperY.set_hstrt(GLOBAL_HSTRT);
    #endif
    #ifdef GLOBAL_TOFF
    stepperY.set_toff(GLOBAL_TOFF);
    #endif

    #ifdef GLOBAL_SFILT
    stepperY.set_sfilt(GLOBAL_SFILT);
    #endif
    #ifdef GLOBAL_SGT
    stepperY.set_sgt(GLOBAL_SGT);
    #endif
    #ifdef GLOBAL_SEIMIN
    stepperY.set_seimin(GLOBAL_SEIMIN);
    #endif
    #ifdef GLOBAL_SEDN
    stepperY.set_sedn(GLOBAL_SEDN);
    #endif
    #ifdef GLOBAL_SEMAX
    stepperY.set_semax(GLOBAL_SEMAX);
    #endif
    #ifdef GLOBAL_SEUP
    stepperY.set_seup(GLOBAL_SEUP);
    #endif
    #ifdef GLOBAL_SEMIN
    stepperY.set_semin(GLOBAL_SEMIN);
    #endif

    #ifdef GLOBAL_DC_TIME
    stepperY.set_DCCTRL(GLOBAL_DC_TIME, GLOBAL_DC_SG);
    #endif

    #ifdef GLOBAL_FREEWHEEL
    stepperY.set_freewheel(GLOBAL_FREEWHEEL);
    #endif
    #ifdef GLOBAL_PWM_SYMMETRIC
    stepperY.set_pwm_symmetric(GLOBAL_PWM_SYMMETRIC);
    #endif
    #ifdef GLOBAL_PWM_AUTOSCALE
    stepperY.set_pwm_autoscale(GLOBAL_PWM_AUTOSCALE);
    #endif
    #ifdef GLOBAL_PWM_FREQ
    stepperY.set_pwm_freq(GLOBAL_PWM_FREQ);
    #endif
    #ifdef GLOBAL_PWM_GRAD
    stepperY.set_PWM_GRAD(GLOBAL_PWM_GRAD);
    #endif
    #ifdef GLOBAL_PWM_AMPL
    stepperY.set_PWM_AMPL(GLOBAL_PWM_AMPL);
    #endif

    #ifdef GLOBAL_ENCM_CTRL
    stepperY.set_ENCM_CTRL(GLOBAL_ENCM_CTRL);
    #endif
  #endif
}
