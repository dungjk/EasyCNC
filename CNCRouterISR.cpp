/*!
 \file      CNCRouterISR.cpp
 \brief     Data structure that coordinates the motors motion
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */
#include "CNCRouterISR.h"

CNC_Router_ISR *_crt = NULL; //!< It is used to bind the interrupts due to the limits switches to the MotionPerformer::stopMotion function of the last instanced CNC_Router_ISR object

void CNC_Router_ISR::ls_x_down_routine() {
	if (_crt != NULL) {
		_crt->ls_x_down =
				(ROUTER_DOWN_LIMIT_SWITCH_X_ACTIVE_HIGH == 1) ?
						digitalRead(getPinFromInterrupt(
						ROUTER_DOWN_LIMIT_SWITCH_X_INTERRUPT)) :
						!digitalRead(getPinFromInterrupt(
						ROUTER_DOWN_LIMIT_SWITCH_X_INTERRUPT));
		if (_crt->ls_x_down) {
			_crt->stop();
			PositionXYZ tmp = _crt->processed_p;
			tmp.X(tmp.X() + 1.0);
			_crt->moveTo(tmp);
			_crt->start();
		} else {
			_crt->searchProc = false;
		}
	}
}

void CNC_Router_ISR::ls_x_up_routine() {
	if (_crt != NULL) {
		_crt->ls_x_up =
				(ROUTER_UP_LIMIT_SWITCH_X_ACTIVE_HIGH == 1) ?
						digitalRead(getPinFromInterrupt(
						ROUTER_UP_LIMIT_SWITCH_X_INTERRUPT)) :
						!digitalRead(getPinFromInterrupt(
						ROUTER_UP_LIMIT_SWITCH_X_INTERRUPT));
		if (!_crt->ls_x_up) {
			_crt->stop();
			PositionXYZ tmp = _crt->processed_p;
			tmp.X(tmp.X() - 1.0);
			_crt->moveTo(tmp);
			_crt->start();
		} else {
			_crt->searchProc = false;
		}
	}
}

void CNC_Router_ISR::ls_y_down_routine() {
	if (_crt != NULL) {
		_crt->ls_y_down =
				(ROUTER_DOWN_LIMIT_SWITCH_Y_ACTIVE_HIGH == 1) ?
						digitalRead(getPinFromInterrupt(
						ROUTER_DOWN_LIMIT_SWITCH_Y_INTERRUPT)) :
						!digitalRead(getPinFromInterrupt(
						ROUTER_DOWN_LIMIT_SWITCH_Y_INTERRUPT));
		if (!_crt->ls_y_down) {
			_crt->stop();
			PositionXYZ tmp = _crt->processed_p;
			tmp.Y(tmp.Y() + 1.0);
			_crt->moveTo(tmp);
			_crt->start();
		} else {
			_crt->searchProc = false;
		}
	}
}

void CNC_Router_ISR::ls_y_up_routine() {
	if (_crt != NULL) {
		_crt->ls_y_up =
				(ROUTER_UP_LIMIT_SWITCH_Y_ACTIVE_HIGH == 1) ?
						digitalRead(getPinFromInterrupt(
						ROUTER_UP_LIMIT_SWITCH_Y_INTERRUPT)) :
						!digitalRead(getPinFromInterrupt(
						ROUTER_UP_LIMIT_SWITCH_Y_INTERRUPT));
		if (!_crt->ls_y_up) {
			_crt->stop();
			PositionXYZ tmp = _crt->processed_p;
			tmp.Y(tmp.Y() - 1.0);
			_crt->moveTo(tmp);
			_crt->start();
		} else {
			_crt->searchProc = false;
		}
	}
}

void CNC_Router_ISR::ls_z_down_routine() {
	if (_crt != NULL) {
		_crt->ls_z_down =
				(ROUTER_DOWN_LIMIT_SWITCH_Z_ACTIVE_HIGH == 1) ?
						digitalRead(getPinFromInterrupt(
						ROUTER_DOWN_LIMIT_SWITCH_Z_INTERRUPT)) :
						!digitalRead(getPinFromInterrupt(
						ROUTER_DOWN_LIMIT_SWITCH_Z_INTERRUPT));
		if (!_crt->ls_z_down) {
			_crt->stop();
			PositionXYZ tmp = _crt->processed_p;
			tmp.Z(tmp.Z() + 0.1);
			_crt->moveTo(tmp);
			_crt->start();
		} else {
			_crt->searchProc = false;
		}
	}
}

void CNC_Router_ISR::ls_z_up_routine() {
	if (_crt != NULL) {
		_crt->ls_z_up =
				(ROUTER_UP_LIMIT_SWITCH_Z_ACTIVE_HIGH == 1) ?
						digitalRead(getPinFromInterrupt(
						ROUTER_UP_LIMIT_SWITCH_Z_INTERRUPT)) :
						!digitalRead(getPinFromInterrupt(
						ROUTER_UP_LIMIT_SWITCH_Z_INTERRUPT));
		if (!_crt->ls_z_up) {
			_crt->stop();
			PositionXYZ tmp = _crt->processed_p;
			tmp.Z(tmp.Z() - 1.0);
			_crt->moveTo(tmp);
			_crt->start();
		} else {
			_crt->searchProc = false;
		}
	}
}

CNC_Router_ISR::CNC_Router_ISR() :
		processed_p(), spmmx(ROUTER_MX_STEPS_PER_MM), spmmy(
		ROUTER_MY_STEPS_PER_MM), spmmz(ROUTER_MZ_STEPS_PER_MM), v_max_x(
		ROUTE_MX_MODE * ROUTER_MX_STEPS_PER_MM * ROUTER_MX_SPEED / 60), v_max_y(
		ROUTE_MY_MODE * ROUTER_MY_STEPS_PER_MM * ROUTER_MY_SPEED / 60), v_max_z(
		ROUTE_MZ_MODE * ROUTER_MZ_STEPS_PER_MM * ROUTER_MZ_SPEED / 60), pos_type(
		false), round_off_x(0.0), round_off_y(0.0), round_off_z(0.0), searchProc(
		false), m_planner(ROUTER_DRIVERS_COOLING), m_performer(&(this->m_planner)) {
	ls_x_down = ls_x_up = ls_y_down = ls_y_up = ls_z_down = ls_z_up = false;
	_crt = this;
}

void CNC_Router_ISR::initMotors() {

	m_performer.mx.setPins(ROUTE_MX_STEP_CONTROL_PIN,
	ROUTE_MX_DIRECTION_CONTROL_PIN, ROUTE_MX_ENABLE_CONTROL_PIN);
	m_performer.mx.control_mode = ROUTE_MX_MODE;
	m_performer.mx.orientation = ROUTER_MX_ORIENTATION;

	m_performer.my.setPins(ROUTE_MY_STEP_CONTROL_PIN,
	ROUTE_MY_DIRECTION_CONTROL_PIN, ROUTE_MY_ENABLE_CONTROL_PIN);
	m_performer.my.control_mode = ROUTE_MY_MODE;
	m_performer.my.orientation = ROUTER_MY_ORIENTATION;

	m_performer.mz.setPins(ROUTE_MZ_STEP_CONTROL_PIN,
	ROUTE_MZ_DIRECTION_CONTROL_PIN, ROUTE_MZ_ENABLE_CONTROL_PIN);
	m_performer.mz.control_mode = ROUTE_MZ_MODE;
	m_performer.mz.orientation = ROUTER_MZ_ORIENTATION;

}

void CNC_Router_ISR::initInterrupts() {
	if (ROUTER_DOWN_LIMIT_SWITCH_X_INTERRUPT > -1) {
		pinMode(getPinFromInterrupt(ROUTER_DOWN_LIMIT_SWITCH_X_INTERRUPT),
		INPUT_PULLUP);
		attachInterrupt(ROUTER_DOWN_LIMIT_SWITCH_X_INTERRUPT,
				CNC_Router_ISR::ls_x_down_routine, CHANGE);
	}

	if (ROUTER_DOWN_LIMIT_SWITCH_Y_INTERRUPT > -1) {
		pinMode(getPinFromInterrupt(ROUTER_DOWN_LIMIT_SWITCH_Y_INTERRUPT),
		INPUT_PULLUP);
		attachInterrupt(ROUTER_DOWN_LIMIT_SWITCH_Y_INTERRUPT,
				CNC_Router_ISR::ls_y_down_routine, CHANGE);
	}

	if (ROUTER_DOWN_LIMIT_SWITCH_Z_INTERRUPT > -1) {
		pinMode(getPinFromInterrupt(ROUTER_DOWN_LIMIT_SWITCH_Z_INTERRUPT),
		INPUT_PULLUP);
		attachInterrupt(ROUTER_DOWN_LIMIT_SWITCH_Z_INTERRUPT,
				CNC_Router_ISR::ls_z_down_routine, CHANGE);
	}

	if (ROUTER_UP_LIMIT_SWITCH_X_INTERRUPT > -1) {
		pinMode(getPinFromInterrupt(ROUTER_UP_LIMIT_SWITCH_X_INTERRUPT),
		INPUT_PULLUP);
		attachInterrupt(ROUTER_UP_LIMIT_SWITCH_X_INTERRUPT,
				CNC_Router_ISR::ls_x_up_routine, CHANGE);
	}

	if (ROUTER_UP_LIMIT_SWITCH_Y_INTERRUPT > -1) {
		pinMode(getPinFromInterrupt(ROUTER_UP_LIMIT_SWITCH_Y_INTERRUPT),
		INPUT_PULLUP);
		attachInterrupt(ROUTER_UP_LIMIT_SWITCH_Y_INTERRUPT,
				CNC_Router_ISR::ls_y_up_routine, CHANGE);
	}

	if (ROUTER_UP_LIMIT_SWITCH_Z_INTERRUPT > -1) {
		pinMode(getPinFromInterrupt(ROUTER_UP_LIMIT_SWITCH_Z_INTERRUPT),
		INPUT_PULLUP);
		attachInterrupt(ROUTER_UP_LIMIT_SWITCH_Z_INTERRUPT,
				CNC_Router_ISR::ls_z_up_routine, CHANGE);
	}
}

void CNC_Router_ISR::initMotionPerformer() {
//NOTE: if it does not work, try with a MotionPerformer pointer.
	m_performer.init();
}

void CNC_Router_ISR::resetPos() {
//TODO : review this function later
	if (m_performer.isNotWorking()) {
		processed_p = PositionXYZ(0.0, 0.0, 0.0);
		m_performer.mx.global_steps = m_performer.my.global_steps =
				m_performer.mz.global_steps = 0;
	}
}

void CNC_Router_ISR::moveToXY(float px, float py, float spd) {

	float dx, dy, dist, spd_x, spd_y, actual_spmmx, actual_spmmy;
	actual_spmmx = spmmx * m_performer.mx.control_mode;
	actual_spmmy = spmmy * m_performer.my.control_mode;

	PositionXYZ new_p;
	new_p.X(px).Y(py).Z(processed_p.Z());

	LM new_line_motion;

	if (pos_type) {
		dx = px;
		dy = py;
		dist = new_p.moduleXY();
	} else {
		dx = processed_p.offsetX(new_p);
		dy = processed_p.offsetY(new_p);
		dist = processed_p.moduleXY(new_p);
	}

	float steps_x = dx * actual_spmmx + round_off_x; // (mm * steps/mm + num = steps); pay attention that here the number of steps is a float value
	float steps_y = dy * actual_spmmy + round_off_y;

	round_off_x = steps_x - (int32_t) steps_x; // it is a signed value; here I take the decimal part of the number
	round_off_y = steps_y - (int32_t) steps_y;

//Here we have computed the number of steps for each axis
	uint32_t tmp_stx, tmp_sty;
	tmp_stx = abs(steps_x);
	tmp_sty = abs(steps_y);
	new_line_motion.steps_x = steps_x; //This value could be smaller than 0 so that it can specify the direction to the motor driver
	new_line_motion.steps_y = steps_y;
	new_line_motion.steps_z = 0;
	new_line_motion.act_fr = spd;
	if (tmp_stx == 0 && tmp_sty == 0) {
		//This check is important to avoid insidious condition where more than one LM object with x, y and z steps are 0.
		processed_p = new_p;
		return;
	}

	float ratio = spd / dist;          // (mm/s)/mm = 1/s
	if (tmp_stx > tmp_sty) {
		spd_x = tmp_stx * ratio;   // steps/s
		if (spd_x > v_max_x || spd_x <= 0.0) {
			spd_x = v_max_x;   // steps/s
			new_line_motion.act_fr = dist * v_max_x / tmp_stx; // (mm * steps/s / steps = mm/s)
		}
		new_line_motion.delay = F_CPU / (spd_x * 1024.0) - 1.0; //F_CPU=16000000; The value of the OCR3A reg. it must be smaller than 65534

	} else {
		spd_y = tmp_sty * ratio;   // steps/s
		if (spd_y > v_max_y || spd_y <= 0.0) {
			spd_y = v_max_y;   // steps/s
			new_line_motion.act_fr = dist * v_max_y / tmp_sty; // (mm * steps/s / steps = mm/s)
		}
		new_line_motion.delay = F_CPU / (spd_y * 1024.0) - 1.0; //F_CPU=16000000; The value of the OCR3A reg. it must be smaller than 65534
	}
	while (m_planner.addMotion(new_line_motion))
		delay(100);
//It waits for a correct insertion into the motion planner
	processed_p = new_p;
}

void CNC_Router_ISR::moveTo(float px, float py, float pz, float spd) {
	float dx, dy, dz, dist, spd_x, spd_y, spd_z, actual_spmmx, actual_spmmy,
			actual_spmmz;
	actual_spmmx = spmmx * m_performer.mx.control_mode;
	actual_spmmy = spmmy * m_performer.my.control_mode;
	actual_spmmz = spmmz * m_performer.mz.control_mode;

	PositionXYZ new_p;
	new_p.X(px).Y(py).Z(pz);
	LM new_line_motion;

	if (pos_type) {
		dx = px;
		dy = py;
		dz = pz;
		dist = new_p.module();
	} else {
		dx = processed_p.offsetX(new_p);
		dy = processed_p.offsetY(new_p);
		dz = processed_p.offsetZ(new_p);
		dist = processed_p.module(new_p);
	}

	float steps_x = dx * actual_spmmx + round_off_x; // (mm * steps/mm * num = steps); pay attention that here the number of steps is a float value
	float steps_y = dy * actual_spmmy + round_off_y;
	float steps_z = dz * actual_spmmz + round_off_z;

	round_off_x = steps_x - (int32_t) steps_x; // it is a signed value; here I take the decimal part of the number
	round_off_y = steps_y - (int32_t) steps_y;
	round_off_z = steps_z - (int32_t) steps_z;

//Here we have computed the number of steps for each axis
	uint32_t tmp_stx, tmp_sty, tmp_stz;
	tmp_stx = abs(steps_x);
	tmp_sty = abs(steps_y);
	tmp_stz = abs(steps_z);

	if (tmp_stx == 0 && tmp_sty == 0 && tmp_stz == 0) {
		//This check is important to avoid insidious condition where more than one LM object with x, y and z steps are 0.
		processed_p = new_p;
		return;
	}

	new_line_motion.steps_x = steps_x; //This value could be smaller than 0 so that it can specify the direction to the motor driver
	new_line_motion.steps_y = steps_y;
	new_line_motion.steps_z = steps_z;

	new_line_motion.act_fr = spd;

	float ratio = spd / dist;          // (mm/s)/mm = 1/s
	if (tmp_stx >= tmp_sty && tmp_stx >= tmp_stz) {
		//X > Y && X > Z
		spd_x = (float) tmp_stx * ratio;   // steps/s

		if (spd_x > v_max_x || spd_x <= 0.0) {
			spd_x = v_max_x;   // steps/s
			new_line_motion.act_fr = dist * v_max_x / tmp_stx; // (mm * steps/s / steps = mm/s)
		}

		new_line_motion.delay = F_CPU / (spd_x * 1024.0) - 1.0; //F_CPU = 16000000; Range: 64uS; 8388.608mS; The value of the OCR3A reg. it must be smaller than 65534
		//new_line_motion.delay = F_CPU / (spd_x * 256.0) - 1.0; //F_CPU = 16000000; Range: 16uS; 2097.152mS The value of the OCR3A reg. it must be smaller than 65534

	} else if (tmp_sty >= tmp_stx && tmp_sty >= tmp_stz) {
		//Y > X && Y > Z
		spd_y = (float) tmp_sty * ratio;   // steps/s

		if (spd_y > v_max_y || spd_y <= 0.0) {
			spd_y = v_max_y;   // steps/s
			new_line_motion.act_fr = dist * v_max_y / tmp_sty; // (mm * steps/s / steps = mm/s)
		}

		new_line_motion.delay = F_CPU / (spd_y * 1024.0) - 1.0; //F_CPU = 16000000; The value of the OCR3A reg. it must be smaller than 65534
		//new_line_motion.delay = F_CPU / (spd_y * 256.0) - 1.0; //F_CPU = 16000000; The value of the OCR3A reg. it must be smaller than 65534

	} else {
		//Z > X && Z > Y
		spd_z = (float) tmp_stz * ratio;   // steps/s

		if (spd_z > v_max_z || spd_z <= 0.0) {
			spd_z = v_max_z;   // steps/s
			new_line_motion.act_fr = dist * v_max_z / tmp_stz; // (mm * steps/s / steps = mm/s)
		}
		new_line_motion.delay = F_CPU / (spd_z * 1024.0) - 1.0; //F_CPU = 16000000; The value of the OCR3A reg. it must be smaller than 65534
		//new_line_motion.delay = F_CPU / (spd_z * 256.0) - 1.0; //F_CPU = 16000000; The value of the OCR3A reg. it must be smaller than 65534
	}

	while (m_planner.addMotion(new_line_motion)) {
		if (Serial.available() > 0) {
			new_line[Serial.readBytesUntil('\n', new_line, 256)] = '\0';
			String line = new_line;
			if (line[0] == '$') {
				char c;
				float v;
				if (getControlComm(c, v, line)) {
					switch (c) {

					case 's':
						m_performer.stopMotion();
						m_planner.clear();
						processed_p = getPos();
						m_performer.startMotion();
						return;
					};
				}
			}
		}
		delay(100); //It waits for a correct insertion into the motion planner
	}

	processed_p = new_p;
}

void CNC_Router_ISR::moveToXY(const PositionXYZ &np, float spd) {
	moveToXY(np.X(), np.Y(), spd);
}

void CNC_Router_ISR::moveTo(const PositionXYZ &np, float spd) {
	moveTo(np.X(), np.Y(), np.Z(), spd);
}

void CNC_Router_ISR::stop() {
	m_performer.stopMotion();
	m_planner.clear();
	processed_p = getPos();
}

void CNC_Router_ISR::start() {
	m_performer.startMotion();
}

void CNC_Router_ISR::pause() {
//int32_t stpx, stpy, stpz;
	m_performer.pauseMotion();
}

void CNC_Router_ISR::restart() {
	m_performer.restartMotion();
}

void CNC_Router_ISR::setIncrPos() {
	pos_type = true;
}

void CNC_Router_ISR::setAbsolPos() {
	pos_type = false;
}

boolean CNC_Router_ISR::searchHomePos() {
	if (ROUTER_DOWN_LIMIT_SWITCH_X_INTERRUPT > -1
			&& ROUTER_DOWN_LIMIT_SWITCH_Y_INTERRUPT > -1
			&& m_planner.isEmpty()) {
		PositionXYZ tmp = getPos();
		tmp.X(tmp.X() - 500.0);
		moveTo(tmp);
		searchProc = true;
		int count_sec = 15;
		while (searchProc && count_sec > 0) {
			count_sec--;
			delay(1000);
		}

		if (searchProc) {
			searchProc = false;
			return true;
		}

		delay(1000);

		tmp = getPos();
		tmp.Y(tmp.Y() - 500.0);
		moveTo(tmp);
		searchProc = true;
		count_sec = 15;
		while (searchProc && count_sec > 0) {
			count_sec--;
			delay(1000);
		}

		if (searchProc) {
			searchProc = false;
			return true;
		}
		delay(1000);
		return false;
	}
	return true;
}

boolean CNC_Router_ISR::searchZ0Pos() {
	if (ROUTER_DOWN_LIMIT_SWITCH_Z_INTERRUPT > -1 && m_planner.isEmpty()) {
		PositionXYZ tmp = getPos();
		tmp.Z(tmp.Z() - 100.0);
		moveTo(tmp);
		searchProc = true;
		int count_sec = 15;
		while (searchProc && count_sec > 0) {
			count_sec--;
			delay(1000);
		}
		if (searchProc) {
			searchProc = false;
			return true;
		}
		delay(1000);
		return false;
	}
	return true;
}

PositionXYZ CNC_Router_ISR::getPos() {
	PositionXYZ tmp(
			m_performer.mx.global_steps / (spmmx * m_performer.mx.control_mode),
			m_performer.my.global_steps / (spmmy * m_performer.my.control_mode),
			m_performer.mz.global_steps
					/ (spmmz * m_performer.mz.control_mode));
	return tmp;
}

PositionXYZ CNC_Router_ISR::getProcessed() {
	return processed_p;
}

void CNC_Router_ISR::setPos(PositionXYZ p) {
	if (m_performer.isNotWorking()) {
		processed_p = p;
		m_performer.mx.global_steps = p.X() * spmmx
				* m_performer.mx.control_mode;
		m_performer.my.global_steps = p.Y() * spmmy
				* m_performer.my.control_mode;
		m_performer.mz.global_steps = p.Z() * spmmz
				* m_performer.mz.control_mode;
	}
}

void CNC_Router_ISR::orientationX(int8_t v) {
	if (v == 1 || v == -1) {
		m_performer.mx.orientation = v;
	}
}

void CNC_Router_ISR::orientationY(int8_t v) {
	if (v == 1 || v == -1) {
		m_performer.my.orientation = v;
	}
}

void CNC_Router_ISR::orientationZ(int8_t v) {
	if (v == 1 || v == -1) {
		m_performer.mz.orientation = v;
	}
}

float CNC_Router_ISR::getCurrFR() {
	return m_performer.current_fr;
}

int CNC_Router_ISR::buffInfo() {
	return m_planner.getFreeBuffSize();
}

