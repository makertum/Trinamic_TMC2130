/**************************************************************************/
/*!
@file     Trinamic_TMC2130.cpp
@author   Moritz Walter
@license  GPLv3 (see license.txt)

SPI configuration tool for Trinamic TMC2130 Motor Drivers

@section  HISTORY

v0.1 - it works
*/
/**************************************************************************/
#ifndef TRINAMIC_TMC2130_H
#define TRINAMIC_TMC2130_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//#include <SPI.h>

// RW
#define TMC_READ                         (0x00)
#define TMC_WRITE                        (0x80)

// SPISTATUS MASKS
#define TMC_SPISTATUS_RESET_MASK         (0x01)
#define TMC_SPISTATUS_ERROR_MASK         (0x02)
#define TMC_SPISTATUS_STALLGUARD_MASK    (0x04)
#define TMC_SPISTATUS_STANDSTILL_MASK    (0x08)

// REGISTERS
#define TMC_REG_GCONF                    (0x00) // RW //    17 // Global configuration flags
#define TMC_REG_GSTAT                    (0x01) // RC //     3 // Global status flags
#define TMC_REG_IOIN                     (0x04) // R  //   8+8 // Reads the state of all input pins available
#define TMC_REG_IHOLD_IRUN               (0x10) // W  // 5+5+4 // Driver current control
#define TMC_REG_TPOWERDOWN               (0x11) // W  //     8 // sets delay time after stand still (stst) to motor current power down (0-4 seconds) 0_((2^8)-1) * 2^18 tclk
#define TMC_REG_TSTEP                    (0x12) // R  //    20 // Actual measured time between two 1/256 microsteps derived from the step input frequency in units of 1/fCLK. Measured value is (2^20)-1 in case of overflow or stand still
#define TMC_REG_TPWMTHRS                 (0x13) // W  //    20 // Upper velocity threshold for stealthChop voltage PWM mode
#define TMC_REG_TCOOLTHRS                (0x14) // W  //    20 // Lower threshold velocity for switching on smart energy coolStep and stallGuard feature (unsigned)
#define TMC_REG_THIGH                    (0x15) // W  //    20 // Velocity dependend switching into different chopper mode and fullstepping to maximize torque (unsigned)
#define TMC_REG_XDIRECT                  (0x2D) // RW //    32 // specifies motor coil currents and polarity directly programmed via SPI. Use signed, two's complement numbers. In this mode, the current is scaled by IHOLD. Velocity based current regulation of voltage PWM is not available in this mode. +- 255 for both coils
#define TMC_REG_VDCMIN                   (0x33) // W  //    23 // automatic commutation dcStep becomes enabled by the external signal DCEN. VDCMIN is used as the minimum step velocity when the motor is heavily loaded. Hint: Also set DCCTRL parameters in order to operate dcStep
#define TMC_REG_MSLUT0                   (0x60) // W  //    32 // Each bit gives the difference between entry x and entry x+1 when combined with the corresponding MSLUTSEL W bits. Differential coding for the first quarter of a wave. Start values for CUR_A and CUR_B are stored for MSCNT position 0 in START_SIN and START_SIN90.
#define TMC_REG_MSLUT1                   (0x61) // W  //    32 //
#define TMC_REG_MSLUT2                   (0x62) // W  //    32 //
#define TMC_REG_MSLUT3                   (0x63) // W  //    32 //
#define TMC_REG_MSLUT4                   (0x64) // W  //    32 //
#define TMC_REG_MSLUT5                   (0x65) // W  //    32 //
#define TMC_REG_MSLUT6                   (0x66) // W  //    32 //
#define TMC_REG_MSLUT7                   (0x67) // W  //    32 //
#define TMC_REG_MSLUTSEL                 (0x68) // W  //    32 // defines four segments within each quarter MSLUT wave. Four 2 bit entries determine the meaning of a 0 and a 1 bit in the corresponding segment of MSLUT
#define TMC_REG_MSLUTSTART               (0x69) // W  //   8+8 //
#define TMC_REG_MSCNT                    (0x6A) // R  //    10 //
#define TMC_REG_MSCURACT                 (0x6B) // R  //   9+9 //
#define TMC_REG_CHOPCONF                 (0x6C) // RW //    32 //
#define TMC_REG_COOLCONF                 (0x6D) // W  //    25 //
#define TMC_REG_DCCTRL                   (0x6E) // W  //    24 //
#define TMC_REG_DRV_STATUS               (0x6F) // R  //    22 //
#define TMC_REG_PWMCONF                  (0x70) // W  //     8 //
#define TMC_REG_PWM_SCALE                (0x71) // R  //     8 //
#define TMC_REG_ENCM_CTRL                (0x72) // W  //     2 //
#define TMC_REG_LOST_STEPS               (0x73) // R  //    20 //

// GCONF BIT OFFSETS
#define TMC_GCONF_I_SCALE                      (0) // 0: Internal, 1: AIN
#define TMC_GCONF_R_SENSE                      (1) // 0: Normal, 1: Internal
#define TMC_GCONF_STEALTH_CHOP_PWM_MODE        (2) // 0: Disable, 1: Enable
#define TMC_GCONF_ENCODER                      (3) // 0: Disable, 1: Enable
#define TMC_GCONF_INVERT_MOTOR_DIRECTION       (4) // 0: Normal, 1: Invert
#define TMC_GCONF_DIAG0_ERROR                  (5) // 0: Disable, 1: Enable
#define TMC_GCONF_DIAG0_TEMPERATURE            (6) // 0: Disable, 1: Enable
#define TMC_GCONF_DIAG0_STALL                  (7) // 0: Disable, 1: Enable
#define TMC_GCONF_DIAG1_STALL                  (8) // 0: Disable, 1: Enable
#define TMC_GCONF_DIAG1_INDEX                  (9) // 0: Disable, 1: Enable
#define TMC_GCONF_DIAG1_ONSTATE                (10) // 0: Disable, 1: Enable
#define TMC_GCONF_DIAG1_STEPS_SKIPPED          (11) // 0: Disable, 1: Enable
#define TMC_GCONF_DIAG0_PINMODE                (12) // 0: Open Collector, 1: Push Pull
#define TMC_GCONF_DIAG1_PINMODE                (13) // 0: Open Collector, 1: Push Pull
#define TMC_GCONF_HYSTERESIS                   (14) // 0: 1/16, 1: 1/32
#define TMC_GCONF_STOP_ENABLE                  (15) // 0: Normal, 1: Emergency Stop
#define TMC_GCONF_STOP_DIRECT_MODE             (16) // 0: Normal, 1: XDIRECT
#define TMC_GCONF_TEST_MODE                    (17) // 0: Normal, 1: Enable, Don't use!

// IHOLD_IRUN BIT OFFSETS
#define TMC_IHOLD                        (0)
#define TMC_IRUN                         (8)
#define TMC_IHOLDDELAY                   (16)

// IHOLD IRUN MASKS
#define TMC_IHOLD_MASK                   (0b11111UL)
#define TMC_IRUN_MASK                    (0b11111UL)
#define TMC_IHOLDDELAY_MASK              (0b1111UL)

// CHOPCONF MASKS
// mask the bits from the values we want to set

const uint32_t TMC_CHOPCONF_MASKS[] = {
  0b1111UL,  // 0 TOFF_TIME
  0b111UL,  // 1
  0b11UL,   // 2
  0b1UL,    // 3
  0b111UL,  // 4 HYSTERESIS_START
  0b11UL,   // 5
  0b1UL,    // 6
  0b0001UL, // 7 HYSTERESIS_LOW
  0b001UL,  // 8
  0b01UL,   // 9
  0b1UL,    // 10
  0b1UL,    // 11 FAST_DECAY_TIMING
  0b1UL,    // 12 FAST_DECAY_MODE
  0b1UL,    // 13 RANDOM_TOFF
  0b1UL,    // 14 CHOPPER_MODE
  0b11UL,   // 15 BLANK_TIME
  0b1UL,    // 16
  0b1UL,    // 17 SENSE_CURRENT_SCALING
  0b1UL,    // 18 HIGH_VELOCITY_STEPS
  0b1UL,    // 19 HIGH_VELOCITY_CHOPPER
  0b1111UL, // 20 SYNC_PWM
  0b111UL,  // 21
  0b11UL,   // 22
  0b1UL,    // 23
  0b1111UL, // 24 MRES
  0b111UL,  // 25
  0b11UL,   // 26
  0b1UL,    // 27
  0b1UL,    // 28 INTERPOLATE
  0b1UL,    // 29 DOUBLE_EDGE_PULSES
  0b1UL,    // 30 SHORT_PROTECTION
  0b1UL    // 31
};

// CHOPCONF BIT OFFSETS
// for shifting incoming values to the right register position
#define TMC_CHOPCONF_SHORT_PROTECTION             (30)
#define TMC_CHOPCONF_DOUBLE_EDGE_PULSES           (29)
#define TMC_CHOPCONF_INTERPOLATE                  (28)
#define TMC_CHOPCONF_MRES                         (24)
#define TMC_CHOPCONF_SYNC_PWM                     (20)
#define TMC_CHOPCONF_HIGH_VELOCITY_CHOPPER        (19)
#define TMC_CHOPCONF_HIGH_VELOCITY_STEPS          (18)
#define TMC_CHOPCONF_SENSE_CURRENT_SCALING        (17)
#define TMC_CHOPCONF_BLANK_TIME                   (15)
#define TMC_CHOPCONF_CHOPPER_MODE                 (14)
#define TMC_CHOPCONF_RANDOM_TOFF                  (13)
#define TMC_CHOPCONF_FAST_DECAY_MODE              (12)
#define TMC_CHOPCONF_FAST_DECAY_TIMING            (11)
#define TMC_CHOPCONF_HYSTERESIS_LOW               (7)
#define TMC_CHOPCONF_HYSTERESIS_START             (4)
#define TMC_CHOPCONF_TOFF_TIME                    (0)

// TIMING SETTINGS; NOT YET IMPLEMENTED

// TPOWERDOWN
//#define TPOWERDOWN_MASK              (0b11111111UL)
// TPWMTHRS
//#define TPWMTHRS_MASK                (0b11111111111111111111UL)
// TCOOLTHRS
//#define TCOOLTHRS_MASK               (0b11111111111111111111UL)
// THIGH
//#define THIGH_MASK                   (0b11111111111111111111UL)

// SPI MODE REGISTER; CANNOT BE USED IN STEP/DIR MODE
// XDIRECT
// #define XDIRECT_MASK                 (0xFFFFFFFFUL)

// VDCMIN
// #define VDCMIN_MASK                  (0b11111111111111111111111UL)


class Trinamic_TMC2130{
public:
  Trinamic_TMC2130(uint8_t csPin);
  void init();
  uint8_t readStatus();
  uint8_t readRegister( uint8_t address , uint32_t *data );
  uint8_t writeRegister( uint8_t address, uint32_t data );
  uint8_t alterRegister(uint8_t address, uint32_t data, uint32_t mask);
  uint8_t setGCONF(uint8_t bit, uint8_t value);
  uint8_t setCHOPCONF(uint8_t position, uint8_t value);
  uint8_t setCurrent(uint8_t ihold, uint8_t irun, uint8_t iholddelay);
  uint8_t setMicrosteps(uint8_t value);
  uint8_t setIScale(uint8_t value);
  boolean isReset();
  boolean isError();
  boolean isStallguard();
  boolean isStandstill();

private:
  uint8_t _csPin;
  uint8_t _status;
};

#endif // TRINAMIC_TMC2130_H
