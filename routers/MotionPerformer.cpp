/*
 * MotioPerformer.cpp
 *
 *  Created on: 30/nov/2014
 *      Author: francescogiurlanda
 */

#include "MotionPerformer.h"

MotionPerformer::MotionPerformer(MotionPlanner *mp) :
		delay(0.0), d1(0), d2(0), isNotWorking(true) {
	motor[0] = motor[1] = motor[2] = NULL;
	planner = mp;
}

boolean MotionPerformer::loadMotion() {
	if (planner && isNotWorking) {
		uint8_t oldSREG;
		LM tmp = planner->getMotion();
		mx.tot_steps = abs(tmp.steps_x);
		my.tot_steps = abs(tmp.steps_y);
		mz.tot_steps = abs(tmp.steps_z);
		delay = tmp.delay;

		mx.act_steps = my.act_steps = mz.act_steps = 0;

		oldSREG = SREG;
		cli();
		if (tmp.steps_x * mx.orientation > 0) {
			*mx.pin_dir.outreg &= ~mx.pin_dir.bit;
		} else {
			*mx.pin_dir.outreg |= mx.pin_dir.bit;
		}

		if (tmp.steps_y * my.orientation > 0) {
			*my.pin_dir.outreg &= ~my.pin_dir.bit;
		} else {
			*my.pin_dir.outreg |= my.pin_dir.bit;
		}

		if (tmp.steps_z * mz.orientation > 0) {
			*mz.pin_dir.outreg &= ~mz.pin_dir.bit;
		} else {
			*mz.pin_dir.outreg |= mz.pin_dir.bit;
		}

		SREG = oldSREG;

		// Descending sort motor driver according the number of steps. The operation is crucial to apply the Bresenham algorithm.
		if (mx.tot_steps > my.tot_steps) {
			if (mx.tot_steps > mz.tot_steps) {
				if (my.tot_steps > mz.tot_steps) {
					//x > y > z
					motor[0] = &mx;
					motor[1] = &my;
					motor[2] = &mz;
				} else {
					//x > z > y
					motor[0] = &mx;
					motor[1] = &mz;
					motor[2] = &my;
				}
			} else {
				//z > x > y
				motor[0] = &mz;
				motor[1] = &mx;
				motor[2] = &my;
			}
		} else {
			if (my.tot_steps > mz.tot_steps) {
				if (mx.tot_steps > mz.tot_steps) {
					// y > x > z
					motor[0] = &my;
					motor[1] = &mx;
					motor[2] = &mz;
				} else {
					// y > z > x
					motor[0] = &my;
					motor[1] = &mz;
					motor[2] = &mx;
				}
			} else {
				//  z > y > x
				motor[0] = &mz;
				motor[1] = &my;
				motor[2] = &mx;
			}
		}

		// Computing the d* values of the Bresenham algorithm
		d1 = 2 * motor[1]->tot_steps - motor[0]->tot_steps; //It could be less than zero
		d2 = 2 * motor[2]->tot_steps - motor[0]->tot_steps;

		// Enables the motors
		oldSREG = SREG;
		cli();

		if (mx.tot_steps > 0) {
			*mx.pin_enable.outreg |= mx.pin_enable.bit;
		}
		if (my.tot_steps > 0) {
			*my.pin_enable.outreg |= my.pin_enable.bit;
		}
		if (mx.tot_steps > 0) {
			*mz.pin_enable.outreg |= mz.pin_enable.bit;
		}

		SREG = oldSREG;

		// Configure the Timer Interrupt
		cli();
		//disable global interrupt
		//TODO
		TCCR3A = 0;     // set entire TCCR3A register to 0
		TCCR3B = 0;     // same for TCCR3
		TCNT3 = 0;     //initialize counter value to 0B
		// set compare match register for 1hz increments
		OCR3A = 15624;		// = (16*10^6) / (1*1024) - 1 (must be <65536)

		TCCR3B |= (1 << WGM32); // turn on CTC mode
		TCCR3B |= (1 << CS30) | (1 << CS32);
		// enable timer compare interrupt:
		TIMSK3 |= (1 << OCIE3A);
		sei();
		//enable global interrupt

	}
	return true;
}

void MotionPerformer::stopMotion(int32_t &stpx, int32_t &stpy, int32_t &stpz) {
	cli();
	TCCR3B &= ~((1 << CS30) | (1 << CS32));
	sei();
	stpx = (*mx.pin_dir.outreg & mx.pin_dir.bit) ?
			(mx.orientation * mx.act_steps) : -(mx.orientation * mx.act_steps);
	stpy = (*my.pin_dir.outreg & my.pin_dir.bit) ?
			(my.orientation * my.act_steps) : -(my.orientation * my.act_steps);
	stpz = (*mz.pin_dir.outreg & mz.pin_dir.bit) ?
			(mz.orientation * mz.act_steps) : -(mz.orientation * mz.act_steps);
	mx.act_steps = mx.tot_steps = my.act_steps = my.tot_steps = mz.act_steps =
			mz.tot_steps = 0;
}

void MotionPerformer::pauseMotion(int32_t &stpx, int32_t &stpy, int32_t &stpz) {
	cli();
	TCCR3B &= ~((1 << CS30) | (1 << CS32));
	sei();
	stpx = (*mx.pin_dir.outreg & mx.pin_dir.bit) ?
			(mx.orientation * mx.act_steps) : -(mx.orientation * mx.act_steps);
	stpy = (*my.pin_dir.outreg & my.pin_dir.bit) ?
			(my.orientation * my.act_steps) : -(my.orientation * my.act_steps);
	stpz = (*mz.pin_dir.outreg & mz.pin_dir.bit) ?
			(mz.orientation * mz.act_steps) : -(mz.orientation * mz.act_steps);
	mx.tot_steps -= mx.act_steps;
	mx.act_steps = 0;
	my.tot_steps -= my.act_steps;
	my.act_steps = 0;
	mz.tot_steps -= mz.act_steps;
	mz.act_steps = 0;
}

void MotionPerformer::restartMotion() {
	cli();
	TCCR3B |= (1 << CS30) | (1 << CS32);
	sei();
}
