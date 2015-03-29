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

void setup() {

	lcd.backlight();
	lcd.clear();
	lcd.setCursor(4, 1);
	lcd.print("EasyCNC v0.2");

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
