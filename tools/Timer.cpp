/*
 * Timer.cpp
 *
 *  Created on: 13/set/2014
 *      Author: francescogiurlanda
 */

#include "Timer.h"

Timer::Timer() {
	time = tmin = tmax = 0;
}

void Timer::start() {
	time = micros();
}

uint32_t Timer::stop() {
	uint32_t delta = micros() - time;
	if (tmin == 0 || delta < tmin)
		tmin = delta;
	if (tmax == 0 || delta > tmax)
		tmax = delta;

	return delta;
}

uint32_t Timer::getMin(){
	return tmin;
}

uint32_t Timer::getMax(){
	return tmax;
}

