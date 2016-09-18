/**************************************************************************/
/*!
@file     getting_started.ino
@author   Moritz Walter
@license  GPLv3 (see license.txt)

Demo of SPI configuration tool for Trinamic TMC2130 Motor Drivers

@section  HISTORY

v0.1 - it works
*/
/**************************************************************************/

    ////////////////////
   //
  //    HOW TO USE
 //
////////////////////

// This tool uses hardware SPI
// Just wire the drivers to your SPI pins and define the CS pin below.
// Once flashed, the sketch sends a basic configuration to the driver and makes the motor spin.

#include <SPI.h>
#include <Trinamic_TMC2130.h>

// pin configuration (this is the X-axis on a standard RAMPS 1.4)
#define CS_PIN 53
#define EN_PIN 38 //enable (CFG6)
#define DIR_PIN 55 //direction
#define STEP_PIN 54 //step

Trinamic_TMC2130 myStepper(CS_PIN);

void setup(){
  // pins
  pinMode(EN_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  digitalWrite(EN_PIN, HIGH); // disable driver
  digitalWrite(DIR_PIN, LOW); // chose direction
  digitalWrite(STEP_PIN, LOW); // no step yet

  // stepper
  myStepper.init();
  myStepper.set_mres(64); // ({1,2,4,8,16,32,64,128,256}) number of microsteps
  myStepper.set_IHOLD_IRUN(31,31,5); // ([0-31],[0-31],[0-5]) sets all currents to maximum
  myStepper.set_I_scale_analog(1); // ({0,1}) 0: I_REF internal, 1: sets I_REF to AIN
  myStepper.set_tbl(1); // ([0-3]) set comparator blank time to 16, 24, 36 or 54 clocks, 1 or 2 is recommended
  myStepper.set_toff(8); // ([0-15]) 0: driver disable, 1: use only with TBL>2, 2-15: off time setting during slow decay phase

  // get ready
  digitalWrite(EN_PIN, LOW); // enable driver
}

void loop(){
  // make a step
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(100);
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(100);
}
