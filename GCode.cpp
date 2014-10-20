/*
 * GCode.cpp
 *
 *  Created on: 26/lug/2014
 *      Author: francescogiurlanda
 */

#include "GCode.h"

GCode::GCode(CNC_Router *rt, MillingMachine *ml) {
	parser_status = STATUS_OK;
	drill_speed = feed_rate = 0.0;
	//new_pos_z = 0.0;
	memset(last_word, UNSPECIFIED, 16);
	router = rt;
	utensil = ml;
}

int GCode::getInt(int &i) {
	int ptr = i;
	while ((line[ptr] >= '0' && line[ptr] <= '9') || line[ptr] == '-'
			|| line[ptr] == '+') {
		ptr++;
	}
	String tmp = line.substring(i, ptr);
	i = ptr - 1;
	return tmp.toInt();
}

boolean GCode::getFloat(uint8_t &pos, float &val) {
	int ptr = pos;
	while ((line[ptr] >= '0' && line[ptr] <= '9') || line[ptr] == '.'
			|| line[ptr] == '-' || line[ptr] == '+') {
		ptr++;
	}
	if (ptr == pos)
		return true;
	String tmp = line.substring(pos, ptr);
	pos = ptr;
	val = tmp.toFloat();
	return false;
}

void GCode::returnStatus() {
	Serial.print(router->getPos().X());
	Serial.print(":");
	Serial.print(router->getPos().Y());
	Serial.print(":");
	Serial.print(router->getPos().Z());
	Serial.print(":");
	Serial.print(feed_rate * 60);
	Serial.print(":");
	Serial.println(parser_status);
	/*Serial.print(":");
	 Serial.println(line);*/
}

void GCode::resetStatus() {
	parser_status = STATUS_OK;
}

void GCode::cycleG81() {

	if (!(pars_spec[PARAM_Z] && pars_spec[PARAM_R])
			|| ((pars_spec[PARAM_P]) && params[PARAM_P] < 0)
			|| ((pars_spec[PARAM_L]) && params[PARAM_L] < 1
					&& last_word[GROUP3] == G91)) {
		parser_status = STATUS_SYNTAX_ERROR;
		return;
	}

	int loops = (pars_spec[PARAM_L]) ? params[PARAM_L] : 1;
	PositionXYZ old_p = router->getPos();

	for (int l = 0; l < loops; l++) {
		//Step 1: rapid motion to XY
		router->moveToXY(new_pos);
		runMotion();
		returnStatus();
		//Step 2: rapid motion to Z specified by R parameter (clear position)
		if (l == 0) {
			router->moveTo(
					router->getPos() + PositionXYZ(0.0, 0.0, params[PARAM_R]));
			runMotion();
			returnStatus();
		}

		//Step 3: motion on Z-axis to the value Z specified in the cycle command
		router->moveTo(router->getPos() + PositionXYZ(0.0, 0.0, new_pos.Z()),
				feed_rate);
		runMotion();
		returnStatus();
		//Step 4: return to the clear position specified by R with a rapid motion
		if (last_word[GROUP10] == G98 && old_p.Z() > params[PARAM_R]) {

			router->moveTo(
					router->getPos()
							+ PositionXYZ(0.0, 0.0,
									old_p.Z() - params[PARAM_R] - new_pos.Z()));
		} else {
			router->moveTo(
					router->getPos() + PositionXYZ(0.0, 0.0, params[PARAM_R]));
		}
		runMotion();
		returnStatus();
	}
}

void GCode::motionG2G3() {
	PositionXYZ center;
	PositionXYZ start_p = router->getPos();
	float alpha, angle_next_p, angle_end, r;
	boolean can_move = false;

	if ((pars_spec[PARAM_X] || pars_spec[PARAM_Y]) && pars_spec[PARAM_R]) {

		can_move = true;
		float beta = start_p.angleXY(new_pos);
		float dist = start_p.moduleXY(new_pos);
		r = params[PARAM_R];
		float gamma = acos(dist / (2 * r));

		//it depends on the sign of the radius value
		if (last_word[GROUP1] == G3) {
			center.polarXY(r, (r > 0) ? beta + gamma : beta - gamma);
		} else {
			center.polarXY(r, (r > 0) ? beta - gamma : beta + gamma);
		}

		//center.Z(start_p.Z());

		center += start_p;  // the center of the cyrcle

	} else if ((pars_spec[PARAM_X] || pars_spec[PARAM_Y])
			&& (pars_spec[PARAM_I] || pars_spec[PARAM_J])) {

		can_move = true;
		PositionXYZ offset(params[PARAM_I], params[PARAM_J], 0.0);
		center = start_p;
		r = offset.module();
		center += offset;  // the center of the cyrcle

	} else {
		parser_status = STATUS_SYNTAX_ERROR;
	}

	if (can_move) {
		//Computation of the angle for each step of the arch
		alpha = 2 * asin(ARCH_DEFINITION / (2 * r));
		//Computation of the angle from start_p to new_pos_xy
		angle_end = center.angleXY(new_pos);
		angle_next_p = center.angleXY(start_p);

		PositionXYZ tmp;

		if (last_word[GROUP1] == G3) {
			if (angle_end < angle_next_p) // In the case of the atan2 function returns a angle_end value smaller then the start angle.
				angle_end += 2 * PI;

			angle_next_p += alpha;
			while (angle_next_p < angle_end) {
				router->moveTo(center + tmp.polarXY(r, angle_next_p), feed_rate);
				runMotion();
				angle_next_p += alpha;
			}
			router->moveTo(new_pos, feed_rate);
			runMotion();
		} else {
			if (angle_end > angle_next_p) // In the case of the atan2 function returns a angle_end value greater then the start angle.
				angle_end += 2 * PI;

			angle_next_p -= alpha;
			while (angle_next_p > angle_end) {
				router->moveTo(center + tmp.polarXY(r, angle_next_p), feed_rate);
				runMotion();
				angle_next_p -= alpha;
			}
			router->moveTo(new_pos, feed_rate);
			runMotion();
		}
	}
}

void GCode::motionG0G1() {
	if (pars_spec[PARAM_X] || pars_spec[PARAM_Y] || pars_spec[PARAM_Z]) {

		if (last_word[GROUP1] == G0) {
			//utensil->moveTo(new_pos_z);
			router->moveTo(new_pos);
		} else {
			//utensil->moveTo(new_pos_z, feed_rate);
			router->moveTo(new_pos, feed_rate);
		}
		runMotion();
	} else {
		parser_status = STATUS_SYNTAX_ERROR;
	}

}

boolean GCode::getWord(char &code, float &val, uint8_t &pos, uint8_t len) {
	if (pos == len) {
		//parser_status = STATUS_OK;
		return false;
	}
	if (line[pos] < 'A' || line[pos] > 'Z') {
		parser_status = STATUS_BAD_WORD;
		return false;
	}
	code = line[pos];
	pos++;

	if (getFloat(pos, val)) {

		parser_status = STATUS_BAD_WORD;
		return false;
	}
	return true;
}

int GCode::runMotion() {
	while (!router->update()) {
		////TODO add the support to limit switch
	}
}

int GCode::parseLine() {
	removeSpaces();
	//Special commands that starts with "$"
	if (line[0] == '$') {
		switch (line.length()) {
		case 2:
			switch (line[1]) {
			case 'r':
				resetStatus();
				break;
			case 'p':
				router->resetPos();
				//utensil->resetPos();
				break;
			}
		}

		returnStatus();
		return parser_status;
	}

	if (parser_status != STATUS_OK) {
		returnStatus();
		return parser_status;
	}

	//G-Code commands
	if (last_word[GROUP3] == G91) {
		new_pos = PositionXYZ();
	} else {
		new_pos = router->getPos();
	}
	boolean motion_command = false;
	boolean pause = false;
	for (int i = 0; i < PARAMS; i++) {
		if (i == PARAM_R) {
			//this condition depends on the behavior of the parameter R when a drilling cycle is active. It is a "sticky" number in this case.
			if (last_word[GROUP1] < G81 || last_word[GROUP1] > G89) {
				params[i] = 0.0;
				pars_spec[i] = false;
			}
		} else {
			params[i] = 0.0;
			pars_spec[i] = false;
		}
	}

	uint8_t len = line.length();
	uint8_t ptr = 0;
	char type;
	float val;
	// Line parsing
	while (getWord(type, val, ptr, len)) {
		switch (type) {
		case 'G':
			switch ((int) val) {
			case 0:  // Rapid positioning
				motion_command = true;
				last_word[GROUP1] = G0;
				break;
			case 1:
				motion_command = true;
				last_word[GROUP1] = G1;
				break;
			case 2:
				motion_command = true;
				last_word[GROUP1] = G2;
				break;
			case 3:
				motion_command = true;
				last_word[GROUP1] = G3;
				break;
			case 4:
				pause = true;
				break;
			case 17:
				// plane XY
				last_word[GROUP2] = G17;
				break;
			case 18:
				// plane XZ (unsupported)
				last_word[GROUP2] = G18;
				break;
			case 19:
				// plane YZ (unsupported)
				last_word[GROUP2] = G19;
				break;
			case 20:
				// Inches
				last_word[GROUP6] = G20;
				break;
			case 21:
				// Millimeters
				last_word[GROUP6] = G21;
				break;
			case 40:
				// Disable radius compensation
				last_word[GROUP7] = G40;
				break;
			case 54:
				//Coordinate job 1
				last_word[GROUP12] = G54;
				break;
			case 64:
				// Continuous mode
				last_word[GROUP13] = G64;
				break;
			case 81:
				motion_command = true;
				last_word[GROUP1] = G81;
				break;
			case 90:
				// Absolute distance mode
				last_word[GROUP3] = G90;
				router->setAbsolPos();
				utensil->setAbsolPos();
				break;
			case 91:
				// Incremental distance mode
				last_word[GROUP3] = G91;
				router->setIncrPos();
				utensil->setIncrPos();
				break;
			case 93:
				// Forwarding mode in time inverse
				last_word[GROUP5] = G93;
				break;
			case 94:
				// Forwarding mode in units per minute
				last_word[GROUP5] = G94;
				break;
			case 98:
				// Initial level of return from a loop
				last_word[GROUP10] = G98;
				break;
			case 99:
				// Point R of return from a loop
				last_word[GROUP10] = G99;
				break;

			default:
				/*DBG("Unsupported command ");
				 DBG(type);
				 DBGNL(val);*/
				parser_status = STATUS_UNSUPPORTED;
			}
			;
			break;
		case 'M':
			/*DBG(type);
			 DBGNL((int )trunc(val));*/
			break;
		case 'F':
			//the F's value is in units/minute, instead the motion control needs a feed rate in units/s
			feed_rate = val / 60.0;
			break;
		case 'R':
			motion_command = pars_spec[PARAM_R] = true;
			params[PARAM_R] = val;
			break;
		case 'S':
			drill_speed = val;
			break;
		case 'X':
			new_pos.X(val);
			params[PARAM_X] = val;
			motion_command = pars_spec[PARAM_X] = true;
			break;
		case 'Y':
			new_pos.Y(val);
			params[PARAM_Y] = val;
			motion_command = pars_spec[PARAM_Y] = true;
			break;
		case 'Z':
			new_pos.Z(val);
			params[PARAM_Z] = val;
			motion_command = pars_spec[PARAM_Z] = true;
			break;
		case 'I':
			pars_spec[PARAM_I] = true;
			params[PARAM_I] = val;
			break;
		case 'J':
			pars_spec[PARAM_J] = true;
			params[PARAM_J] = val;
			break;
		case 'L':
			pars_spec[PARAM_L] = true;
			params[PARAM_L] = val;
			break;
		case 'P':
			params[PARAM_P] = val;
			pars_spec[PARAM_P] = true;
			break;
		default:
			parser_status = STATUS_UNSUPPORTED;
		};

	}

	// check status

	// motion execution
	if (pause && pars_spec[PARAM_P]) {
		//utensil->pause();
		router->pause();
		delay(params[PARAM_P] * 1000);
		//utensil->restart();
		router->restart();
	} else if (motion_command) {
		switch (last_word[GROUP1]) {
		case G0:
		case G1:
			motionG0G1();
			break;
		case G2:
		case G3:
			motionG2G3();
			break;
		case G81:
			cycleG81();
			break;
		};

	}

	returnStatus();
	return parser_status;
}

void GCode::removeSpaces() {
	int len = line.length();
	int i = 0;
	while (i < len) {
		if (line[i] == ' ' || line[i] == '\n' || line[i] == '\r') {
			line.remove(i, 1);
			len--;
		} else
			i++;
	}
}
