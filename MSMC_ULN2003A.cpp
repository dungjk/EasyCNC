//  Quest'opera �� stata rilasciata con licenza Creative Commons Attribuzione
//  - Condividi allo stesso modo 4.0 Internazionale. 
//  Per leggere una copia della licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
//
//  Autor: Francesco Giurlanda
//  Project: Easy CNC
//  versione: 0.0.1

#include "MSMC_ULN2003A.h"

//********** COSTRUCTOR **********
MSMC_ULN2003A::MSMC_ULN2003A() {
	p1 = p2 = p3 = p4 = old_time = pause_time = steps = tot_steps = spd = pos =
			dir = 0;
	control_mode = 0;
	m_ready = false; //the motor is not ready because the pins are not specified yet.
	m_pause = false;
}

MSMC_ULN2003A::MSMC_ULN2003A(uint8_t pin1, uint8_t pin2, uint8_t pin3,
		uint8_t pin4) {
	p1 = pin1;
	p2 = pin2;
	p3 = pin3;
	p4 = pin4;
	pinMode(p1, OUTPUT);
	pinMode(p2, OUTPUT);
	pinMode(p3, OUTPUT);
	pinMode(p4, OUTPUT);
	control_mode = 0;
	pause_time = old_time = steps = tot_steps = spd = pos = dir = 0;
	m_ready = true;
	m_pause = false;
}

//********** SETUP ************

void MSMC_ULN2003A::speed(uint32_t s) {
	spd = s;
}

void MSMC_ULN2003A::setMode(uint8_t mod) {
	if(mod != FULL_STEP || mod != HALF_STEP)
		m_ready = false;   //mode not supported
	else
		control_mode = mod;
}

uint8_t MSMC_ULN2003A::getMode() {
	return control_mode;
}

void MSMC_ULN2003A::setPins(uint8_t pin1, uint8_t pin2, uint8_t pin3,
		uint8_t pin4) {
	if (!m_ready) {
		p1 = pin1;
		p2 = pin2;
		p3 = pin3;
		p4 = pin4;
		pinMode(p1, OUTPUT);
		pinMode(p2, OUTPUT);
		pinMode(p3, OUTPUT);
		pinMode(p4, OUTPUT);
		m_ready = true;
	}
}

//********** MOVING ************
void MSMC_ULN2003A::forward(uint32_t st, uint32_t sp) {
	steps = tot_steps = st;

	spd = sp;
	dir = 1;
	old_time = micros();
	m_ready = false;
}

void MSMC_ULN2003A::backward(uint32_t st, uint32_t sp) {
	steps = tot_steps = st;
	spd = sp;
	dir = -1;
	old_time = micros();
	m_ready = false;
}

void MSMC_ULN2003A::forward_one() {
	switch (control_mode) {
	case FULL_STEP:
		pos = (pos + 1) % 4;
		digitalWrite(p1, full_step_seq[pos][0]);
		digitalWrite(p2, full_step_seq[pos][1]);
		digitalWrite(p3, full_step_seq[pos][2]);
		digitalWrite(p4, full_step_seq[pos][3]);
		break;
	case HALF_STEP:
		pos = (pos + 1) % 8;
		digitalWrite(p1, half_step_seq[pos][0]);
		digitalWrite(p2, half_step_seq[pos][1]);
		digitalWrite(p3, half_step_seq[pos][2]);
		digitalWrite(p4, half_step_seq[pos][3]);
		break;
	default:
		;
	}
}

void MSMC_ULN2003A::backward_one() {
	switch (control_mode) {
	case FULL_STEP:
		pos = (pos != 0) ? (pos - 1) : 3;
		digitalWrite(p1, full_step_seq[pos][0]);
		digitalWrite(p2, full_step_seq[pos][1]);
		digitalWrite(p3, full_step_seq[pos][2]);
		digitalWrite(p4, full_step_seq[pos][3]);
		break;
	case HALF_STEP:
		pos = (pos != 0) ? (pos - 1) : 7;
		digitalWrite(p1, half_step_seq[pos][0]);
		digitalWrite(p2, half_step_seq[pos][1]);
		digitalWrite(p3, half_step_seq[pos][2]);
		digitalWrite(p4, half_step_seq[pos][3]);
		break;
	default:
		;
	}
}

void MSMC_ULN2003A::coilsOff() {
	digitalWrite(p1, LOW);
	digitalWrite(p2, LOW);
	digitalWrite(p3, LOW);
	digitalWrite(p4, LOW);
}

void MSMC_ULN2003A::stop() {
	coilsOff();
	dir = 0;
	m_ready = true;
	steps = 0;
}

void MSMC_ULN2003A::pause() {
	if (!m_pause) {
		m_pause = true;
		pause_time = micros();
		coilsOff();
	}
}

void MSMC_ULN2003A::restart() {
	if (m_pause) {
		m_pause = false;
		old_time = micros() - (pause_time - old_time);
	}
}

//********* STEP UPDATE **************  
boolean MSMC_ULN2003A::update() {
	uint32_t us = micros();
	uint32_t delta = us - old_time;

	if (dir != 0 && (us - old_time) > spd && !m_pause) {

		if (steps == 0) {  //after the last step, the system turns off the motor
			coilsOff();
			dir = 0;
			m_ready = true;
			return m_ready;
		}

		steps++;
		old_time = us;
		if (dir == 1) {
			forward_one();
		} else {
			backward_one();
		}
	}
	return m_ready;
}

uint32_t MSMC_ULN2003A::getSteps(){
	return steps;
}

//********* DEBUGGING ***************
void MSMC_ULN2003A::print() {
	Serial.println(p1);
	Serial.println(p2);
	Serial.println(p3);
	Serial.println(p4);
	Serial.println(dir);
	Serial.println(pos);
	Serial.println(spd);
	Serial.println(steps);
	Serial.println(old_time);
#ifdef SM_DBG
	ledOn();
	delay(1000);
	ledOff();
	delay(1000);
#endif
}

