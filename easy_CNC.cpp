// Do not remove the include below
#include "easy_CNC.h"

//The setup function is called once at startup of the sketch
void setup() {
// Add your initialization code here
	cncrt.initMotorX();
	cncrt.initMotorY();
	cncrt.initMotorZ();
	cncrt.resetPos();
	cncrt.setMotionModeX(EIGHTH_STEP);
	cncrt.setMotionModeY(EIGHTH_STEP);
	cncrt.setMotionModeZ(EIGHTH_STEP);
	cncrt.setAbsolPos(); gc.params[GROUP3] = G90;
	cncrt.setLimitSwitchX(ROUTER_DOWN_LIMIT_SWITCH_X);
	cncrt.setLimitSwitchY(ROUTER_DOWN_LIMIT_SWITCH_Y);
	cncrt.setLimitSwitchZ(ROUTER_DOWN_LIMIT_SWITCH_Z);
	cncrt.orientationX(-1);

	mill.init();

	attachInterrupt(INTERRUPT_STOP_MOTION, stopButton, FALLING);
	digitalWrite(INTERRUPT_STOP_MOTION, HIGH);

	//mypen.init();

	Serial.begin(SERIAL_BOUND);

}

// The loop function is called in an endless loop
void loop() {
#ifndef _TEST
	if (Serial.available() > 0) {
		//memset(new_line, '\0', 256);
		new_line[Serial.readBytesUntil('\n', new_line, 256)] = '\0';
		gc.line = new_line;
		gc.parseLine();
	}
#endif
#ifdef _TEST
	if (Serial.available() > 0) {
		memset(new_line, '\0', 256);
		Serial.readBytesUntil('\n', new_line, 256);
		gc.line = new_line;
		gc.parseLine();

	}

#endif

}
