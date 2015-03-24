/*
 * MillingMachine.cpp
 *
 *  Created on: 14/nov/2014
 *      Author: francescogiurlanda
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
