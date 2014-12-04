/*
 * MotioPerformer.h
 *
 *  Created on: 30/nov/2014
 *      Author: francescogiurlanda
 */

#ifndef MOTIONPERFORMER_H_
#define MOTIONPERFORMER_H_

#include "../motor_drivers/MotorDriver.h"
#include "MotionPlanner.h"
#include "arduino.h"
//#include <avr/io.h>
//#include <avr/interrupt.h>

class MotionPerformer {
	float delay; //<! \brief It keeps the delay in microseconds between two steps update
	int32_t d1, d2; //<! \brief Variables used by  Bresenham algorithm.
	MotorDriver *motor[3]; //<! \brief Pointers to the motor drivers, they are ordered according the Bresenham algorithm. The first element stores the driver of the motor of the main axis, the others store the secondary axes.
	boolean isNotWorking; //<! \brief It is false if the MotionPerformer is already working, true otherwise.

public:
	MotorDriver mx;
	MotorDriver my;
	MotorDriver mz;
	MotionPlanner *planner;


	MotionPerformer(MotionPlanner *mp);
	void setIntNextStep();
	void setIntStepDown();
	boolean loadMotion();
	void stopMotion(int32_t &stpx, int32_t &stpy, int32_t &stpz);
	void pauseMotion(int32_t &stpx, int32_t &stpy, int32_t &stpz);
	void restartMotion();

};

#endif /* MOTIONPERFORMER_H_ */
