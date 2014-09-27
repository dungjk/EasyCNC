/*
 * GCode.h
 *
 *  Created on: 26/lug/2014
 *      Author: francescogiurlanda
 */

#ifndef GCODE_H_
#define GCODE_H_

#include "config.h"
#include "GCode_def.h"
#include "Arduino.h"
#include "CNC_Router.h"
#include "Utensils.h"
#include <stdint.h>

/*! \class GCode
 *  \brief The class interprets lines of RS274/NGC language.
 *  \details The class interprets lines of RS274/NGC language that is a language to control CNC
 */
class GCode {
	boolean getWord(char &code, float &val, uint8_t &pos, uint8_t len);

public:
	String line;
	float feed_rate;
	float drill_speed;

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

	uint8_t parser_status;
	uint8_t last_word[16];
	PositionXY new_pos_xy;
	float new_pos_z;
	float params[PARAMS];
	boolean pars_spec[PARAMS];

	CNC_Router *router;

/*#if defined(_PLOTTER_SERVO)
	PlotterServo *utensil;
#elif  defined(_MILLING_MACHINE)*/
	MillingMachine *utensil;
//#endif

	GCode(CNC_Router*, MillingMachine*);
	int parseLine();
	void removeSpaces();
	int getInt(int&);
	boolean getFloat(uint8_t&, float&);
};

#endif /* GCODE_H_ */
