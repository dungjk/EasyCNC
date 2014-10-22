//  Quest'opera �� stata rilasciata con licenza Creative Commons Attribuzione
//  - Condividi allo stesso modo 4.0 Internazionale. 
//  Per leggere una copia della licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
//
//  Autor: Francesco Giurlanda
//  Project: Easy CNC
//  versione: 0.0.1

#include "CNC_Router.h"

CNC_Router::CNC_Router() :
		mx(), my(), old_p(), end_p(), actual_p(), spmmx(0.0), spmmy(0.0), spmmz(
				0.0), v_max_x(0.0), v_max_y(0.0), v_max_z(0.0), pos_type(false), pin_ls_x_down(
				-1), pin_ls_y_down(-1), pin_ls_z_down(-1), pin_ls_x_up(-1), pin_ls_y_up(
				-1), pin_ls_z_up(-1) {
}
CNC_Router::CNC_Router(float spx, float spy, float spz, float vmx, float vmy,
		float vmz) :
		mx(), my(), old_p(), end_p(), actual_p(), spmmx(spx), spmmy(spy), spmmz(
				spz), v_max_x(vmx), v_max_y(vmy), v_max_z(vmz), pos_type(false), pin_ls_x_down(
				-1), pin_ls_y_down(-1), pin_ls_z_down(-1), pin_ls_x_up(-1), pin_ls_y_up(
				-1), pin_ls_z_up(-1) {
}

void CNC_Router::initMotorX() {
#if defined(ROUTER_MX_CONTROLLER_ULN2003A)
	mx.setPins(ROUTER_MX_COIL1_PIN, ROUTER_MX_COIL2_PIN, ROUTER_MX_COIL3_PIN, ROUTER_MX_COIL4_PIN);
#elif defined(ROUTER_MX_CONTROLLER_A4988)
	mx.setPins(ROUTE_MX_STEP_CONTROL_PIN, ROUTE_MX_DIRECTION_CONTROL_PIN,
	ROUTE_MX_ENABLE_CONTROL_PIN);
#endif
}

void CNC_Router::initMotorY() {
#if defined(ROUTER_MY_CONTROLLER_ULN2003A)
	my.setPins(ROUTER_MY_COIL1_PIN, ROUTER_MY_COIL2_PIN, ROUTER_MY_COIL3_PIN, ROUTER_MY_COIL4_PIN);
#elif defined(ROUTER_MY_CONTROLLER_A4988)
	my.setPins(ROUTE_MY_STEP_CONTROL_PIN, ROUTE_MY_DIRECTION_CONTROL_PIN,
	ROUTE_MY_ENABLE_CONTROL_PIN);
#endif
}

void CNC_Router::initMotorZ() {
#if defined(ROUTER_MZ_CONTROLLER_ULN2003A)
	mz.setPins(ROUTER_MZ_COIL1_PIN, ROUTER_MZ_COIL2_PIN, ROUTER_MZ_COIL3_PIN, ROUTER_MZ_COIL4_PIN);
#elif defined(ROUTER_MZ_CONTROLLER_A4988)
	mz.setPins(ROUTE_MZ_STEP_CONTROL_PIN, ROUTE_MZ_DIRECTION_CONTROL_PIN,
	ROUTE_MZ_ENABLE_CONTROL_PIN);
#endif
}

void CNC_Router::resetPos() {
	old_p = end_p = actual_p = PositionXYZ(0.0, 0.0, 0.0);
}

void CNC_Router::moveToXY(float px, float py, float spd) {

	float dx, dy, dist, spd_x, spd_y, actual_spmmx, actual_spmmy;
	actual_spmmx = spmmx * mx.getMode();
	actual_spmmy = spmmy * my.getMode();

	old_p = actual_p;
	end_p.X(px).Y(py).Z(actual_p.Z());

	if (pos_type) {
		dx = PositionXYZ().offsetX(end_p);
		dy = PositionXYZ().offsetY(end_p);
		dist = PositionXYZ().moduleXY(end_p);
	} else {
		dx = actual_p.offsetX(end_p);
		dy = actual_p.offsetY(end_p);
		dist = actual_p.moduleXY(end_p);
	}

	float steps_x = abs(dx * actual_spmmx); // (mm * steps/mm * num = steps)
	float steps_y = abs(dy * actual_spmmy);

	if (spd > 0.0) {
		float ratio = spd / dist;          // (mm/s)/mm = 1/s
		spd_x = abs(ratio * dx);   // mm/s
		spd_y = abs(ratio * dy);   // mm/s
		float check_x = spd_x - (v_max_x / actual_spmmx);
		float check_y = spd_y - (v_max_y / actual_spmmy);

		if (check_x > 0 || check_y > 0) {
			if (check_x > check_y) {
				spd_x = v_max_x / actual_spmmx;   // mm/s
				float lower_spd = dist * (spd_x) / dx;
				spd_y = abs(dy * lower_spd / dist);
			} else {
				spd_y = v_max_y / actual_spmmy;
				float lower_spd = dist * (spd_y) / dy;
				spd_x = abs(dx * lower_spd / dist);
			}
		}
	} else {
		float tx = steps_x / v_max_x;
		float ty = steps_y / v_max_y;

		if (tx > ty) {
			spd_y = steps_y / (tx * actual_spmmy);
			spd_x = v_max_x / actual_spmmx;
		} else {
			spd_x = steps_x / (ty * actual_spmmx);
			spd_y = v_max_y / actual_spmmy;
		}
	}

	if (dx > 0.0) {
		mx.forward(steps_x, 1000000 / (spd_x * actual_spmmx)); // the delay per step is in microseconds
	} else if (dx < 0.0) {
		mx.backward(steps_x, 1000000 / (spd_x * actual_spmmx));
	}

	if (dy > 0.0) {
		my.forward(steps_y, 1000000 / (spd_y * actual_spmmy));
	} else if (dy < 0.0) {
		my.backward(steps_y, 1000000 / (spd_y * actual_spmmy));
	}

}

void CNC_Router::moveTo(float px, float py, float pz, float spd) {
	float dx, dy, dz, dist, spd_x, spd_y, spd_z, actual_spmmx, actual_spmmy,
			actual_spmmz;
	actual_spmmx = spmmx * mx.getMode();
	actual_spmmy = spmmy * my.getMode();
	actual_spmmz = spmmz * mz.getMode();

	old_p = actual_p;
	end_p.X(px).Y(py).Z(pz);

	if (pos_type) {
		dx = PositionXYZ().offsetX(end_p);
		dy = PositionXYZ().offsetY(end_p);
		dz = PositionXYZ().offsetZ(end_p);
		dist = PositionXYZ().module(end_p);
	} else {
		dx = actual_p.offsetX(end_p);
		dy = actual_p.offsetY(end_p);
		dz = actual_p.offsetZ(end_p);
		dist = actual_p.module(end_p);
	}

	float steps_x = abs(dx * actual_spmmx); // (mm * steps/mm * num = steps)
	float steps_y = abs(dy * actual_spmmy);
	float steps_z = abs(dz * actual_spmmz);

	if (spd > 0.0) {
		float ratio = spd / dist;          // (mm/s)/mm = 1/s
		spd_x = abs(ratio * dx);   // mm/s
		spd_y = abs(ratio * dy);   // mm/s
		spd_z = abs(ratio * dz);   // mm/s
		float check_x = spd_x - (v_max_x / actual_spmmx);
		float check_y = spd_y - (v_max_y / actual_spmmy);
		float check_z = spd_z - (v_max_z / actual_spmmy);

		if (check_x > 0 || check_y > 0 || check_z > 0) {
			if (check_x > check_y && check_x > check_z) {
				spd_x = v_max_x / actual_spmmx;   // mm/s
				float lower_spd = spd_x / dx;   // 1/s
				spd_y = abs(dy * lower_spd);     // mm/s
				spd_z = abs(dz * lower_spd);      // mm/s
			} else if (check_y > check_z) {
				spd_y = v_max_y / actual_spmmy;  // mm/s
				float lower_spd = spd_y / dy;    // 1/s
				spd_x = abs(dx * lower_spd);     // mm/s
				spd_z = abs(dz * lower_spd);     // mm/s
			} else {
				spd_z = v_max_z / actual_spmmz;  // mm/s
				float lower_spd = spd_z / dz;    // 1/s
				spd_x = abs(dx * lower_spd);     // mm/s
				spd_y = abs(dy * lower_spd);     // mm/s
			}
		}
	} else {
		float tx = steps_x / v_max_x;
		float ty = steps_y / v_max_y;
		float tz = steps_z / v_max_z;

		if (tx > ty && tx > tz) {
			spd_y = steps_y / (tx * actual_spmmy);
			spd_z = steps_z / (tx * actual_spmmz);
			spd_x = v_max_x / actual_spmmx;
		} else if (ty > tz) {
			spd_x = steps_x / (ty * actual_spmmx);
			spd_z = steps_z / (ty * actual_spmmz);
			spd_y = v_max_y / actual_spmmy;
		} else {
			spd_x = steps_x / (tz * actual_spmmx);
			spd_y = steps_y / (tz * actual_spmmy);
			spd_z = v_max_z / actual_spmmz;
		}
	}

	if (dx > 0.0) {
		mx.forward(steps_x, 1000000 / (spd_x * actual_spmmx)); // the delay per step is in microseconds
	} else if (dx < 0.0) {
		mx.backward(steps_x, 1000000 / (spd_x * actual_spmmx));
	}

	if (dy > 0.0) {
		my.forward(steps_y, 1000000 / (spd_y * actual_spmmy));
	} else if (dy < 0.0) {
		my.backward(steps_y, 1000000 / (spd_y * actual_spmmy));
	}

	if (dz > 0.0) {
		mz.forward(steps_z, 1000000 / (spd_z * actual_spmmz));
	} else if (dz < 0.0) {
		mz.backward(steps_z, 1000000 / (spd_z * actual_spmmz));
	}
}

void CNC_Router::moveToXY(const PositionXYZ &np, float spd) {
	moveToXY(np.X(), np.Y(), spd);
}

void CNC_Router::moveTo(const PositionXYZ &np, float spd) {
	moveTo(np.X(), np.Y(), np.Z(), spd);
}

void CNC_Router::stopMotion() {
	actual_p = old_p
			+ PositionXYZ(mx.getDir() * mx.getSteps() / (spmmx * mx.getMode()),
					my.getDir() * my.getSteps() / (spmmy * my.getMode()),
					mz.getDir() * mz.getSteps() / (spmmz * mz.getMode()));
	mx.stop();
	my.stop();
	mz.stop();
}

void CNC_Router::pause() {
	mx.pause();
	my.pause();
	mz.pause();
	actual_p = old_p
			+ PositionXYZ(mx.getDir() * mx.getSteps() / (spmmx * mx.getMode()),
					my.getDir() * my.getSteps() / (spmmy * my.getMode()),
					mz.getDir() * mz.getSteps() / (spmmz * mz.getMode()));
}

void CNC_Router::restart() {
	mx.restart();
	my.restart();
	mz.restart();
}

boolean CNC_Router::update() {
	boolean a = mx.update();
	boolean b = my.update();
	boolean c = mz.update();
	if (a && b && c) {
		if(pos_type)
			actual_p += end_p;
		else
			actual_p = end_p;
		return true;
	}

	return false;
}

void CNC_Router::setMotionModeX(uint8_t m){
	mx.setMode(m);
}

void CNC_Router::setMotionModeY(uint8_t m){
	my.setMode(m);
}

void CNC_Router::setMotionModeZ(uint8_t m){
	mz.setMode(m);
}
/*
void CNC_Router::highPrecision() {
#if defined(ROUTER_MX_CONTROLLER_ULN2003A)
	mx.setMode(HALF_STEP);
#elif defined(ROUTER_MX_CONTROLLER_A4988)
	mx.setMode(EIGHTH_STEP);
#endif

#if defined(ROUTER_MY_CONTROLLER_ULN2003A)
	my.setMode(HALF_STEP);
#elif defined(ROUTER_MY_CONTROLLER_A4988)
	my.setMode(EIGHTH_STEP);
#endif

}

void CNC_Router::lowPrecision() {
	mx.setMode(FULL_STEP);
	my.setMode(FULL_STEP);
}

*/

void CNC_Router::setIncrPos() {
	pos_type = true;
}

void CNC_Router::setAbsolPos() {
	pos_type = false;
}

void CNC_Router::searchHomePos() {
	if (pin_ls_x_down != -1 && pin_ls_y_down != -1) {
		mx.backward(100000, 1000000 / v_max_x);
		while (digitalRead(pin_ls_x_down) != LOW) {
			mx.update();
		}
		mx.stop();
		delay(500);

		my.backward(100000, 1000000 / v_max_y);
		while (digitalRead(pin_ls_y_down) != LOW) {
			my.update();
		}
		my.stop();

		resetPos();
	}
}

void CNC_Router::setLimitSwitchX(int8_t dw, int8_t up) {

	if (dw >= 0) {
		pin_ls_x_down = dw;
		pinMode(pin_ls_x_down, INPUT_PULLUP);
	}

	if (up >= 0) {
		pin_ls_x_up = up;
		pinMode(pin_ls_x_up, INPUT_PULLUP);
	}
}

void CNC_Router::setLimitSwitchY(int8_t dw, int8_t up) {

	if (dw >= 0) {
		pin_ls_y_down = dw;
		pinMode(pin_ls_y_down, INPUT_PULLUP);
	}

	if (up >= 0) {
		pin_ls_y_up = up;
		pinMode(pin_ls_y_up, INPUT_PULLUP);
	}
}

void CNC_Router::setLimitSwitchZ(int8_t dw, int8_t up) {

	if (dw >= 0) {
		pin_ls_z_down = dw;
		pinMode(pin_ls_z_down, INPUT_PULLUP);
	}

	if (up >= 0) {
		pin_ls_z_up = up;
		pinMode(pin_ls_z_up, INPUT_PULLUP);
	}
}

PositionXYZ CNC_Router::getPos() {
	return actual_p;
}

void CNC_Router::orientationX(int8_t v) {
	mx.setOrientation(v);
}

void CNC_Router::orientationY(int8_t v) {
	my.setOrientation(v);
}

void CNC_Router::orientationZ(int8_t v) {
	mz.setOrientation(v);
}
