# Trinamic TMC2130 Arduino library
This library makes it easy to configure the Trinamic TMC2130 stepper motor driver found on Watterott's "ludicrous" version of the [SilentStepStick motor driver](http://www.watterott.com/de/SilentStepStick-TMC2130), which has it's repository [here](https://github.com/watterott/SilentStepStick). It's also used in my [Marlin Fork](https://github.com/makertum/Marlin), which gets you up and running if you actually want to use these beasts in a 3D-printer-like machine.

## Features
- setting microstep resolution via `setMicrosteps(n)`
- setting holding current, running current and holding current delay via `setCurrent(ihold,irun,iholddelay)`
- setting current reference via `setIscale(1) // 0: internal, 1: AIN`
- update status via `readStatus()`, 
- checking status via `isReset()`, `isError()`, `isStallguard()`, `isStandstill()`

## More functions
- write to registers via `writeRegister(address,data)`
- read registers via `readRegister(address,buf)`
- alter register bits via `alterRegister(address,data,mask)`
- set/unset bits in GCONF via `setGCONF(position,value)`
- set/unset bits or values in CHOPCONF via `setCHOPCONF(position,value)`
- set/unset bits or values in other registers will be added soon
- if you can't wait, it's easy to add the missing bits yourself. Here's the [datasheet](http://www.trinamic.com/_articles/products/integrated-circuits/tmc2130/_datasheet/TMC2130_datasheet.pdf).

## How to use
    #include <SPI.h>
    #include <Trinamic_TMC2130.h>
    
    #define CS_PIN 53
    
    Trinamic_TMC2130 myStepper(CS_PIN);
    
    void setup(){
      myStepper.init();
      myStepper.setCurrents(31,31,5); // sets all currents to maximum
      myStepper.setIRef(1); // sets I_REF to AIN
    }
    
    void loop(){
      // do something
    }

## FAQ

### Why?
- why not?

### Why no Arduino examples
- yet to come

### Can I run my RepRap with this?
- yes, just install the library, download the [Marlin Fork](https://github.com/makertum/Marlin) and get going

## Troubleshooting

### Why doesn't it work
- If you're using the SilentStepsStick driver, did you `tmc.setIRef(1)`?

## Disclaimer / License
This is a work in progress. Any suggestions are heavily welcome. All scripts in this repository are licensed under the GNU Affero General Public License, version 3. Created by Moritz Walter 2016.
