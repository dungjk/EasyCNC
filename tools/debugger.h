/*!
 \file      debugger.h
 \brief     Macros for debugging purpose
 \author    Francesco Giurlanda
 \version   0.1
 \date      2014

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
            To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#ifndef DEBUGGER_H_
#define DEBUGGER_H_

#include "Arduino.h"

#define _TEST_1

#define DEBUG 0

#define DBG_BP(x) {if(DEBUG) { Serial.print("#<"); \
	                         Serial.print(x); \
							 Serial.println(">"); \
							 while(Serial.available() == 0 ); \
						     Serial.read();}};

#define DBG_MSG(x) {if(DEBUG){ Serial.print("#["); \
	                 Serial.print(x);  \
	                 Serial.println("]");}};

#define DBG_VAR(x, y) { if(DEBUG){ Serial.print("#{"); \
	                 Serial.print(x);  \
	                 Serial.print(":");  \
	                 Serial.print(y);  \
	                 Serial.println("}");}};





#endif /* DEBUGGER_H_ */
