/*
 * MSMC_A4988.cpp
 *
 *  Created on: 17/lug/2014
 *      Author: francescogiurlanda
 */

#include "MSMC_A4988.h"

MSMC_A4988::MSMC_A4988() {
	pin_step = pin_dir = pin_enable = spd = steps = tot_steps = old_time =
			pause_time = dir = control_mode = 0;
	m_ready = false;
	m_pause = false;
	step_pin_val = LOW;
}

MSMC_A4988::MSMC_A4988(uint8_t pst, uint8_t pdr, uint8_t pen) {
	pin_dir = pdr;
	pin_enable = pen;
	pin_step = pst;
	spd = steps = tot_steps = old_time = pause_time = dir =
			control_mode = 0;
	pinMode(pin_dir, OUTPUT);
	pinMode(pin_enable, OUTPUT);
	pinMode(pin_step, OUTPUT);
	step_pin_val = LOW;
	m_ready = true;
	m_pause = false;
}

void MSMC_A4988::setDirection() {
	if (dir  == 1) {
		digitalWrite(pin_dir, HIGH);
	} else if (dir == -1) {
		digitalWrite(pin_dir, LOW);
	}
}

int8_t MSMC_A4988::getDir() {
	return dir;
}

void MSMC_A4988::enable() {
	digitalWrite(pin_enable, LOW);
}

void MSMC_A4988::disable() {
	digitalWrite(pin_enable, HIGH);
}

void MSMC_A4988::forward(uint32_t st, uint32_t sp) {
	if (m_ready) {
		tot_steps = st;
		steps = 0;
		spd = sp;
		dir = 1;
		old_time = micros();
		m_ready = false;
		setDirection();
		enable();
	}
}

void MSMC_A4988::backward(uint32_t st, uint32_t sp) {
	if (m_ready) {
		tot_steps = st;
		steps = 0;
		spd = sp;
		dir = -1;
		old_time = micros();
		m_ready = false;
		setDirection();
		enable();
	}
}

void MSMC_A4988::setPins(uint8_t pst, uint8_t pdr, uint8_t pen) {
	pin_dir = pdr;
	pin_enable = pen;
	pin_step = pst;
	pinMode(pin_dir, OUTPUT);
	pinMode(pin_enable, OUTPUT);
	pinMode(pin_step, OUTPUT);
	disable();
	m_ready = true;
}

void MSMC_A4988::setMode(uint8_t mod) {
	control_mode = mod;
}

uint8_t MSMC_A4988::getMode() {
	return control_mode;
}

void MSMC_A4988::speed(uint32_t sp) {
	spd = sp;
}

void MSMC_A4988::stop() {
	disable();
	dir = 0;
	m_ready = true;
	tot_steps = steps = 0;
}

void MSMC_A4988::pause() {
	if (!m_pause) {
		pause_time = micros();
		m_pause = true;
		disable();
	}
}

void MSMC_A4988::restart() {
	if (m_pause) {
		m_pause = false;
		old_time = micros() - (pause_time - old_time);
		enable();
	}
}

boolean MSMC_A4988::update() {
	uint32_t us = micros();
	uint32_t delta = us - old_time;

	if (dir != 0 && delta > spd && !m_pause) {
		if (steps == tot_steps) {
			disable();
			dir = 0;
			m_ready = true;
			return m_ready;
		}

		steps++;
		old_time = us;
		digitalWrite(pin_step, HIGH);
		step_pin_val = true;

		return m_ready;
	}

	if (step_pin_val) {
		digitalWrite(pin_step, LOW);
		step_pin_val = false;
	}

	return m_ready;
}

uint32_t MSMC_A4988::getSteps() {
	return steps;
}

