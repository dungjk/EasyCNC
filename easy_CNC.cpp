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
	mycnc.setLimitSwitchX(ROUTER_LIMIT_X);
	mycnc.setLimitSwitchY(ROUTER_LIMIT_Y);

	attachInterrupt(INTERRUPT_STOP_MOTION, stopButton, FALLING);
	digitalWrite(2, HIGH);

	mypen.init();

	Serial.begin(9600);
}

// The loop function is called in an endless loop
void loop() {
//Add your repeated code here
	if( mycnc.update()){

	    if(end_task){
	      Serial.print('a');
	      end_task = false;
	    }

	    if(Serial.available() > 0){
	      switch( Serial.parseInt() ) {
	        case 0:
	           mypen.up();
	           processPos();
	           break;
	        case 1:
	           mypen.down();
	           processPos();
	           break;
	        case 2:
	           mycnc.resetPos();
	           break;
	        case 3:
	           mycnc.searchHomePos();
	      }

	      end_task = true;
	    }
	  }
}
