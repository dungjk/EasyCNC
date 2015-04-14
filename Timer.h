/*!
 \file      Timer.h
 \brief     Useful tool to measure the execution time between a start and a n end point in the code.
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
            To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
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
