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
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <SPI.h>

#include "Trinamic_TMC2130.h"

Trinamic_TMC2130::Trinamic_TMC2130(uint8_t csPin)
{
  _csPin=csPin;
  _status=0;
  _debug="";
}

// initialize the driver with its CS/SS pin
void Trinamic_TMC2130::init() {
  pinMode(_csPin, OUTPUT);
  digitalWrite(_csPin, HIGH);
  initSPI();
  readStatus();
}

// initialize SPI
void Trinamic_TMC2130::initSPI() {
  SPI.setDataMode(TMC_SPI_DATA_MODE);
  SPI.setBitOrder(TMC_SPI_BIT_ORDER);
  SPI.setClockDivider(TMC_SPI_CLOCK_DIVIDER);
  SPI.begin();
}

// read status
uint8_t Trinamic_TMC2130::readStatus()
{
  initSPI();
  digitalWrite(_csPin, LOW);

  // read address
  _status = SPI.transfer(0x00);

  // flush 4 bytes
  for(int i=0;i<4;i++){
    SPI.transfer(0x00);
  }

  digitalWrite(_csPin, HIGH);

  return _status;
}

// read a register
uint8_t Trinamic_TMC2130::readRegister(uint8_t address, uint32_t *data)
{
  initSPI();
  digitalWrite(_csPin, LOW);

  // read address
  _status = SPI.transfer(address&~TMC_WRITE);

  // flush 4 bytes
  for(int i=0;i<4;i++){
    SPI.transfer(0x00);
  }

  digitalWrite(_csPin, HIGH);
  // restart transmission
  digitalWrite(_csPin, LOW);

  // read address
  _status = SPI.transfer(address&~TMC_WRITE);

  // retrieve data
  *data  = SPI.transfer(0x00)&0xFF;
  *data <<=8;
  *data |= SPI.transfer(0x00)&0xFF;
  *data <<=8;
  *data |= SPI.transfer(0x00)&0xFF;
  *data <<=8;
  *data |= SPI.transfer(0x00)&0xFF;

  digitalWrite(_csPin, HIGH);

  return _status;
}

// write to a register
uint8_t Trinamic_TMC2130::writeRegister(uint8_t address, uint32_t data)
{
  digitalWrite(_csPin, LOW);

  // write address
  _status = SPI.transfer(address|TMC_WRITE);

  // write data
  SPI.transfer((data>>24UL)&0xFF);
  SPI.transfer((data>>16UL)&0xFF);
  SPI.transfer((data>> 8UL)&0xFF);
  SPI.transfer((data>> 0UL)&0xFF);

  digitalWrite(_csPin, HIGH);

  return _status;
}

// alter a register using a bitmask
uint8_t Trinamic_TMC2130::alterRegister(uint8_t address, uint32_t data, uint32_t mask)
{
  uint32_t oldData, newData;

  readRegister( address, &oldData );

  newData = ( oldData&~mask ) | ( data&mask );

  writeRegister( address, newData );

  return _status;
}

// set single bits in the GCONF register
uint8_t Trinamic_TMC2130::setGCONF(uint8_t position, uint8_t value)
{
  alterRegister(TMC_REG_GCONF, uint32_t(value)<<position, 0x1UL<<position);

  return _status;
}

// set single bits or values in the chopconf register (constraining masks are applied if necessary)
uint8_t Trinamic_TMC2130::setCHOPCONF(uint8_t position, uint8_t value)
{
  alterRegister(TMC_REG_CHOPCONF, uint32_t(value)<<position, TMC_CHOPCONF_MASKS[position]<<position);

  return _status;
}

// set the IHOLD, IRUN and IHOLDDELAY (constraining masks are applied)
uint8_t Trinamic_TMC2130::setCurrent(uint8_t ihold, uint8_t irun, uint8_t iholddelay)
{
  uint32_t data;
  // adding ihold
  data = (( uint32_t(ihold)&TMC_IHOLD_MASK )<<TMC_IHOLD );
  // adding irun
  data |= (( uint32_t(irun)&TMC_IRUN_MASK )<<TMC_IRUN );
  // adding iholddelay
  data |= (( uint32_t(iholddelay)&TMC_IHOLDDELAY_MASK )<<TMC_IHOLDDELAY );

  // writing data
  writeRegister(TMC_REG_IHOLD_IRUN, data);

  return _status;
}

// setting the microstep resolution
uint8_t Trinamic_TMC2130::setMicrosteps(uint8_t value){
  uint8_t data;

  switch(value){
    case 1:
      data = 8;
    break;
    case 2:
      data = 7;
    break;
    case 4:
      data = 6;
    break;
    case 8:
      data = 5;
    break;
    case 16:
      data = 4;
    break;
    case 32:
      data = 3;
    break;
    case 64:
      data = 2;
    break;
    case 128:
      data = 1;
    break;
    case 256:
      data = 0;
    break;
  }

  setCHOPCONF(TMC_CHOPCONF_MRES, data);

  return _status;
}

uint8_t Trinamic_TMC2130::setTBL(uint8_t value){

  setCHOPCONF(TMC_CHOPCONF_TBL, value);

  return _status;
}

uint8_t Trinamic_TMC2130::setTOFF(uint8_t value){

  setCHOPCONF(TMC_CHOPCONF_TOFF, value);

  return _status;
}

uint8_t Trinamic_TMC2130::setIScale(uint8_t value){
  setGCONF(TMC_GCONF_I_SCALE, value);

  return _status;
}

// check the reset status
boolean Trinamic_TMC2130::isReset()
{
  return _status&TMC_SPISTATUS_RESET_MASK ? true : false;
}

// check the error status
boolean Trinamic_TMC2130::isError()
{
  return _status&TMC_SPISTATUS_ERROR_MASK ? true : false;
}

// check the stallguard status
boolean Trinamic_TMC2130::isStallguard()
{
  return _status&TMC_SPISTATUS_STALLGUARD_MASK ? true : false;
}

// check the standstill status
boolean Trinamic_TMC2130::isStandstill()
{
  return _status&TMC_SPISTATUS_STANDSTILL_MASK ? true : false;
}

// get debug messages
String Trinamic_TMC2130::debug()
{
  return _debug;
}
