/*!
 \file      MillingMachine.cpp
 \brief     The file contains the class which implements the control of milling utensils.
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
            To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#include "MillingMachine.h"

MillingMachine::MillingMachine(uint8_t e, uint8_t c) :
		enable_pin(e), control_pin(c), bit_speed(0), bit_dir(true), state(
				false) {
}

void MillingMachine::init() {
	pinMode(enable_pin, OUTPUT);
	analogWrite(control_pin, bit_speed);
	disable();
}


void MillingMachine::setSpindleSpeed(int s) {
	//TODO Implementation of MillingMachine::setBitSpeed
}

void MillingMachine::setSpindleDir(boolean d) {
	//TODO Implementation of MillingMachine::setBitDir
}

void MillingMachine::enable(){
	state = true;
	digitalWrite(enable_pin, LOW);
}

void MillingMachine::disable(){
	state = false;
	digitalWrite(enable_pin, HIGH);
}

void MillingMachine::preMotion(){}

void MillingMachine::postMotion(){}
