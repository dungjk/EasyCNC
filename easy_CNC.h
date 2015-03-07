/*!
 \file      easy_CNC.h
 \brief     Main file.
 \details   Only modify this file to include
			- function definitions (prototypes)
			- include files
			- extern variable definitions
			In the appropriate section
 \author    Francesco Giurlanda
 \version   0.1
 \date      2014

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
            To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#ifndef _easy_CNC_H_
#define _easy_CNC_H_
#include "Arduino.h"

#include "config.h"
#include "GCode/GCode.h"
#include "GCode/GCode_def.h"
#include "utensils/MillingMachine.h"
#include "routers/CNCRouterISR.h"
#include "tools/Position.h"
#include "motor_drivers/MotorDriver.h"
#include "tools/debugger.h"

//#include <>


#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project easy_CNC here

MillingMachine mill(_MILLING_MACHINE_ENABLE_PIN, _MILLING_MACHINE_SPEED_PIN);
CNC_Router_ISR cncrt;

GCode gc(&cncrt, &mill);
char new_line[256];

#endif /* _easy_CNC_H_ */
