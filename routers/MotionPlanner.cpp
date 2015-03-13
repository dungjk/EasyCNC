/*
 * MotionPlanner.cpp
 *
 *  Created on: 30/nov/2014
 *      Author: francescogiurlanda
 */

#include "MotionPlanner.h"

MotionPlanner::MotionPlanner() :
		last(0), first(0), empty(true) {

}

boolean MotionPlanner::addMotion(const LM &mov) {
	uint8_t old_sreg = SREG;
	cli();
	if (!empty && last == first) {
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

int MotionPlanner::getFreeBuffSize() const{
	if(empty)
		return BUFF_LEN;
	return  ((first - last) > 0)? BUFF_LEN - first + last : last - first;
}
