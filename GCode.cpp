/*
 * GCode.cpp
 *
 *  Created on: 26/lug/2014
 *      Author: francescogiurlanda
 */

#include "GCode.h"

GCode::GCode(CNC_Router *rt, MillingMachine *ml) {
	moving = plane_select = distanse_mode = forward_mode = unit =
			utensil_offset = loop_return = coordinate = path_control =
					parser_status;
	drill_speed = feed_rate = 0.0;
	new_pos_z = 0.0;
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

boolean GCode::getWord(char &code, float &val, uint8_t &pos, uint8_t len) {
	if (pos == len) {
		parser_status = STATUS_OK;
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
	//TODO add the support to limit switch
	boolean a, b;
	a = utensil->update();
	b = router->update();
	while (!a || !b) {
		a = utensil->update();
		b = router->update();
	}
}

int GCode::parseLine() {
	removeSpaces();
	DBGNL(line);
	DBGNL(line.length());

	new_pos_xy = router->getPos();
	new_pos_z = utensil->getPos();
	boolean motion_command = false;
	boolean pause = false;
	for (int i = 0; i < PARAMS; i++) {
		params[i] = 0.0;
		pars_spec[i] = false;
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
				// plane XZ unsupported
				last_word[GROUP2] = G18;
				break;
			case 19:
				// plane YZ unsupported
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
			case 54:
				//Coordinate job 1
				last_word[GROUP12] = G54;
				break;
			default:
				DBG("Unsupported command ");
				DBG(type);
				DBGNL(val);
			}
			;
			break;
		case 'M':
			DBG(type);
			DBGNL((int )trunc(val));
			break;
		case 'F':
			//the F's value is in units/minute, instead the motion control needs a feed rate in units/s
			feed_rate = val/60.0;
			break;
		case 'R':
			motion_command = pars_spec[PARAM_R] = true;
			params[PARAM_R] = val;
			break;
		case 'S':
			drill_speed = val;
			break;
		case 'X':
			new_pos_xy.X(val);
			motion_command = pars_spec[PARAM_X] = true;
			break;
		case 'Y':
			new_pos_xy.Y(val);
			motion_command = pars_spec[PARAM_Y] = true;
			break;
		case 'Z':
			new_pos_z = val;
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
		case 'P':
			params[PARAM_P] = val;
			pars_spec[PARAM_P] = true;
			break;

		};

	}

	// check status

	// motion execution
	if (pause && pars_spec[PARAM_P]) {
		utensil->pause();
		router->pause();
		delay(params[PARAM_P] * 1000);
		utensil->restart();
		router->restart();
	} else if (motion_command) {
		switch (last_word[GROUP1]) {
		case G0:
			if (pars_spec[PARAM_X] || pars_spec[PARAM_Y]
					|| pars_spec[PARAM_Z]) {
				DBG("Rapid motion to X= ");
				DBG(new_pos_xy.X());
				DBG(", Y=");
				DBG(new_pos_xy.Y());
				DBG(", Z=");
				DBGNL(new_pos_z);
				utensil->moveTo(new_pos_z);
				router->moveTo(new_pos_xy);
				runMotion();
			} else {
				parser_status = STATUS_SYNTAX_ERROR;
				return parser_status;
			}
			break;
		case G1:
			if (pars_spec[PARAM_X] || pars_spec[PARAM_Y]
					|| pars_spec[PARAM_Z]) {
				DBG("Working motion to X= ");
				DBG(new_pos_xy.X());
				DBG(", Y=");
				DBG(new_pos_xy.Y());
				DBG(", Z=");
				DBG(new_pos_z);
				DBG(", F=");
				DBGNL(feed_rate);
				utensil->moveTo(new_pos_z, feed_rate);
				router->moveTo(new_pos_xy, feed_rate);
				runMotion();
			} else {
				parser_status = STATUS_SYNTAX_ERROR;
				return parser_status;
			}
			break;
		case G2:
		case G3:
			PositionXY center;
			PositionXY start_p = router->getPos();
			float alpha, angle_next_p, angle_end, r;
			boolean can_move = false;

			if ((pars_spec[PARAM_X] || pars_spec[PARAM_Y])
					&& pars_spec[PARAM_R]) {

				can_move = true;
				float beta = start_p.angle(new_pos_xy);
				float dist = start_p.module(new_pos_xy);
				r = params[PARAM_R];
				float gamma = acos(dist / (2 * r));

				DBG("Arch with radius X=");
				DBG(new_pos_xy.X());
				DBG(", Y=");
				DBG(new_pos_xy.Y());
				DBG(", R=");
				DBG(r);
				DBG(", F=");
				DBGNL(feed_rate);

				//it depends on the sign of the radius value
				if (last_word[GROUP1] == G2) {
					center.polar(r, (r > 0) ? beta + gamma : beta - gamma);
				} else {
					center.polar(r, (r > 0) ? beta - gamma : beta + gamma);
				}

				center += start_p;  // the center of the cyrcle

			} else if ((pars_spec[PARAM_X] || pars_spec[PARAM_Y])
					&& (pars_spec[PARAM_I] || pars_spec[PARAM_J])) {
				can_move = true;
				PositionXY offset(params[PARAM_I], params[PARAM_J]);
				center = start_p;
				r = offset.module();
				center += offset;  // the center of the cyrcle
				DBG("Arch with center X=");
				DBG(new_pos_xy.X());
				DBG(", Y=");
				DBG(new_pos_xy.Y());
				DBG(", I=");
				DBG(offset.X());
				DBG(", J=");
				DBG(offset.Y());
				DBG(", F=");
				DBGNL(feed_rate);
			}

			if (can_move) {
				//Computation of the angle for each step of the arch
				alpha = 2 * asin(ARCH_DEFINITION / (2 * r));
				//Computation of the angle from start_p to new_pos_xy
				angle_end = center.angle(new_pos_xy);
				angle_next_p = center.angle(start_p);

				PositionXY tmp;

				if (last_word[GROUP1] == G2) {
					if (angle_end < angle_next_p) // In the case of the atan2 function returns a angle_end value smaller then the start angle.
						angle_end += 2 * PI;

					angle_next_p += alpha;
					while (angle_next_p < angle_end) {
						router->moveTo(center + tmp.polar(r, angle_next_p),
								feed_rate);
						runMotion();
						angle_next_p += alpha;
					}
					router->moveTo(new_pos_xy, feed_rate);
					runMotion();
				} else {
					if (angle_end > angle_next_p) // In the case of the atan2 function returns a angle_end value greater then the start angle.
						angle_end += 2 * PI;

					angle_next_p -= alpha;
					while (angle_next_p > angle_end) {
						router->moveTo(center + tmp.polar(r, angle_next_p),
								feed_rate);
						runMotion();
						angle_next_p -= alpha;
					}
					router->moveTo(new_pos_xy, feed_rate);
					runMotion();
				}
			}
			break;
		};

	}

	DBG("CG status: ");
	DBGNL(parser_status);
	parser_status = STATUS_OK;
	return 0;
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
