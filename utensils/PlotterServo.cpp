/*
 * PlotterServo.cpp
 *
 *  Created on: 14/nov/2014
 *      Author: francescogiurlanda
 */

#include "PlotterServo.h"

PlotterServo::PlotterServo(uint8_t p, uint8_t dv, uint8_t uv) : pin(p), down_val(dv), up_val(uv), state(UP) {}

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
