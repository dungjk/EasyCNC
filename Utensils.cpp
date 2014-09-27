//  Quest'opera �� stata rilasciata con licenza Creative Commons Attribuzione
//  - Condividi allo stesso modo 4.0 Internazionale. 
//  Per leggere una copia della licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
//
//  Autor: Francesco Giurlanda
//  Project: Easy CNC
//  versione: 0.0.1

#include "Utensils.h"

// *******************************
// ***** PLOTTER WITH SERVO  *****
// *******************************
#ifdef _PLOTTER_SERVO

PlotterServo::PlotterServo(uint8_t p, uint8_t dv, uint8_t uv) : pin(p), down_val(dv), up_val(uv) {}

void PlotterServo::init() {
	servo_pen.attach(pin);
	state = UP;
	servo_pen.write(up_val);
}

boolean PlotterServo::up() {
	boolean ret = (state != UP);
	if(ret) {
		state = UP;
		servo_pen.write(up_val);
		delay(500);
	}
	return ret;
}

boolean PlotterServo::down() {
	boolean ret = (state != DOWN);
	if(ret) {
		state = DOWN;
		servo_pen.write(down_val);
		delay(500);
	}
	return ret;
}

#endif

// ****************************
// ***** MILLING MACHINE  *****
// ****************************
#ifdef _MILLING_MACHINE

MillingMachine::MillingMachine() :
		mz(), old_p(), end_p(), actual_p(), bit_speed(0.0), bit_dir(true), spmm(
				0.0), v_max_z(0.0), pos_type(false), pin_ls_z_down(-1), pin_ls_z_up(
				-1) {
}
MillingMachine::MillingMachine(float s, float vmax) :
		mz(), old_p(), end_p(), actual_p(), bit_speed(0.0), bit_dir(true), spmm(
				s), v_max_z(vmax), pos_type(false), pin_ls_z_down(-1), pin_ls_z_up(
				-1) {
}

void MillingMachine::init() {
#if defined(_MILLING_MACHINE_MZ_CONTROLLER_ULN2003A)
	mz.setPins(_MILLING_MACHINE_MZ_COIL1_PIN, _MILLING_MACHINE_MZ_COIL2_PIN, _MILLING_MACHINE_MZ_COIL3_PIN, _MILLING_MACHINE_MZ_COIL4_PIN);
#elif defined(_MILLING_MACHINE_MZ_CONTROLLER_A4988)
	mz.setPins(_MILLING_MACHINE_MZ_STEP_CONTROL_PIN,
	_MILLING_MACHINE_MZ_DIRECTION_CONTROL_PIN,
	_MILLING_MACHINE_MZ_ENABLE_CONTROL_PIN);
#endif
}

float MillingMachine::getPos() {
	return actual_p;
}

void MillingMachine::resetPos() {
	old_p = end_p = actual_p = 0.0;
}

void MillingMachine::moveTo(float p, float spd) {
	float dz;
	float delay;
	old_p = actual_p;
	end_p = p;

	if (spd > 0.0) {
		delay = 1000000 / (
				((spd * spmm * mz.getMode()) > v_max_z) ?
						v_max_z : spd * spmm * mz.getMode());
	} else {
		delay = 1000000 / v_max_z;
	}

	if (pos_type) {
		dz = p;
	} else {
		dz = p - actual_p;
	}

	float steps_z = abs(dz * spmm * mz.getMode()); // (mm * steps/mm * num = steps)
	if (dz > 0.0) {
		mz.forward(steps_z, delay);
	} else {
		mz.backward(steps_z, delay);
	}
}

void MillingMachine::stopMotion() {
	actual_p = old_p + mz.getDir() * mz.getSteps() / (spmm * mz.getMode());
	mz.stop();
}

void MillingMachine::pause() {
	mz.pause();
	actual_p = old_p + mz.getDir() * mz.getSteps() / (spmm * mz.getMode());
}

void MillingMachine::restart() {
	mz.restart();
}

void MillingMachine::highPrecision() {
#if defined(_MILLING_MACHINE_MZ_CONTROLLER_ULN2003A)
	mz.setMode(HALF_STEP);
#elif defined(_MILLING_MACHINE_MZ_CONTROLLER_A4988)
	mz.setMode(EIGHTH_STEP);
#endif
}

void MillingMachine::lowPrecision() {
	mz.setMode(FULL_STEP);
}

void MillingMachine::setIncrPos() {
	pos_type = true;
}

void MillingMachine::setAbsolPos() {
	pos_type = false;
}

void MillingMachine::searchZeroPos() {
	if (pin_ls_z_down != -1) {
		mz.forward(100000, 1000000 / v_max_z);
		while (digitalRead(pin_ls_z_down) != LOW) {
			mz.update();
		}
		mz.stop();
		resetPos();
		mz.backward(spmm * 20, 1000000 / v_max_z); //move back the drill bit of 2 cm
	}
}

void MillingMachine::setLimitSwitch(int8_t dw, int8_t up) {
	if (dw >= 0) {
		pin_ls_z_down = dw;
		pinMode(pin_ls_z_down, INPUT_PULLUP);
	}

	if (up >= 0) {
		pin_ls_z_up = up;
		pinMode(pin_ls_z_up, INPUT_PULLUP);
	}
}

void MillingMachine::setBitSpeed(int s) {
	//TODO Implementation of MillingMachine::setBitSpeed
}

void MillingMachine::setBitDir(boolean d) {
	//TODO Implementation of MillingMachine::setBitDir
}

boolean MillingMachine::update() {
	if (mz.update()) {
		actual_p = end_p;
		return true;
	}
	return false;
}

#endif

// *******************************
