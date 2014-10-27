/*
 * Timer.cpp
 *
 *  Created on: 13/set/2014
 *      Author: francescogiurlanda
 */

#include "Timer.h"

Timer::Timer() {
	time = t_min = t_max = t_average = 0;
	sample = 1;
}

void Timer::start() {
	time = micros();
}

uint32_t Timer::stop() {
	uint32_t delta = micros() - time;
	if (t_min == 0 || delta < t_min)
		t_min = delta;
	if (t_max == 0 || delta > t_max)
		t_max = delta;

	t_average = (t_average * (sample - 1) + delta) / sample ;
	return delta;
}

uint32_t Timer::getMin(){
	return t_min;
}

uint32_t Timer::getMax(){
	return t_max;
}

uint32_t Timer::getAverage(){
	return t_average;
}

