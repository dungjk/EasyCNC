//  Quest'opera è stata rilasciata con licenza Creative Commons Attribuzione 
//  - Condividi allo stesso modo 4.0 Internazionale. 
//  Per leggere una copia della licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
//
//  Autor: Francesco Giurlanda
//  Project: Easy CNC
//  versione: 0.1

#include "config.h"
#include "GCode.h"
#include "GCode_def.h"
#include "MillingMachine.h"
#include "CNCRouterISR.h"
#include "Position.h"
#include "MotorDriver.h"
#include "debugger.h"

MillingMachine mill(_MILLING_MACHINE_ENABLE_PIN, _MILLING_MACHINE_SPEED_PIN);
CNC_Router_ISR cncrt;

GCode gc(&cncrt, &mill);
char new_line[256];

void setup(){
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

void loop(){
  if (Serial.available() > 0) {
		new_line[Serial.readBytesUntil('\n', new_line, 256)] = '\0';
		gc.line = new_line;
		gc.parseLine();
		gc.sendAck();
	}
}

