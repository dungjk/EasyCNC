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

MillingMachine::MillingMachine(uint8_t e, uint8_t c) :
		enable_pin(e), control_pin(c), bit_speed(0), bit_dir(true), state(
				false) {
}

void MillingMachine::init() {
	pinMode(enable_pin, OUTPUT);
	analogWrite(control_pin, bit_speed);
}


void MillingMachine::setSpindleSpeed(int s) {
	//TODO Implementation of MillingMachine::setBitSpeed
}

void MillingMachine::setSpindleDir(boolean d) {
	//TODO Implementation of MillingMachine::setBitDir
}

void MillingMachine::enable(){
	state = true;
	digitalWrite(enable_pin, HIGH);
}

void MillingMachine::disable(){
	state = false;
	digitalWrite(enable_pin, LOW);
}

#endif

// *******************************
