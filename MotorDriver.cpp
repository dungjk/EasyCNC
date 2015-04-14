/*!
 \file      MotorDriver.cpp
 \brief     Data structure that manages the motor driver.
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
            To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#include "MotorDriver.h"

MotorDriver::MotorDriver() :
		act_steps(0), tot_steps(0), global_steps(0), control_mode(0), ready(
				false), orientation(1), direction(0) {
}

MotorDriver::MotorDriver(uint8_t pst, uint8_t pdr, uint8_t pen) :
		act_steps(0), tot_steps(0), global_steps(0), control_mode(0), ready(
				true), orientation(1), direction(1) {
	byte port;                   //!< Port identifier
	volatile uint8_t *reg;//Pointer to the DDRx register (Data Direction Register)

	pin_dir.bit = digitalPinToBitMask(pdr);	// Get the bit mask of the specified pin number e.g. B00100000
	port = digitalPinToPort(pdr);// Get the port identifier of the specified pin number
	reg = portModeRegister(port);// Get the pointer to the DDR of the specified port
	pin_dir.outreg = portOutputRegister(port);//Get the pointer to the output register of the specified port

	cli();
	// Disable interrupts
	*reg |= pin_dir.bit;// Configure the pin in output mode setting the corresponding bit
	*pin_dir.outreg &= ~pin_dir.bit;			// Reset the output of the pin
	sei();

	pin_enable.bit = digitalPinToBitMask(pen);
	port = digitalPinToPort(pen);
	reg = portModeRegister(port);
	pin_enable.outreg = portOutputRegister(port);

	cli();
	*reg |= pin_enable.bit;
	*pin_enable.outreg |= pin_enable.bit; // Usually the enable is low active, so that it must be 1 to disable
	sei();

	pin_step.bit = digitalPinToBitMask(pst);
	port = digitalPinToPort(pst);
	reg = portModeRegister(port);
	pin_step.outreg = portOutputRegister(port);

	cli();
	*reg |= pin_step.bit;
	*pin_step.outreg &= ~pin_step.bit;
	sei();
}

void MotorDriver::setPins(uint8_t pst, uint8_t pdr, uint8_t pen) {
	byte port;                   // Port identifier
	volatile uint8_t *reg;// Pointer to the DDRx register (Data Direction Register)

	pin_dir.bit = digitalPinToBitMask(pdr);	// Get the bit mask of the specified pin number e.g. B00100000
	port = digitalPinToPort(pdr);// Get the port identifier of the specified pin number
	reg = portModeRegister(port);// Get the pointer to the DDR of the specified port
	pin_dir.outreg = portOutputRegister(port);//!< Get the pointer to the output register of the specified port

	cli();
	// Disable interrupts
	*reg |= pin_dir.bit;// Configure the pin in output mode setting the corresponding bit
	*pin_dir.outreg &= ~pin_dir.bit;			// Reset the output of the pin
	sei();

	pin_enable.bit = digitalPinToBitMask(pen);
	port = digitalPinToPort(pen);
	reg = portModeRegister(port);
	pin_enable.outreg = portOutputRegister(port);

	cli();
	*reg |= pin_enable.bit;
	*pin_enable.outreg |= pin_enable.bit; // Usually the enable is low active, so that it must be 1 to disable
	sei();

	pin_step.bit = digitalPinToBitMask(pst);
	port = digitalPinToPort(pst);
	reg = portModeRegister(port);
	pin_step.outreg = portOutputRegister(port);

	cli();
	*reg |= pin_step.bit;
	*pin_step.outreg &= ~pin_step.bit;
	sei();
}

