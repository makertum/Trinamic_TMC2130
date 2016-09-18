# Trinamic TMC2130 Arduino library
This library makes it easy to configure the Trinamic TMC2130 stepper motor driver found on Watterott's "ludicrous" version of the [SilentStepStick motor driver](http://www.watterott.com/de/SilentStepStick-TMC2130), which has it's repository [here](https://github.com/watterott/SilentStepStick). It's also used in my [Marlin Fork](https://github.com/makertum/Marlin), which gets you up and running if you actually want to use these beasts in a 3D-printer-like machine.

## Features
- [x] Basic status readings
- [x] All methods follow the naming convention in the [datasheet](http://www.trinamic.com/_articles/products/integrated-circuits/tmc2130/_datasheet/TMC2130_datasheet.pdf).
- [x] All configurable parameters implemented

## To do
- [ ] Full status report
- [ ] Some readable parameters are missing

## How to use
- Please check out the example sketches for further information.
- The [datasheet](http://www.trinamic.com/_articles/products/integrated-circuits/tmc2130/_datasheet/TMC2130_datasheet.pdf) contains a quick start guide

## Status methods (return boolean status information)

     boolean isReset();
     boolean isError();
     boolean isStallguard();
     boolean isStandstill();

## Configuration methods (write configuration parameters to the driver)
All methods follow the naming convention set_<name of parameter as stated in the datasheet> respectively get_<name of parameter> and return the SPI status flags or the requested parameter.
All methods expect the encoding stated in the datasheet - with the following exceptions:
- `set_mres()` sets the microstepping resolution and expects the integer values 1, 2, 4, 8, 16, 32, 64, 128, 256 e.g. `set_mres(128)`
- `set_IHOLD_IRUN()` sets the holding current, running current and holding delay, e.g. `set_IHOLD_IRUN(31,31,15)`
- `set_MSLUTSTART()` sets `start_sin` and `start_sin90`
- `set_DCCTRL()` sets the `dc_time` and `dc_sg` 

     uint8_t set_I_scale_analog(uint8_t value);
     uint8_t set_internal_Rsense(uint8_t value);
     uint8_t set_en_pwm_mode(uint8_t value);
     uint8_t set_enc_commutation(uint8_t value);
     uint8_t set_shaft(uint8_t value);
     uint8_t set_diag0_error(uint8_t value);
     uint8_t set_diag0_otpw(uint8_t value);
     uint8_t set_diag0_stall(uint8_t value);
     uint8_t set_diag1_stall(uint8_t value);
     uint8_t set_diag1_index(uint8_t value);
     uint8_t set_diag1_onstate(uint8_t value);
     uint8_t set_diag1_steps_skipped(uint8_t value);
     uint8_t set_diag0_int_pushpull(uint8_t value);
     uint8_t set_diag1_int_pushpull(uint8_t value);
     uint8_t set_small_hysteresis(uint8_t value);
     uint8_t set_stop_enable(uint8_t value);
     uint8_t set_direct_mode(uint8_t value);

     uint8_t set_IHOLD_IRUN(uint8_t ihold, uint8_t irun, uint8_t iholddelay);
     uint8_t set_TPOWERDOWN(uint8_t value);
     uint32_t get_TSTEP();
     uint8_t set_TPWMTHRS(uint32_t value);
     uint8_t set_TCOOLTHRS(uint32_t value);
     uint8_t set_THIGH(uint32_t value);
     uint8_t set_XDIRECT(int32_t value);
     int32_t get_XDIRECT();
     uint8_t set_VDCMIN(int32_t value);

     uint8_t set_MSLUT0(uint32_t value);
     uint8_t set_MSLUT1(uint32_t value);
     uint8_t set_MSLUT2(uint32_t value);
     uint8_t set_MSLUT3(uint32_t value);
     uint8_t set_MSLUT4(uint32_t value);
     uint8_t set_MSLUT5(uint32_t value);
     uint8_t set_MSLUT6(uint32_t value);
     uint8_t set_MSLUT7(uint32_t value);
     uint8_t set_MSLUTSEL(uint32_t value);
     uint8_t set_MSLUTSTART(uint8_t start_sin, uint8_t start_sin90);
     uint16_t get_MSCNT();
     int32_t get_MSCURACT();

     uint8_t set_dedge(uint8_t value);
     uint8_t set_diss2g(uint8_t value);
     uint8_t set_intpol(uint8_t value);
     uint8_t set_mres(uint8_t value);
     uint8_t set_sync(uint8_t value);
     uint8_t set_vhighchm(uint8_t value);
     uint8_t set_vhighfs(uint8_t value);
     uint8_t set_vsense(uint8_t value);
     uint8_t set_tbl(uint8_t value);
     uint8_t set_chm(uint8_t value);
     uint8_t set_rndtf(uint8_t value);
     uint8_t set_disfdcc(uint8_t value);
     uint8_t set_fd(uint8_t value);
     uint8_t set_hend(uint8_t value);
     uint8_t set_hstrt(uint8_t value);
     uint8_t set_toff(uint8_t value);
     uint8_t set_sfilt(uint8_t value);
     uint8_t set_sgt(uint8_t value);
     uint8_t set_seimin(uint8_t value);
     uint8_t set_sedn(uint8_t value);
     uint8_t set_semax(uint8_t value);
     uint8_t set_seup(uint8_t value);
     uint8_t set_semin(uint8_t value);

     uint8_t set_DCCTRL(uint16_t dc_time, uint16_t dc_sg);

     uint8_t set_freewheel(uint8_t value);
     uint8_t set_pwm_symmetric(uint8_t value);
     uint8_t set_pwm_autoscale(uint8_t value);
     uint8_t set_pwm_freq(uint8_t value);
     uint8_t set_PWM_GRAD(uint8_t value);
     uint8_t set_PWM_AMPL(uint8_t value);

     uint8_t set_ENCM_CTRL(uint8_t value);

## Further methods

     uint8_t read_STAT();
     uint8_t read_REG( uint8_t address , uint32_t *data );
     uint8_t write_REG( uint8_t address, uint32_t data );
     uint8_t alter_REG(uint8_t address, uint32_t data, uint32_t mask);
	
     uint8_t set_GCONF(uint8_t bit, uint8_t value);
     uint8_t set_CHOPCONF(uint8_t position, uint8_t value);
     uint8_t alter_COOLCONF(uint32_t data, uint32_t mask);
     uint8_t set_COOLCONF(uint8_t position, uint8_t value);
     uint8_t alter_PWMCONF(uint32_t data, uint32_t mask);
     uint8_t set_PWMCONF(uint8_t position, uint8_t value);

## FAQ

### Why?
- why not?

### Can I run my RepRap with this?
- yes, just install the library and download the [Marlin Fork](https://github.com/makertum/Marlin)

## Troubleshooting

### Why doesn't it work
- If you're using the SilentStepsStick driver, did you `myStepper.set_I_scale_analog(1)`?
- Did you specify `myStepper.set_tbl(1)` and `myStepper.set_toff(8)` (example values)?
- Did you specify a motor current `myStepper.set_IHOLD_IRUN(31,31,5);`?

## Disclaimer / License
This is a work in progress. Any suggestions are heavily welcome. All scripts in this repository are licensed under the GNU Affero General Public License, version 3. Created by Moritz Walter 2016.
