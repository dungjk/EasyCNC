/*!
 \file      easy_CNC.cpp
 \brief     Main file.
 \author    Francesco Giurlanda
 \version   0.1
 \date      2014

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#include "easy_CNC.h"

#ifdef _TEST_1
void unit_test_1() {
	MotionPlanner p;
	LM m;
	m.delay = 2323;
	m.steps_x = 1200;
	m.steps_y = -890;
	m.steps_z = 100;
	DBG_TEST("Add 1", !p.addMotion(m));
	m.delay = 1000;
	m.steps_x = -10;
	m.steps_y = -20;
	m.steps_z = -30;
	DBG_TEST("Add 2", !p.addMotion(m));
	m.delay = 1500;
	m.steps_x = 50;
	m.steps_y = 90;
	m.steps_z = 100;
	DBG_TEST("Add 3", !p.addMotion(m));
	DBG_TEST("Get 1", !p.getMotion(m));
	DBG_TEST("Values get 1",
			m.delay == 2323 && m.steps_x == 1200 && m.steps_y == -890
					&& m.steps_z == 100);
	DBG_TEST("Get 2", !p.getMotion(m));
	DBG_TEST("Values get 2",
			m.delay == 1000 && m.steps_x == -10 && m.steps_y == -20
					&& m.steps_z == -30);
	DBG_TEST("Get 3", !p.getMotion(m));
	DBG_TEST("Values get 3",
			m.delay == 1500 && m.steps_x == 50 && m.steps_y == 90
					&& m.steps_z == 100);
	DBG_TEST("Get empty queue", p.getMotion(m));
}

MotionPlanner p;
MotionPerformer mp(&p);

void unit_test_2() {

	DBG_TEST("It is not working", mp.isNotWorking());
	LM m;
	m.delay = 8000;
	m.steps_x = 20;
	m.steps_y = 0;
	m.steps_z = 0;
	DBG_TEST("LM added", !p.addMotion(m));
	m.delay = 4000;
	m.steps_x = 30;
	m.steps_y = 0;
	m.steps_z = 0;
	DBG_TEST("LM added", !p.addMotion(m));
	m.delay = 10000;
	m.steps_x = 10;
	m.steps_y = 0;
	m.steps_z = 0;
	DBG_TEST("LM added", !p.addMotion(m));
}
void unit_test_3() {
	Serial.begin(SERIAL_BOUND);
	delay(1000);
	DBG_BP(1);
	cncrt.initMotors();

	cncrt.moveTo(10, -10, 0);
	delay(1000);
	while (!cncrt.m_performer.isNotWorking())
		delay(100);

	DBG_TEST("Step performed", (n_update == 1600 && n_stepdwn == 1600));
	DBG_VAR(n_update);
	DBG_VAR(n_stepdwn);
	/*
	 cncrt.moveTo(0, 10, 0);

	 cncrt.moveTo(-10, 0, 0);

	 cncrt.moveTo(0, -10, 0);

	 cncrt.moveTo(10, 0, 10);

	 cncrt.moveTo(0,0,0);*/

}
#endif

void setup() {
	Serial.begin(SERIAL_BOUND);
	//new_line.reserve(256);
	cncrt.initMotors();
	cncrt.resetPos();
	cncrt.setAbsolPos();
	cncrt.initInterrupts();
	cncrt.initMotionPerformer();

	gc.last_word[GROUP3] = G90;
	gc.init();

	mill.init();
}

void loop() {

	if (Serial.available() > 0) {
		new_line[Serial.readBytesUntil('\n', new_line, 256)] = '\0';
		gc.line = new_line;
		gc.parseLine();
		gc.sendAck();
	}
}
