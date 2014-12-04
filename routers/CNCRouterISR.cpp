/*
 * CNCRouterISR.cpp
 *
 *  Created on: 03/dic/2014
 *      Author: francescogiurlanda
 */

#include "CNCRouterISR.h"

volatile boolean CNC_Router_ISR::ls_x_down = true;
volatile boolean CNC_Router_ISR::ls_y_down = true;
volatile boolean CNC_Router_ISR::ls_z_down = true;
volatile boolean CNC_Router_ISR::ls_x_up = true;
volatile boolean CNC_Router_ISR::ls_y_up = true;
volatile boolean CNC_Router_ISR::ls_z_up = true;

void CNC_Router_ISR::ls_x_down_routine() {
	CNC_Router_ISR::ls_x_down = digitalRead(
			getPinFromInterrupt(ROUTER_DOWN_LIMIT_SWITCH_X_INTERRUPT));
}

void CNC_Router_ISR::ls_x_up_routine() {
	CNC_Router_ISR::ls_x_up = digitalRead(
			getPinFromInterrupt(ROUTER_UP_LIMIT_SWITCH_X_INTERRUPT));
}

void CNC_Router_ISR::ls_y_down_routine() {
	CNC_Router_ISR::ls_y_down = digitalRead(
			getPinFromInterrupt(ROUTER_DOWN_LIMIT_SWITCH_Y_INTERRUPT));
}

void CNC_Router_ISR::ls_y_up_routine() {
	CNC_Router_ISR::ls_y_up = digitalRead(
			getPinFromInterrupt(ROUTER_UP_LIMIT_SWITCH_Y_INTERRUPT));
}

void CNC_Router_ISR::ls_z_down_routine() {
	CNC_Router_ISR::ls_z_down = digitalRead(
			getPinFromInterrupt(ROUTER_DOWN_LIMIT_SWITCH_Z_INTERRUPT));
}

void CNC_Router_ISR::ls_z_up_routine() {
	CNC_Router_ISR::ls_z_up = digitalRead(
			getPinFromInterrupt(ROUTER_UP_LIMIT_SWITCH_Z_INTERRUPT));
}

CNC_Router_ISR::CNC_Router_ISR() :
		processed_p(), actual_p(), spmmx(0.0), spmmy(0.0), spmmz(0.0), v_max_x(
				0.0), v_max_y(0.0), v_max_z(0.0), pos_type(false), round_off_x(
				0.0), round_off_y(0.0), round_off_z(0.0), m_planner(), m_performer(
				&(this->m_planner)) {
}
CNC_Router_ISR::CNC_Router_ISR(float spx, float spy, float spz, float vmx,
		float vmy, float vmz) :
		processed_p(), actual_p(), spmmx(spx), spmmy(spy), spmmz(spz), v_max_x(
				vmx), v_max_y(vmy), v_max_z(vmz), pos_type(false), round_off_x(
				0.0), round_off_y(0.0), round_off_z(0.0), m_planner(), m_performer(
				&(this->m_planner)) {
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

void CNC_Router_ISR::resetPos() {
	//TODO : review this function later
	actual_p = PositionXYZ(0.0, 0.0, 0.0);
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

	float steps_x = dx * actual_spmmx + round_off_x; // (mm * steps/mm * num = steps); pay attention that here the number of steps is a float value
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

	float ratio = spd / dist;          // (mm/s)/mm = 1/s
	if (tmp_stx > tmp_sty) {
		spd_x = tmp_stx * ratio;   // steps/s

		if (spd_x > v_max_x) {
			spd_x = v_max_x;   // steps/s
		}
		new_line_motion.delay = 16000000.0 / (spd_x * 1024.0) - 1.0; //The value of the OCR3A reg. it must be smaller than 65534

	} else {
		spd_y = tmp_sty * ratio;   // steps/s

		if (spd_y > v_max_y) {
			spd_y = v_max_y;   // steps/s
		}
		new_line_motion.delay = 16000000.0 / (spd_y * 1024.0) - 1.0; //The value of the OCR3A reg. it must be smaller than 65534
	}

	m_planner.addMotion(new_line_motion);
	processed_p = new_p;
	//TODO: activate the motion performer
}

void CNC_Router_ISR::moveTo(float px, float py, float pz, float spd) {
	float dx, dy, dz, dist, spd_x, spd_y, spd_z, actual_spmmx, actual_spmmy,
			actual_spmmz;
	actual_spmmx = spmmx * m_performer.mx.control_mode;
	actual_spmmy = spmmy * m_performer.my.control_mode;
	actual_spmmz = spmmz * m_performer.mz.control_mode;

	PositionXYZ new_p;
	new_p.X(px).Y(py).Z(processed_p.Z());

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
	new_line_motion.steps_x = steps_x; //This value could be smaller than 0 so that it can specify the direction to the motor driver
	new_line_motion.steps_y = steps_y;
	new_line_motion.steps_z = steps_z;

	float ratio = spd / dist;          // (mm/s)/mm = 1/s
	if (tmp_stx > tmp_sty && tmp_stx > tmp_stz) {
		//X > Y && X > Z
		spd_x = tmp_stx * ratio;   // steps/s

		if (spd_x > v_max_x) {
			spd_x = v_max_x;   // steps/s
		}
		new_line_motion.delay = 16000000.0 / (spd_x * 1024.0) - 1.0; //The value of the OCR3A reg. it must be smaller than 65534
	} else if (tmp_sty > tmp_stx && tmp_sty > tmp_stz) {
		//Y > X && Y > Z
		spd_y = tmp_sty * ratio;   // steps/s

		if (spd_y > v_max_y) {
			spd_y = v_max_y;   // steps/s
		}
		new_line_motion.delay = 16000000.0 / (spd_y * 1024.0) - 1.0; //The value of the OCR3A reg. it must be smaller than 65534
	} else {
		//Z > X && X > Y
		spd_z = tmp_stz * ratio;   // steps/s

		if (spd_z > v_max_z) {
			spd_z = v_max_z;   // steps/s
		}
		new_line_motion.delay = 16000000.0 / (spd_z * 1024.0) - 1.0; //The value of the OCR3A reg. it must be smaller than 65534
	}

	m_planner.addMotion(new_line_motion);
	processed_p = new_p;
	//TODO: activate the motion performer
}

void CNC_Router_ISR::moveToXY(const PositionXYZ &np, float spd) {
	moveToXY(np.X(), np.Y(), spd);
}

void CNC_Router_ISR::moveTo(const PositionXYZ &np, float spd) {
	moveTo(np.X(), np.Y(), np.Z(), spd);
}

void CNC_Router_ISR::stop() {
	int32_t stpx, stpy, stpz;
	m_performer.stopMotion(stpx, stpy, stpz);
	actual_p += PositionXYZ(stpx / (spmmx * m_performer.mx.control_mode),
			stpy / (spmmy * m_performer.my.control_mode),
			stpz / (spmmz * m_performer.mz.control_mode));
}

void CNC_Router_ISR::pause() {

	int32_t stpx, stpy, stpz;
	m_performer.pauseMotion(stpx, stpy, stpz);
	actual_p += PositionXYZ(stpx / (spmmx * m_performer.mx.control_mode),
			stpy / (spmmy * m_performer.my.control_mode),
			stpz / (spmmz * m_performer.mz.control_mode));
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

void CNC_Router_ISR::searchHomePos() {
	//TODO: searchNomePos
	if (ROUTER_DOWN_LIMIT_SWITCH_X_INTERRUPT > -1
			&& ROUTER_DOWN_LIMIT_SWITCH_Y_INTERRUPT > -1) {
		mx.backward(100000, 1000000 / v_max_x);
		while (CNC_Router_ISR::ls_x_down) {
			mx.update();
		}
		mx.stop();
		delay(500);

		mx.forward(100000, 1000000 / v_max_x);
		while (!CNC_Router_ISR::ls_x_down) {
			mx.update();
		}
		mx.stop();
		delay(500);

		my.backward(100000, 1000000 / v_max_y);
		while (CNC_Router_ISR::ls_y_down) {
			my.update();
		}
		my.stop();
		delay(500);

		my.forward(100000, 1000000 / v_max_y);
		while (!CNC_Router_ISR::ls_y_down) {
			my.update();
		}
		my.stop();
		delay(500);

		old_p = end_p = actual_p.X(0.0).Y(0.0);
	}
}

void CNC_Router_ISR::searchZ0Pos() {
	//TODO: searchZ0Pos
	if (ROUTER_DOWN_LIMIT_SWITCH_Z_INTERRUPT > -1) {
		mz.backward(100000, 1000000 / v_max_z);
		while (CNC_Router_ISR::ls_z_down) {
			mz.update();
		}
		mz.stop();

		old_p = end_p = actual_p.Z(0.0);

	}
}

PositionXYZ CNC_Router_ISR::getPos() {
	return actual_p;
}

void CNC_Router_ISR::setPos(PositionXYZ p) {
	actual_p = p;
}

void CNC_Router_ISR::orientationX(int8_t v) {
	mx.setOrientation(v);
}

void CNC_Router_ISR::orientationY(int8_t v) {
	my.setOrientation(v);
}

void CNC_Router_ISR::orientationZ(int8_t v) {
	mz.setOrientation(v);
}
