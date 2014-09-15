// Do not remove the include below
#include "easy_CNC.h"

//The setup function is called once at startup of the sketch
void setup() {
// Add your initialization code here
	mycnc.initMotorX();
	mycnc.initMotorY();
	mycnc.resetPos();
	mycnc.highPrecision();
	mycnc.setAbsolPos();
	mycnc.setLimitSwitchX(ROUTER_DOWN_LIMIT_SWITCH_X);
	mycnc.setLimitSwitchY(ROUTER_DOWN_LIMIT_SWITCH_Y);

	mill.init();
	mill.resetPos();
	mill.highPrecision();
	mill.setAbsolPos();
	mill.setLimitSwitch(_MILLING_MACHINE_DOWN_LIMIT_SWITCH_Z);

	attachInterrupt(INTERRUPT_STOP_MOTION, stopButton, FALLING);
	digitalWrite(INTERRUPT_STOP_MOTION, HIGH);

	//mypen.init();

	Serial.begin(9600);
}

// The loop function is called in an endless loop
void loop() {
//Add your repeated code here
	tloop.start();
	a = mycnc.update();
	b = mill.update();
	if (a && b) {

		if (end_task) {
			//Serial.println('a');
			Serial.print("Min: ");
			Serial.println(tloop.getMin());
			Serial.print("Max: ");
			Serial.println(tloop.getMax());

			end_task = false;
		}

		if (Serial.available() > 0) {
			switch (Serial.parseInt()) {
			case 0:
				//mypen.up();
				processPos();
				break;
			case 1:
				//mypen.down();
				processPos();
				break;
			case 2:
				mycnc.resetPos();
				mill.resetPos();
				break;
			case 3:
				mycnc.searchHomePos();
				mill.searchZeroPos();
			}

			end_task = true;
		}
	}
	tloop.stop();
}
