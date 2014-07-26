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
#include <Servo.h>
#include "Position.h"
#include "CNCxy.h"

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

PlotterServo mypen(_PLOTTER_SERVO_PIN,_PLOTTER_SERVO_DOWN_POS, _PLOTTER_SERVO_UP_POS);

CNCxy mycnc(ROUTER_MX_STEPS_PER_ROUND, ROUTER_MY_STEPS_PER_ROUND, ROUTER_MX_SPEED, ROUTER_MY_SPEED);  //200.0 at lowPrecision

boolean end_task = false;


void stopButton(){
  mycnc.stopMotion();
}

void processPos(){
  float new_x, new_y;
  new_x = Serial.parseFloat();
  new_y = Serial.parseFloat();
  Serial.read();
  mycnc.moveTo(new_x, new_y);
}


//Do not add code below this line
#endif /* _easy_CNC_H_ */