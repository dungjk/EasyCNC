/*
 * debugger.h
 *
 *  Created on: 27/ott/2014
 *      Author: francescogiurlanda
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
