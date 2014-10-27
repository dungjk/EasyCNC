// Do not remove the include below
#include "easy_CNC.h"

//The setup function is called once at startup of the sketch
void setup() {
// Add your initialization code here
	cncrt.initMotorX();
	cncrt.initMotorY();
	cncrt.initMotorZ();
	cncrt.resetPos();
	cncrt.setMotionModeX(QUARTER_STEP);
	cncrt.setMotionModeY(QUARTER_STEP);
	cncrt.setMotionModeZ(QUARTER_STEP);
	cncrt.setAbsolPos(); gc.last_word[GROUP3] = G90;
	cncrt.initInterrupts();
	cncrt.orientationX(-1);

	mill.init();

	//attachInterrupt(INTERRUPT_STOP_MOTION, stopButton, FALLING);
	//digitalWrite(INTERRUPT_STOP_MOTION, HIGH);

	Serial.begin(SERIAL_BOUND);

}

// The loop function is called in an endless loop
void loop() {
	if (Serial.available() > 0) {
		new_line[Serial.readBytesUntil('\n', new_line, 256)] = '\0';
		gc.line = new_line;
		gc.parseLine();
	}
}
