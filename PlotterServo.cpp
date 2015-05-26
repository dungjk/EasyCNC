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

/*! \def INIT_TIMER1
 *  \details Below the description of the operations performed by the macro:
 *           - TCCR1A = 0;  set entire TCCR1A register to 0
 *           - TCCR1B = 0;  set entire TCCR1B register to 0
 *           - TCNT1 = 0;   initialize counter value to B0
 *			 - TCCR1B |= (1 << WGM12); turn on CTC mode
 */
#define INIT_TIMER1 {	TCCR1A = 0;    \
	             	 	TCCR1B = 0;    \
	             	 	TCNT1 = 0;   \
	             	 	TIMSK1 |= (1 << OCIE1A);    \
	             	 	TCCR1B |= (1 << WGM12); }

/*! \def SET_TIMER1
 *  \details It sets the value of the Output Compare Register A of the timer 1.
 *  \param x The value of the register
 */
#define SET_TIMER1(x) { OCR1A = x;}

/*! \def CLEAR_COUNTER_TIMER5
 *  \details Set to 0 the TCNT5 register.
 */
#define CLEAR_COUNTER_TIMER1 { TCNT1 = 0; }

/*! \def START_TIMER5
 *  \details The macro runs the timer 3 with a 1024 prescaler.
 *           - TCNT1 = 0; clear the TCNT5 register
 *           - TCCR1B |= (1 << CS10) | (1 << CS12); set 1024 prescaler
 */
#define START_TIMER1 {  TCNT1 = 0;  \
						TCCR1B |= (1 << CS10) | (1 << CS12);}
/*! \def STOP_TIMER5
 *  \details The macro stops the timer 3.
 */
#define STOP_TIMER1 {   TCCR1B &= ~((1 << CS10) | (1 << CS12));}

PlotterServo::PlotterServo(uint8_t p, uint8_t dv, uint8_t uv) : servo_pin(p), down_val(dv), up_val(uv), state(PEN_UP) {
}

void PlotterServo::init() {
	servo_pen.attach(servo_pin);
	state = PEN_UP;
	servo_pen.write(up_val);
	uint8_t oldSREG = SREG;
	cli();
	INIT_TIMER1;
	SET_TIMER1(SERVO_REFRESH);
	START_TIMER1;
	SREG = oldSREG;
}

boolean PlotterServo::up() {
	boolean ret = (state != PEN_UP);
	if (ret) {
		state = PEN_UP;
		servo_pen.write(up_val);
		delay(500);
	}
	return ret;
}

boolean PlotterServo::down() {
	boolean ret = (state != PEN_DOWN);
	if (ret) {
		state = PEN_DOWN;
		servo_pen.write(down_val);
		delay(500);
	}
	return ret;
}

void PlotterServo::preMotion() {
}

void PlotterServo::postMotion() {
}

ISR(TIMER1_COMPA_vect) {
	SoftwareServo::refresh();
}
