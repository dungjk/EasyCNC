/*! 
    \file      easy_CNC.ino
    \author    Francesco Giurlanda
    \version   0.2
    \date      2015
    \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
               To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#include "config.h"
#include "GCode.h"
#include "GCode_def.h"
#include "MillingMachine.h"
#include "CNCRouterISR.h"
#include "Position.h"
#include "MotorDriver.h"

MillingMachine mill(_MILLING_MACHINE_ENABLE_PIN, _MILLING_MACHINE_SPEED_PIN);
CNC_Router_ISR cncrt;

GCode gc(&cncrt, &mill);
char new_line[256];

void setup(){
       Serial.begin(SERIAL_BOUND);
	cncrt.initMotors();
	cncrt.resetPos();
	cncrt.setAbsolPos();
	cncrt.initInterrupts();
	cncrt.initMotionPerformer();

	gc.last_word[GROUP3] = G90;
	gc.init();

	mill.init();
}

void loop(){
  if (Serial.available() > 0) {
		new_line[Serial.readBytesUntil('\n', new_line, 256)] = '\0';
		gc.line = new_line;
		gc.parseLine();
		gc.sendAck();
	}
}

