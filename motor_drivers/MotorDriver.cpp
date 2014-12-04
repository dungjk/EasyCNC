/*
 * MotorDriver.cpp
 *
 *  Created on: 29/nov/2014
 *      Author: francescogiurlanda
 */

#include "MotorDriver.h"

MotorDriver::MotorDriver() :
		act_steps(0), tot_steps(0), control_mode(0), ready(false), orientation(1) {
}

MotorDriver::MotorDriver(uint8_t pst, uint8_t pdr, uint8_t pen) :
		act_steps(0), tot_steps(0), control_mode(0), ready(true), orientation(1){
	byte port;
	volatile uint8_t *reg;
	uint8_t oldSREG;

	pin_dir.bit = digitalPinToBitMask(pdr);
	port = digitalPinToPort(pdr);
	reg = portModeRegister(port);
	pin_dir.outreg = portOutputRegister(port);

	oldSREG = SREG;
	cli();
	*reg |= pin_dir.bit;
	*pin_dir.outreg &= ~pin_dir.bit;
	SREG = oldSREG;

	pin_enable.bit = digitalPinToBitMask(pen);
	port = digitalPinToPort(pen);
	reg = portModeRegister(port);
	pin_enable.outreg = portOutputRegister(port);

	oldSREG = SREG;
	cli();
	*reg |= pin_enable.bit;
	*pin_enable.outreg |= pin_enable.bit;  // Usually the enable is low active, so that it must be 1 to disable
	SREG = oldSREG;

	pin_step.bit = digitalPinToBitMask(pdr);
	port = digitalPinToPort(pdr);
	reg = portModeRegister(port);
	pin_step.outreg = portOutputRegister(port);

	oldSREG = SREG;
	cli();
	*reg |= pin_step.bit;
	*pin_step.outreg &= ~pin_step.bit;
	SREG = oldSREG;
}

void MotorDriver::setPins(uint8_t pst, uint8_t pdr, uint8_t pen){
	byte port;
		volatile uint8_t *reg;
		uint8_t oldSREG;

		pin_dir.bit = digitalPinToBitMask(pdr);
		port = digitalPinToPort(pdr);
		reg = portModeRegister(port);
		pin_dir.outreg = portOutputRegister(port);

		oldSREG = SREG;
		cli();
		*reg |= pin_dir.bit;
		*pin_dir.outreg &= ~pin_dir.bit;
		SREG = oldSREG;

		pin_enable.bit = digitalPinToBitMask(pen);
		port = digitalPinToPort(pen);
		reg = portModeRegister(port);
		pin_enable.outreg = portOutputRegister(port);

		oldSREG = SREG;
		cli();
		*reg |= pin_enable.bit;
		*pin_enable.outreg |= pin_enable.bit;  // Usually the enable is low active, so that it must be 1 to disable
		SREG = oldSREG;

		pin_step.bit = digitalPinToBitMask(pdr);
		port = digitalPinToPort(pdr);
		reg = portModeRegister(port);
		pin_step.outreg = portOutputRegister(port);

		oldSREG = SREG;
		cli();
		*reg |= pin_step.bit;
		*pin_step.outreg &= ~pin_step.bit;
		SREG = oldSREG;
}

