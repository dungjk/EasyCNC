/*
 * Laser.cpp
 *
 *  Created on: 20/apr/2015
 *      Author: francescogiurlanda
 */

#include "Laser.h"

Laser::Laser(uint8_t p, boolean ah) :
		state(false), active_high(ah), control_pin(p) {
}

void Laser::init() {
	pinMode(control_pin, OUTPUT);
	digitalWrite(control_pin, ((state ^ (~active_high)) & 0x01));
}

void Laser::switchOn() {
	state = true;
	digitalWrite(control_pin, ((state ^ (~active_high)) & 0x01));
}

void Laser::SwitchOff() {
	state = false;
	digitalWrite(control_pin, ((state ^ (~active_high)) & 0x01));
}

void Laser::preMotion(){}

void Laser::postMotion() {}
