/*
 * MotionPlanner.h
 *
 *  Created on: 30/nov/2014
 *      Author: francescogiurlanda
 */

#ifndef MOTIONPLANNER_H_
#define MOTIONPLANNER_H_

#include "arduino.h"

#define BUFF_LEN 20

struct LinearMotion {
	uint16_t delay;				//<! It will be the value of the OCR3A register.
	int32_t steps_x, steps_y, steps_z;  //<! They could be negative, it specify the direction.
};

typedef struct LinearMotion LM;

class MotionPlanner {
	LM movs[BUFF_LEN];
	uint8_t last, first;
public:
	MotionPlanner();
	LM getMotion();
	boolean addMotion(LM mov);

};

#endif /* MOTIONPLANNER_H_ */
