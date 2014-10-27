/*
 *  Timer.h
 *
 *  Created on: 13/set/2014
 *      Author: francescogiurlanda
 */
#pragma once

#include <Arduino.h>
#include <stdint.h>

class Timer {
	uint32_t time, t_min, t_max, t_average;
	uint32_t sample;
public:
	Timer();
	void start();
	uint32_t stop();
	uint32_t getMin();
	uint32_t getMax();
	uint32_t getAverage();
};
