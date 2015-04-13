/*
 * MotionPlanner.cpp
 *
 *  Created on: 30/nov/2014
 *      Author: francescogiurlanda
 */

#include "MotionPlanner.h"

MotionPlanner::MotionPlanner(uint8_t pcl) :
		last(0), first(0), empty(true) {
	byte port;                   //!< Port identifier
	volatile uint8_t *reg; //!< Pointer to the DDRx register (Data Direction Register)

	pin_cooling.bit = digitalPinToBitMask(pcl);	//!< Get the bit mask of the specified pin number e.g. B00100000
	port = digitalPinToPort(pcl);//!< Get the port identifier of the specified pin number
	reg = portModeRegister(port);//!< Get the pointer to the DDR of the specified port
	pin_cooling.outreg = portOutputRegister(port);//!< Get the pointer to the output register of the specified port

	cli();
	//!< Disable interrupts
	*reg |= pin_cooling.bit;//!< Configure the pin in output mode setting the corresponding bit
	*pin_cooling.outreg &= ~pin_cooling.bit;			//!< Reset the output of the pin
	sei();

}

boolean MotionPlanner::addMotion(const LM &mov) {
	uint8_t old_sreg = SREG;
	cli();
	if (!empty && last == first) {
		pin_cooling.set();
		SREG = old_sreg;
		//buffer full
		return true;
	}
	//buffer not full
	movs[first] = mov;
	first = (++first) % BUFF_LEN;
	empty = false;
	SREG = old_sreg;
	return false;
}

boolean MotionPlanner::getMotion(LM &mov) {
	uint8_t old_sreg = SREG;
	cli();
	if (empty) {
		pin_cooling.reset();
		SREG = old_sreg;
		//buffer empty
		return true;
	}
	//buffer not empty
	mov = movs[last];
	last = (++last) % BUFF_LEN;
	empty = (last == first);
	SREG = old_sreg;
	return false;
}

void MotionPlanner::clear() {
	uint8_t old_sreg = SREG;
	cli();
	last = first;
	empty = true;
	SREG = old_sreg;
}

boolean MotionPlanner::isEmpty() const {
	return empty;
}

int MotionPlanner::getFreeBuffSize() const {
	if (empty)
		return BUFF_LEN;
	return ((first - last) > 0) ? BUFF_LEN - first + last : last - first;
}