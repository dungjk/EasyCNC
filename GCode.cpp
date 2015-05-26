/*!
 \file      GCode.cpp
 \brief     G-Code parser.
 \details   The class allows to parse the G-Code lines and runs the operation to actuate the command.
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#include "GCode.h"

extern CNC_Router_ISR *_crt;

GCode *_gc = NULL; //!< It is used to bind the timer5 handler and the GCode::returnStatus of the last instanced GCode object

/*! \def INIT_TIMER5
 *  \details Below the description of the operations performed by the macro:
 *           - TCCR5A = 0;  set entire TCCR5A register to 0
 *           - TCCR5B = 0;  set entire TCCR5B register to 0
 *           - TCNT5 = 0;   initialize counter value to B0
 *			 - TCCR5B |= (1 << WGM52); turn on CTC mode
 */
#define INIT_TIMER5 {	TCCR5A = 0;    \
	             	 	TCCR5B = 0;    \
	             	 	TCNT5 = 0;   \
	             	 	TIMSK5 |= (1 << OCIE5A);    \
	             	 	TCCR5B |= (1 << WGM52); }

/*! \def SET_TIMER5
 *  \details It sets the value od the Output Compare Register A of the timer 3.
 *  \param x The value of the register
 */
#define SET_TIMER5(x) { OCR5A = x;}

/*! \def CLEAR_COUNTER_TIMER5
 *  \details Set to 0 the TCNT5 register.
 */
#define CLEAR_COUNTER_TIMER5 { TCNT5 = 0; }

/*! \def START_TIMER5
 *  \details The macro runs the timer 3 with a 1024 prescaler.
 *           - TCNT5 = 0; clear the TCNT5 register
 *           - TCCR5B |= (1 << CS50) | (1 << CS52); set 1024 prescaler
 */
#define START_TIMER5 {  TCNT5 = 0;  \
						TCCR5B |= (1 << CS50) | (1 << CS52);}
/*! \def STOP_TIMER5
 *  \details The macro stops the timer 3.
 */
#define STOP_TIMER5 {   TCCR5B &= ~((1 << CS50) | (1 << CS52));}

GCode::GCode(CNC_Router_ISR *rt, Utensil *ml) :
		parser_status(STATUS_OK), spindle_speed(0.0), feed_rate(0.0), router(rt) {
#ifdef _MILLING_MACHINE
	utensil = static_cast<MillingMachine*> (ml);
#endif

#ifdef _LASER
	utensil = static_cast<Laser*>(ml);
#endif

#ifdef _PLOTTER_SERVO
	utensil = static_cast<PlotterServo*>(ml);
#endif

	line = "";
	sync = true;
	memset(last_word, UNSPECIFIED, 16);
	_gc = this;
}

void GCode::init() {
	line.reserve(256);   //It reserves 256 bytes
	uint8_t oldSREG = SREG;
	//disable global interrupt
	cli();
	//disable global interrupt
	INIT_TIMER5
	;
	SET_TIMER5(STATUS_FEEDBACK);
	START_TIMER5
	;
	SREG = oldSREG;
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
/*
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
 }*/

void GCode::sendAck() {
	if (sync) {
		cli();
		sync = false;
		sei();
		Serial.print("#");
		Serial.print(parser_status);
		Serial.print(":");
		Serial.println(router->buffInfo());
		cli();
		sync = true;
		sei();
	}
}

void GCode::returnStatus() {
	//Interrupts disabled
	if (sync) {
		sync = false;
	} else {
		return;
	}
	PositionXYZ tmp = router->getPos();
	float fr = router->getCurrFR();
	if (_crt->ls_x_down || _crt->ls_x_up || _crt->ls_y_down || _crt->ls_y_up
			|| _crt->ls_z_down || _crt->ls_z_up) {
		parser_status = STATUS_LIMITI_SWITCH_TRG;
	}
	sei();
	//Imterrupt enabled
	Serial.print("$");
	Serial.print(tmp.X());
	Serial.print(":");
	Serial.print(tmp.Y());
	Serial.print(":");
	Serial.print(tmp.Z());
	Serial.print(":");
	Serial.print(fr * 60);
	Serial.print(":");
	Serial.print(parser_status);
	Serial.print(":");
	Serial.println(router->buffInfo());
	cli();
	sync = true;
	sei();
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

	parser_status = STATUS_WORKING;

	int loops = (pars_spec[PARAM_L]) ? params[PARAM_L] : 1;
	PositionXYZ old_p = router->getProcessed();

	for (int l = 0; l < loops; l++) {
		//Step 1: rapid motion to XY
		router->moveToXY(new_pos);
		//Step 2: rapid motion to Z specified by R parameter (clear position)
		if (l == 0) {
			if (last_word[GROUP3] == G90) {
				router->moveTo(router->getProcessed().Z(params[PARAM_R]));
			} else {
				router->moveTo(PositionXYZ(0.0, 0.0, params[PARAM_R]));
			}
		} else {
			if (last_word[GROUP10] == G98 && old_p.Z() > params[PARAM_R]) {
				router->moveTo(PositionXYZ(0.0, 0.0, params[PARAM_R]));
			}
		}

		//Step 3: motion on Z-axis to the value Z specified in the cycle command
		if (last_word[GROUP3] == G90) {
			router->moveTo(new_pos, feed_rate);
		} else {
			router->moveTo(PositionXYZ(0.0, 0.0, new_pos.Z()), feed_rate);
		}

		//Step 4: return to the clear position specified by R with a rapid motion
		if (last_word[GROUP10] == G98 && old_p.Z() > params[PARAM_R]) {
			if (last_word[GROUP3] == G90) {
				router->moveTo(router->getProcessed().Z(old_p.Z()));
			} else {
				router->moveTo(
						PositionXYZ(0.0, 0.0, -new_pos.Z() - params[PARAM_R]));
			}
		} else {
			if (last_word[GROUP3] == G90) {
				router->moveTo(router->getProcessed().Z(params[PARAM_R]));
			} else {
				router->moveTo(PositionXYZ(0.0, 0.0, -new_pos.Z()));
			}
		}
		parser_status = STATUS_OK;
	}
}

void GCode::motionG2G3() {
	PositionXYZ center;
	PositionXYZ start_p = router->getProcessed();
	float alpha, angle_next_p, angle_end, r;
	boolean can_move = false;

	if ((pars_spec[PARAM_X] || pars_spec[PARAM_Y]) && pars_spec[PARAM_R]) { //Computation of arch from radius

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

		center += start_p; // the center of the cyrcle

	} else if ((pars_spec[PARAM_X] || pars_spec[PARAM_Y])
			&& (pars_spec[PARAM_I] || pars_spec[PARAM_J])) { //Computation of arch from offset of the center

		can_move = true;
		PositionXYZ offset(params[PARAM_I], params[PARAM_J], 0.0);
		center = start_p;
		r = offset.module();
		center += offset;  // the center of the cyrcle

	} else {
		parser_status = STATUS_SYNTAX_ERROR;
		return;
	}

	if (can_move) {
		//Computation of the angle for each step of the arch
		alpha = 2 * asin(ARCH_DEFINITION / (2 * r));
		//Computation of the angle from start_p to new_pos_xy
		angle_end = center.angleXY(new_pos);
		angle_next_p = center.angleXY(start_p);

		float delta_z_offset = router->getProcessed().offsetZ(new_pos)
				* alpha/ abs(angle_end - angle_next_p); // delta forward on Z-axis for each segment of the circle.
		float i_seg = 1.0;

		PositionXYZ tmp;

		if (last_word[GROUP1] == G3) {
			if (angle_end < angle_next_p) // In the case of the atan2 function returns a angle_end value smaller then the start angle.
				angle_end += 2 * PI;

			angle_next_p += alpha;

			while (angle_next_p < angle_end) {
				router->moveTo(
						center
								+ tmp.polarXY(r, angle_next_p).Z(
										delta_z_offset * i_seg), feed_rate);
				angle_next_p += alpha;
				i_seg += 1.0;
			}
			router->moveTo(new_pos, feed_rate);
		} else {
			if (angle_end > angle_next_p) // In the case of the atan2 function returns a angle_end value greater then the start angle.
				angle_end -= 2 * PI;

			angle_next_p -= alpha;
			while (angle_next_p > angle_end) {
				router->moveTo(
						center
								+ tmp.polarXY(r, angle_next_p).Z(
										delta_z_offset * i_seg), feed_rate);
				angle_next_p -= alpha;
				i_seg += 1.0;
			}
			router->moveTo(new_pos, feed_rate);
		}
	}
}

void GCode::motionG0G1() {

	if (pars_spec[PARAM_X] || pars_spec[PARAM_Y] || pars_spec[PARAM_Z]) {
		if (last_word[GROUP1] == G0) {
			router->moveTo(new_pos);
		} else {
			router->moveTo(new_pos, feed_rate);
		}
	} else {
		parser_status = STATUS_SYNTAX_ERROR;
	}
}

boolean GCode::getWord(char &code, float &val, uint8_t &pos) {
	uint8_t len = line.length();

	if (pos == len) {
		return false;
	}
	if (line[pos] < 'A' || line[pos] > 'Z') {
		parser_status = STATUS_BAD_WORD;
		return false;
	}
	code = line[pos];
	pos++;

	if (getFloat(pos, val, line)) {

		parser_status = STATUS_BAD_WORD;
		return false;
	}
	return true;
}

/*boolean GCode::getControlComm(char &code, float &val) {
 uint8_t len = line.length();
 uint8_t pos = 1;

 if (pos == len) {
 return false;
 }
 if (line[pos] < 'a' || line[pos] > 'z') {
 return false;
 }
 code = line[pos];
 pos++;

 if (pos == len) {
 val = 0;
 return true;
 }


 if (getFloat(pos, val)) {
 return false;
 }
 return true;
 }*/

void GCode::waitMotionFinish() {
	while (!(router->m_planner.isEmpty() && router->m_performer.isNotWorking())) {
		if (Serial.available() > 0) {
			new_line[Serial.readBytesUntil('\n', new_line, 256)] = '\0';
			String line = new_line;
			if (line[0] == '$') {
				char c;
				float v;
				if (getControlComm(c, v, line)) {
					switch (c) {

					case 's':
						router->stop();
						router->start();
					};
				}
			}
		}
		delay(100); //It waits for a correct insertion into the motion planner
	}
}

int GCode::parseLine() {
	removeSpaces(line);
//Special commands that starts with "$"
	if (line[0] == '$') {
		char c;
		float v;
		if (getControlComm(c, v, line)) {
			switch (c) {
			case 'r':       // errors reset
				resetStatus();
				break;
			case 'p':       // position reset
				router->resetPos();
				break;
			case 'h':		// search the home position
				if (router->searchHomePos())
					parser_status = STATUS_OP_ERROR;
				break;
			case 'z':		// search the z=0
				if (router->searchZ0Pos())
					parser_status = STATUS_OP_ERROR;
				break;
			case 't':// tool change, it moves up the utensil to allow the tool change
				router->moveTo(
						router->getPos()
								+ PositionXYZ(0, 0, TOOL_CHANGE_HEIGHT));
				break;
			case 's':
				router->stop();
				router->start();
				break;
			};

		}
		return parser_status;
	}

	if (parser_status != STATUS_OK) {
		return parser_status;
	}

//G-Code commands
	if (last_word[GROUP3] == G91) {
		new_pos = PositionXYZ();
	} else {
		new_pos = router->getProcessed();
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

	uint8_t ptr = 0;
	char type;
	float val;
// Line parsing
	while (getWord(type, val, ptr)) {
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
			case 80:
				motion_command = false;
				last_word[GROUP1] = G80;
				break;
			case 81:
				motion_command = true;
				last_word[GROUP1] = G81;
				break;
			case 90:
				// Absolute distance mode
				last_word[GROUP3] = G90;
				router->setAbsolPos();
				break;
			case 91:
				// Incremental distance mode
				last_word[GROUP3] = G91;
				router->setIncrPos();
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
				parser_status = STATUS_UNSUPPORTED;
			}
			;
			break;
		case 'M':
			switch ((int) val) {
			case 3:
				// switch on the spindle in CW
#ifdef _MILLING_MACHINE
				utensil->setSpindleDir(true);
				waitMotionFinish();
				utensil->enable();
#endif
#ifdef _LASER
				waitMotionFinish();
				utensil->switchOn();
#endif
#ifdef _PLOTTER_SERVO
				waitMotionFinish();
				utensil->down();
#endif
				break;
			case 4:
				// switch on the spindle in CCW
#ifdef _MILLING_MACHINE
				utensil->setSpindleDir(false);
				waitMotionFinish();
				utensil->enable();
#endif
#ifdef _LASER
				waitMotionFinish();
				utensil->switchOn();
#endif
#ifdef _PLOTTER_SERVO
				waitMotionFinish();
				utensil->down();
#endif
				break;
			case 5:
#ifdef _MILLING_MACHINE
				// switch off the spindle
				// I have to wait for the ending of the last motion command
				waitMotionFinish();
				utensil->disable();
#endif

#ifdef _LASER
				// switch off the laser
				// I have to wait for the ending of the last motion command
				waitMotionFinish();
				utensil->SwitchOff();
#endif
#ifdef _PLOTTER_SERVO
				waitMotionFinish();
				utensil->up();
#endif
				break;
			case 6:
				// tool change (unsupported)
				parser_status = STATUS_TOOL_CHANGE;
				break;
			case 30:
				// end program
				break;
			}
			;
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
			spindle_speed = val;
#ifdef _MILLING_MACHINE
			utensil->setSpindleSpeed(spindle_speed);
#endif
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
		case 'T':
			//tool selection (unsupported)

			break;
		default:
			parser_status = STATUS_UNSUPPORTED;
			return parser_status;
		};

	}

// check status
	if (parser_status != STATUS_OK) {
		return parser_status;
	}

// motion execution
	if (pause && pars_spec[PARAM_P]) {
		router->pause();
		delay(params[PARAM_P] * 1000);
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

	return parser_status;
}
/*
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
 }*/

ISR(TIMER5_COMPA_vect) {
	if (_gc != NULL) {
		_gc->returnStatus();
	}
}

