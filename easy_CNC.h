/*!
 \file      easy_CNC.h
 \brief     Main file.
 \details   Only modify this file to include
			- function definitions (prototypes)
			- include files
			- extern variable definitions
			In the appropriate section
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
            To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#ifndef _easy_CNC_H_
#define _easy_CNC_H_
#include "Arduino.h"

#include "config.h"
#include "GCode.h"
#include "GCode_def.h"
#include "CNCRouterISR.h"
#include "Position.h"
#include "MotorDriver.h"

#ifdef _PLOTTER_SERVO
#include "PlotterServo.h"
#endif
#ifdef _MILLING_MACHINE
#include "MillingMachine.h"
#endif
#ifdef _LASER
#include "Laser.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project easy_CNC here
#ifdef _MILLING_MACHINE
MillingMachine tool(_MILLING_MACHINE_ENABLE_PIN, _MILLING_MACHINE_SPEED_PIN);
#endif
#ifdef _LASER
Laser tool(_LASER_CONTROL_PIN, _LASER_CONTROL_ACTIVE_HIGH);
#endif
#ifdef _PLOTTER_SERVO
PlotterServo tool(_PLOTTER_SERVO_PIN, _PLOTTER_SERVO_UP_POS, _PLOTTER_SERVO_DOWN_POS);
#endif

CNC_Router_ISR cncrt;

GCode gc(&cncrt, &tool);
char new_line[256];

#endif /* _easy_CNC_H_ */
