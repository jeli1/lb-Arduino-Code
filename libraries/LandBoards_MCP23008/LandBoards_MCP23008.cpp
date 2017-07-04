////////////////////////////////////////////////////////////////////////////
//  LandBoards_MCP23008.cpp - Library for MCP23008 cards
//  Created by Douglas Gilliland. 2015-11-23
//	Communication with the card is via I2C Two-wire interface
//  This library allows for both bit access
//  	Bit access (8 bits) via digitalWrite, digitalRead, pinMode
////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include <Wire.h>

#include "LandBoards_MCP23008.h"

////////////////////////////////////////////////////////////////////////////
// LandBoards_MCP23008 constructor
////////////////////////////////////////////////////////////////////////////

LandBoards_MCP23008::LandBoards_MCP23008(void)
{
	return;
}

////////////////////////////////////////////////////////////////////////////
// begin(addr) - Initialize the card
////////////////////////////////////////////////////////////////////////////

void LandBoards_MCP23008::begin(uint8_t addr)
{
	i2caddr = addr;

	Wire.begin();
	TWBR = 12;    	// go to 400 KHz I2C speed mode
	write8(MCP23008_IODIR,0xf0);
	write8(MCP23008_GPIO, 0x00);
	write8(MCP23008_INTCON,0x00);
	write8(MCP23008_IPOL,0xf0);
	write8(MCP23008_GPINTEN,0xf0);
	read8(MCP23008_GPIO);
	return;
}

////////////////////////////////////////////////////////////////////////////
// begin(void) - Initialize the card
////////////////////////////////////////////////////////////////////////////

void LandBoards_MCP23008::begin(void)
{
	begin(0);
	return;
}

////////////////////////////////////////////////////////////////////////////
// uint8_t PinMode(port, direction)
// Arduino.h defines for direction are:
//	#define INPUT 0x0
//	#define OUTPUT 0x1
//	#define INPUT_PULLUP 0x2
// Adafruit's pinMode implementation only supports INPUT and OUTPUT
// This implementation supports INPUT_PULLUP with separate calls to Adafruit
// This eliminates the separate pullup function
////////////////////////////////////////////////////////////////////////////

void LandBoards_MCP23008::pinMode(uint8_t bit, uint8_t d)
{
	uint8_t iodir;
	bit &= 7;
	iodir = read8(MCP23008_IODIR);
	if (d == INPUT) 
	{
		iodir |= 1 << bit; 
	} 
	else 
	{
		iodir &= ~(1 << bit);
	}
	write8(MCP23008_IODIR, iodir);
	if (d == INPUT)
	{
		pullUp(bit,LOW);		// Pullup disabled
	}
	else if (d == INPUT_PULLUP)
	{
		pullUp(bit,HIGH);			// Pullup enabled
	}
}

////////////////////////////////////////////////////////////////////////////
//  uint8_t pullUp(bit, d) 
////////////////////////////////////////////////////////////////////////////

void LandBoards_MCP23008::pullUp(uint8_t bit, uint8_t d) 
{
	uint8_t gppuCopy;
	bit &= 7;

	bit &= 7;

	gppuCopy = read8(MCP23008_GPPU);
	// set the pin and direction
	if (d == HIGH) {
		gppuCopy |= 1 << bit; 
	} else {
		gppuCopy &= ~(1 << bit);
	}
	// write the new GPIO
	write8(MCP23008_GPPU, gppuCopy);
}

////////////////////////////////////////////////////////////////////////////
//  uint8_t digitalWrite(uint8_t, uint8_t)
////////////////////////////////////////////////////////////////////////////

void LandBoards_MCP23008::digitalWrite(uint8_t bit, uint8_t d) 
{
	uint8_t gpioCopy;

	bit &= 7;
//	gpioCopy = read8(MCP23008_GPIO);
	gpioCopy = read8(MCP23008_OLAT);
	if (d == HIGH)
		gpioCopy |= 1 << bit;
	else
		gpioCopy &= ~(1 << bit);
	write8(MCP23008_OLAT, gpioCopy);
}

////////////////////////////////////////////////////////////////////////////
//  uint8_t digitalRead(bit)
////////////////////////////////////////////////////////////////////////////

uint8_t LandBoards_MCP23008::digitalRead(uint8_t bit)
{
	bit &= 7;
	return (read8(MCP23008_GPIO) >> bit) & 0x1;
}

////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////

void LandBoards_MCP23008::writeOLAT(uint8_t value)
{
	write8(MCP23008_OLAT,value);
}

////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////

uint8_t LandBoards_MCP23008::readOLAT(void)
{
	return (read8(MCP23008_OLAT));
}

	
////////////////////////////////////////////////////////////////////////////
// uint8_t read8(addr) 
////////////////////////////////////////////////////////////////////////////

uint8_t LandBoards_MCP23008::read8(uint8_t addr) 
{
	Wire.beginTransmission(MCP23008_ADDRESS | i2caddr);
	Wire.write((uint8_t)addr);	
	Wire.endTransmission();
	Wire.requestFrom(MCP23008_ADDRESS | i2caddr, 1);
	return Wire.read();
}

////////////////////////////////////////////////////////////////////////////
// void LandBoards_MCP23008::write8(addr, data) 
////////////////////////////////////////////////////////////////////////////

void LandBoards_MCP23008::write8(uint8_t addr, uint8_t data) 
{
	Wire.beginTransmission(MCP23008_ADDRESS | i2caddr);
	Wire.write((uint8_t)addr);
	Wire.write((uint8_t)data);
	Wire.endTransmission();
}
