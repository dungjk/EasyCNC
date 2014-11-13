/*!
 \file      Utensils.cpp
 \brief     The file contains the classes which implement the control of different utensils.
 \details   The classes in this file allow to control different utensils,
 such as the head of a plotter, a spindle with a cutter bit, a blade to cut a paper sheet and so on.
 \author    Francesco Giurlanda
 \version   0.1
 \date      2014

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
            To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#include "Utensils.h"

// *******************************
// ***** PLOTTER WITH SERVO  *****
// *******************************
#ifdef _PLOTTER_SERVO

PlotterServo::PlotterServo(uint8_t p, uint8_t dv, uint8_t uv) : pin(p), down_val(dv), up_val(uv) {}

void PlotterServo::init() {
	servo_pen.attach(pin);
	state = UP;
	servo_pen.write(up_val);
}

boolean PlotterServo::up() {
	boolean ret = (state != UP);
	if(ret) {
		state = UP;
		servo_pen.write(up_val);
		delay(500);
	}
	return ret;
}

boolean PlotterServo::down() {
	boolean ret = (state != DOWN);
	if(ret) {
		state = DOWN;
		servo_pen.write(down_val);
		delay(500);
	}
	return ret;
}

#endif

// ****************************
// ***** MILLING MACHINE  *****
// ****************************
#ifdef _MILLING_MACHINE

MillingMachine::MillingMachine(uint8_t e, uint8_t c) :
		enable_pin(e), control_pin(c), bit_speed(0), bit_dir(true), state(
				false) {
}

void MillingMachine::init() {
	pinMode(enable_pin, OUTPUT);
	analogWrite(control_pin, bit_speed);
}


void MillingMachine::setSpindleSpeed(int s) {
	//TODO Implementation of MillingMachine::setBitSpeed
}

void MillingMachine::setSpindleDir(boolean d) {
	//TODO Implementation of MillingMachine::setBitDir
}

void MillingMachine::enable(){
	state = true;
	digitalWrite(enable_pin, HIGH);
}

void MillingMachine::disable(){
	state = false;
	digitalWrite(enable_pin, LOW);
}

void MillingMachine::preMotion(){}

void MillingMachine::postMotion(){}

#endif

// *******************************
