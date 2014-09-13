//  Quest'opera �� stata rilasciata con licenza Creative Commons Attribuzione
//  - Condividi allo stesso modo 4.0 Internazionale. 
//  Per leggere una copia della licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
//
//  Autor: Francesco Giurlanda
//  Project: Easy CNC
//  versione: 0.0.1

#include "CNC_Router.h"

CNC_Router::CNC_Router() :
		mx(), my(), old_p(), end_p(), actual_p(), spmmx(0.0), spmmy(0.0), v_max_x(
				0.0), v_max_y(0.0), pos_type(false), pin_ls_x_down(-1), pin_ls_y_down(
				-1), pin_ls_x_up(-1), pin_ls_y_up(-1) {
}
CNC_Router::CNC_Router(float spx, float spy, float vmx, float vmy) :
		mx(), my(), old_p(), end_p(), actual_p(), spmmx(spx), spmmy(spy), v_max_x(
				vmx), v_max_y(vmy), pos_type(false), pin_ls_x_down(-1), pin_ls_y_down(
				-1), pin_ls_x_up(-1), pin_ls_y_up(-1) {
}

void CNC_Router::initMotorX() {
#if defined(ROUTER_MX_CONTROLLER_ULN2003A)
	mx.setPins(ROUTER_MX_COIL1_PIN, ROUTER_MX_COIL2_PIN, ROUTER_MX_COIL3_PIN, ROUTER_MX_COIL4_PIN);
#elif defined(ROUTER_MX_CONTROLLER_A4988)
	mx.setPins(ROUTE_MX_STEP_CONTROL_PIN, ROUTE_MX_DIRECTION_CONTROL_PIN,
			ROUTE_MX_ENABLE_CONTROL_PIN);
#endif
	mx.dirMode(ROUTER_MX_DIR);
}

void CNC_Router::initMotorY() {
#if defined(ROUTER_MY_CONTROLLER_ULN2003A)
	my.setPins(ROUTER_MY_COIL1_PIN, ROUTER_MY_COIL2_PIN, ROUTER_MY_COIL3_PIN, ROUTER_MY_COIL4_PIN);
#elif defined(ROUTER_MY_CONTROLLER_A4988)
	my.setPins(ROUTE_MY_STEP_CONTROL_PIN, ROUTE_MY_DIRECTION_CONTROL_PIN,
			ROUTE_MY_ENABLE_CONTROL_PIN);
#endif
	my.dirMode(ROUTER_MY_DIR);
}

void CNC_Router::resetPos() {
	old_p = end_p = actual_p = PositionXY(0.0, 0.0);
}

void CNC_Router::moveTo(float px, float py) {
	float dx, dy;

	old_p = actual_p;
	end_p.X(px).Y(py);

	if (pos_type) {
		dx = PositionXY().distX(end_p);
		dy = PositionXY().distY(end_p);
	} else {
		dx = actual_p.distX(end_p);
		dy = actual_p.distY(end_p);
	}

	float steps_x = abs(dx * spmmx * mx.getMode()); // (mm * steps/mm * num = steps)
	float steps_y = abs(dy * spmmy * my.getMode());

	float tx = steps_x / v_max_x;
	float ty = steps_y / v_max_y;
	float v_x, v_y;

	if (tx > ty) {
		v_y = steps_y / tx;
		v_x = v_max_x;
	} else {
		v_x = steps_x / ty;
		v_y = v_max_y;
	}

	if (dx > 0.0) {
		mx.forward(steps_x, 1000000 / v_x); // the delay per step is in microseconds
	} else if (dx < 0.0) {
		mx.backward(steps_x, 1000000 / v_x);
	}

	if (dy > 0.0) {
		my.forward(steps_y, 1000000 / v_y);
	} else if (dy < 0.0) {
		my.backward(steps_y, 1000000 / v_y);
	}

}

void CNC_Router::moveTo(const PositionXY &np) {
	moveTo(np.X(), np.Y());
}

void CNC_Router::stopMotion() {
	mx.stop();
	my.stop();
}

void CNC_Router::pause() {
	mx.pause();
	my.pause();
}

boolean CNC_Router::update() {
	int32_t a = mx.update();
	int32_t b = my.update();
	if (a == -1 || b == -1) {
		actual_p = end_p;
	} else {
		actual_p = old_p
				+ PositionXY(mx.getDir() * a / (spmmx * mx.getMode()),
						my.getDir() * b / (spmmy * my.getMode()));
	}

	return a == -1 && b == -1;
}

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

