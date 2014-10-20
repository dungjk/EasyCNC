// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _easy_CNC_H_
#define _easy_CNC_H_
#include "Arduino.h"
//add your includes for the project easy_CNC here

#include "config.h"
#include "Utensils.h"
#include "GCode.h"
#include <Servo.h>
#include "tools/Position.h"
#include "CNC_Router.h"
#include "tools/Timer.h"

//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project easy_CNC here

//PlotterServo mypen(_PLOTTER_SERVO_PIN,_PLOTTER_SERVO_DOWN_POS, _PLOTTER_SERVO_UP_POS);
MillingMachine mill(_MILLING_MACHINE_MZ_STEPS_PER_MM, _MILLING_MACHINE_MZ_FAST_SPEED);

CNC_Router cncrt(ROUTER_MX_STEPS_PER_MM, ROUTER_MY_STEPS_PER_MM, ROUTER_MZ_STEPS_PER_ROUND, ROUTER_MX_SPEED, ROUTER_MY_SPEED, ROUTER_MZ_STEPS_PER_MM);  //200.0 at lowPrecision

GCode gc(&cncrt, &mill);
char new_line[256];

boolean end_task = false;
boolean a, b;
Timer tloop;

void stopButton(){
  cncrt.stopMotion();
}


//Do not add code below this line
#endif /* _easy_CNC_H_ */
