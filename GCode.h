/*
 * GCode.h
 *
 *  Created on: 26/lug/2014
 *      Author: francescogiurlanda
 */

#ifndef GCODE_H_
#define GCODE_H_

#include "GCode_def.h"
#include "Arduino.h"
#include "CNC_Router.h"
#include "Utensils.h"
#include <stdint.h>

class GCode {
public:
	String line;
	uint8_t moving;
	uint8_t plane_select;
	uint8_t distanse_mode;
	uint8_t forward_mode;
	uint8_t unit;
	uint8_t raius_compensation;
	uint8_t utensil_offset;
	uint8_t loop_return;
	uint8_t coordinate;
	uint8_t path_control;
	CNC_Router *router;
#ifdef _PLOTTER_SERVO
	PlotterServo *utensil;
#endif
#ifdef _MILLING_MACHINE
	MillingMachine *utensil;
#endif

	GCode(CNC_Router*);
	int parseLine();
	void removeSpaces();
	int getInt(int&);
	float getFloat(int&);
};

#endif /* GCODE_H_ */
