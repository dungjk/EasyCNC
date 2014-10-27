//  Quest'opera è stata rilasciata con licenza Creative Commons Attribuzione 
//  - Condividi allo stesso modo 4.0 Internazionale. 
//  Per leggere una copia della licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
//
//  Autor: Francesco Giurlanda
//  Project: Easy CNC
//  versione: 0.0.1

#include "config.h"
#include "Utensils.h"
#include "GCode.h"
#include <Servo.h>
#include "tools/Position.h"
#include "CNC_Router.h"
#include "tools/Timer.h"

MillingMachine mill(_MILLING_MACHINE_ENABLE_PIN, _MILLING_MACHINE_SPEED_PIN);

CNC_Router cncrt(ROUTER_MX_STEPS_PER_MM, ROUTER_MY_STEPS_PER_MM, ROUTER_MZ_STEPS_PER_MM, ROUTER_MX_SPEED, ROUTER_MY_SPEED, ROUTER_MZ_SPEED);  //200.0 at lowPrecision

GCode gc(&cncrt, &mill);
char new_line[256];

Timer tloop;

void setup(){
        cncrt.initMotorX();
	cncrt.initMotorY();
	cncrt.initMotorZ();
	cncrt.resetPos();
	cncrt.setMotionModeX(EIGHTH_STEP);
	cncrt.setMotionModeY(EIGHTH_STEP);
	cncrt.setMotionModeZ(EIGHTH_STEP);
	cncrt.setAbsolPos(); gc.last_word[GROUP3] = G90;
	//cncrt.initInterrupts();
	cncrt.orientationX(-1);

	mill.init();
        Serial.begin(SERIAL_BOUND);

}

void loop(){
  if (Serial.available() > 0) {
		//memset(new_line, '\0', 256);
		new_line[Serial.readBytesUntil('\n', new_line, 256)] = '\0';
		gc.line = new_line;
		gc.parseLine();
	}
}

