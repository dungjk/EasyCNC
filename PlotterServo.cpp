/*!
 \file      PlotterServo.cpp
 \brief     The file contains the classes which implement the control of an utensil composed by a pen which up/down motion is controlled by a servo motor.
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
            To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#include "PlotterServo.h"

#ifdef _SERVO
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
#endif
