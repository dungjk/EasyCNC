/*!
 \file      easy_CNC.cpp
 \brief     Main file.
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#include "easy_CNC.h"

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

#ifdef _MILLING_MACHINE
	tool.init();
#endif
#ifdef _LASER
	tool.init();
#endif
#ifdef _PLOTTER_SERVO
	tool.init();
#endif
}

void loop() {
	if (Serial.available() > 0) {
		new_line[Serial.readBytesUntil('\n', new_line, 256)] = '\0';
		gc.line = new_line;
		gc.parseLine();
		gc.sendAck();
	}
}
