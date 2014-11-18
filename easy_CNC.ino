//  Quest'opera è stata rilasciata con licenza Creative Commons Attribuzione 
//  - Condividi allo stesso modo 4.0 Internazionale. 
//  Per leggere una copia della licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
//
//  Autor: Francesco Giurlanda
//  Project: Easy CNC
//  versione: 0.0.1

#include "config.h"
#include "GCode/GCode.h"


MillingMachine mill(_MILLING_MACHINE_ENABLE_PIN, _MILLING_MACHINE_SPEED_PIN);

CNC_Router cncrt(ROUTER_MX_STEPS_PER_MM, ROUTER_MY_STEPS_PER_MM, ROUTER_MZ_STEPS_PER_MM, ROUTER_MX_SPEED, ROUTER_MY_SPEED, ROUTER_MZ_SPEED);  //200.0 at lowPrecision

GCode gc(&cncrt, &mill);
char new_line[256];

void setup(){
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

void loop(){
  if (Serial.available() > 0) {
		new_line[Serial.readBytesUntil('\n', new_line, 256)] = '\0';
		gc.line = new_line;
		gc.parseLine();
	}
}

