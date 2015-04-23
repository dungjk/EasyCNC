/*!
 \file MotionPerformer.cpp
 \brief Implementation of the class MotionPerformer.
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015
 \warning   If you change Arduino board some parameters could change too.
 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#include "MotionPerformer.h"

/*! \def INIT_TIMER3
 *  \details Below the description of the operations performed by the macro:
 *           - TCCR3A = 0;  set entire TCCR3A register to 0
 *           - TCCR3B = 0;  set entire TCCR3B register to 0
 *           - TCNT3 = 0;   initialize counter value to B0
 *			 - TCCR3B |= (1 << WGM32); turn on CTC mode
 */
#define INIT_TIMER3 {	TCCR3A = 0;    \
	             	 	TCCR3B = 0;    \
	             	 	TCNT3 = 0;   \
	             	 	TIMSK3 |= (1 << OCIE3A);    \
	             	 	TCCR3B |= (1 << WGM32); }

/*! \def SET_TIMER3
 *  \details It sets the value od the Output Compare Register A of the timer 3.
 *  \param x The value of the register
 */
#define SET_TIMER3(x) { OCR3A = x;}

/*! \def CLEAR_COUNTER_TIMER3
 *  \details Set to 0 the TCNT3 register.
 */
#define CLEAR_COUNTER_TIMER3 { TCNT3 = 0; }

/*! \def START_TIMER3
 *  \details The macro runs the timer 3 with a 1024 prescaler.
 *           - TIMSK3 |= (1 << OCIE3A); turn on CTC mode
 *           - TCCR3B |= (1 << CS30) | (1 << CS32); set 1024 prescaler
 */
//#define START_TIMER3 {  TCNT3 = 0;  \
//						TCCR3B |= (1 << CS32);}
#define START_TIMER3 {  TCNT3 = 0;  \
						TCCR3B |= (1 << CS30) | (1 << CS32);}
/*! \def STOP_TIMER3
 *  \details The macro stops the timer 3.
 */
//#define STOP_TIMER3 {   TCCR3B &= ~(1 << CS32);}
#define STOP_TIMER3 {   TCCR3B &= ~((1 << CS30) | (1 << CS32));}

/*! \def INIT_TIMER4
 *  \details Below the description of the operations performed by the macro:
 *           - TCCR4A = 0;  set entire TCCR4A register to 0
 *           - TCCR4B = 0;  set entire TCCR4B register to 0
 *           - TCNT4 = 0;   initialize counter value to B0
 *			 - TCCR4B |= (1 << WGM42); turn on CTC mode
 */
#define INIT_TIMER4 {	TCCR4A = 0;  \
	             	 	TCCR4B = 0;     \
	             	 	TCNT4 = 0;  \
	             	 	TIMSK4 |= (1 << OCIE4A); \
	             	 	TCCR4B |= (1 << WGM42);}
/*! \def SET_TIMER4
 *  \details It sets the value od the Output Compare Register A of the timer 4.
 *  \param x The value of the register
 */
#define SET_TIMER4(x) { OCR4A = x;}

/*! \def CLEAR_COUNTER_TIMER4
 *  \details Set to 0 the TCNT4 register.
 */
#define CLEAR_COUNTER_TIMER4 { TCNT4 = 0; }

/*! \def START_TIMER4
 *  \details The macro runs the timer 4 with a 8 prescaler.
 *           - TIMSK4 |= (1 << OCIE4A); turn on CTC mode
 *           - TCCR4B |= (1 << CS41); set 8 prescaler
 */
#define START_TIMER4 {  TCNT4 = 0;  \
						TCCR4B |= (1 << CS41);}

/*! \def STOP_TIMER4
 *  \details The macro stops the timer 4.
 */
#define STOP_TIMER4 {	TCCR4B &= ~(1 << CS41);}

int n_update = 0;    //debug
int n_stepdwn = 0;   //debug
boolean state = false; //debug

MotionPerformer *_mp = NULL; //!< It is used to bind the timer3 and timer4 handlers respectively to MotionPerformer::update and MotionPerformer::stepPulse functions of the last instanced MotionPerformer object.

MotionPerformer::MotionPerformer(MotionPlanner *mp) :
		d1(0), d2(0), current_fr(0.0), idle(true) {
	motor[0] = motor[1] = motor[2] = NULL;
	planner = mp;

	_mp = this;
}

boolean MotionPerformer::loadMotion() {
	uint8_t old_sreg; //!< Here I use this technique to restore the interrupts because i don't know the value of the I flag in SREG. Someone could have already the cli() function.

	if (planner) {
		LM tmp;
		if (planner->getMotion(tmp)) {
			idle = true;
			return true;
		}

		current_fr = tmp.act_fr;
		old_sreg = SREG;
		cli();
		if (tmp.steps_x != 0) {
			mx.tot_steps = abs(tmp.steps_x);
			mx.direction = mx.tot_steps / tmp.steps_x; // It will be 1 or -1
			if (mx.direction * mx.orientation > 0) {
				mx.pin_dir.reset();
			} else {
				mx.pin_dir.set();
			}
		} else {
			mx.tot_steps = 0;
		}

		if (tmp.steps_y != 0) {
			my.tot_steps = abs(tmp.steps_y);
			my.direction = my.tot_steps / tmp.steps_y;
			if (my.direction * my.orientation > 0) {
				my.pin_dir.reset();
			} else {
				my.pin_dir.set();
			}

		} else {
			my.tot_steps = 0;
		}

		if (tmp.steps_z != 0) {
			mz.tot_steps = abs(tmp.steps_z);
			mz.direction = mz.tot_steps / tmp.steps_z;
			if (mz.direction * mz.orientation > 0) {
				mz.pin_dir.reset();
			} else {
				mz.pin_dir.set();
			}
		} else {
			mz.tot_steps = 0;
		}

		mx.act_steps = my.act_steps = mz.act_steps = 0;

		SREG = old_sreg;

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
		d1 = (motor[1]->tot_steps > 0) ?
				2 * motor[1]->tot_steps - motor[0]->tot_steps : 0; //It could be less than zero
		d2 = (motor[2]->tot_steps > 0) ?
				2 * motor[2]->tot_steps - motor[0]->tot_steps : 0;

		// Configure the Timer Interrupt
		old_sreg = SREG;
		cli();

		SET_TIMER3(tmp.delay);   // = (16*10^6) / (1*1024) - 1 (must be <65536)

		SREG = old_sreg;
		return false;
	}
	return true;
}

void MotionPerformer::init() {
	uint8_t oldSREG = SREG;
	//disable global interrupt
	cli();
	//disable global interrupt
	INIT_TIMER3
	;
	INIT_TIMER4
	;
	SET_TIMER3(IDLE_FREQ);
	SET_TIMER4(DELAY_STEP_UP);
	START_TIMER3
	;
	SREG = oldSREG;
}

void MotionPerformer::startMotion() {
	uint8_t old_sreg = SREG; //!< Here I use this technique to restore the interrupts because i don't know the value of the I flag in SREG. Someone could have already the cli() function.
	cli();
	START_TIMER3
	;
	SREG = old_sreg;
}

void MotionPerformer::stopMotion() {
	uint8_t old_sreg = SREG;
	cli();
	STOP_TIMER3;
	SET_TIMER3(IDLE_FREQ);
	disableMotors();
	mx.act_steps = mx.tot_steps = my.act_steps = my.tot_steps = mz.act_steps =
			mz.tot_steps = 0;
	idle = true;
	SREG = old_sreg;
}

void MotionPerformer::pauseMotion() {
	uint8_t old_sreg = SREG;
	cli();
	STOP_TIMER3;
	disableMotors();
	SREG = old_sreg;
}

void MotionPerformer::restartMotion() {
	enableMotors();
	startMotion();
}

void MotionPerformer::update() {
	uint8_t old_sreg; //!< Here I use this technique to restore the interrupts because i don't know the value of the I flag in SREG. Someone could have already the cli() function.
	if (idle) {
		if (loadMotion()) {
			// No job
			return;
		}
		// Job loaded
		idle = false;
		enableMotors();
	}else if (motor[0]->act_steps == motor[0]->tot_steps) {
		//Case of end of the segment, loading new job
		if (loadMotion()) {
			old_sreg = SREG;
			cli();
			SET_TIMER3(IDLE_FREQ);
			SREG = old_sreg;
			disableMotors();
			idle = true;
			return;
		}
		enableMotors();
	}

	old_sreg = SREG;
	cli();
	motor[0]->pin_step.set();

	if (motor[1]->tot_steps > 0) {
		if (d1 >= 0) {
			//d = d -2 * DX + 2 * DY;
			d1 += -2 * motor[0]->tot_steps + 2 * motor[1]->tot_steps;
			motor[1]->pin_step.set();
			motor[1]->act_steps++;
			motor[1]->global_steps += motor[1]->direction;
		} else {
			//d = d + 2 * DY;
			d1 += 2 * motor[1]->tot_steps;
		}
	}
	if (motor[2]->tot_steps > 0) {
		if (d2 >= 0) {
			//d = d -2 * DX + 2 * DY;
			d2 += -2 * motor[0]->tot_steps + 2 * motor[2]->tot_steps;
			motor[2]->pin_step.set();
			motor[2]->act_steps++;
			motor[2]->global_steps += motor[2]->direction;
		} else {
			//d = d + 2 * DY;
			d2 += 2 * motor[2]->tot_steps;
		}
	}
	motor[0]->act_steps++;
	motor[0]->global_steps += motor[0]->direction;

	START_TIMER4
	;
	SREG = old_sreg;
}

void MotionPerformer::stepPulse() {
	uint8_t old_sreg = SREG; //!< Here I use this technique to restore the interrupts because i don't know the value of the I flag in SREG. Someone could have already the cli() function.
	cli();
	for (int i = 0; i < 3; i++) {
		motor[i]->pin_step.reset();
	}
	STOP_TIMER4
	;
	SREG = old_sreg;
}

void MotionPerformer::disableMotors() {
	uint8_t old_sreg = SREG; //!< Here I use this technique to restore the interrupts because i don't know the value of the I flag in SREG. Someone could have already the cli() function.
	cli();
	//disable motors
	mx.pin_enable.set();
	my.pin_enable.set();
	mz.pin_enable.set();

	SREG = old_sreg;
}

void MotionPerformer::enableMotors() {
	uint8_t old_sreg = SREG; //!< Here I use this technique to restore the interrupts because i don't know the value of the I flag in SREG. Someone could have already the cli() function.
	// Enables the motors
	cli();

	if (mx.tot_steps > 0) {
		mx.pin_enable.reset();   // LOW active
	} else {
		mx.pin_enable.set();
	}
	if (my.tot_steps > 0) {
		my.pin_enable.reset();
	} else {
		my.pin_enable.set();
	}
	if (mz.tot_steps > 0) {
		mz.pin_enable.reset();
	} else {
		mz.pin_enable.set();
	}

	SREG = old_sreg;
}

bool MotionPerformer::isNotWorking() {
	return idle;
}

ISR(TIMER3_COMPA_vect) {
	if (_mp != NULL) {
		_mp->update();
	}
}

ISR(TIMER4_COMPA_vect) {
	if (_mp != NULL) {
		_mp->stepPulse();
	}
}
