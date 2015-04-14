/*!
 \file      debugger.h
 \brief     Macros for debugging purpose
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#ifndef DEBUGGER_H_
#define DEBUGGER_H_

#include "Arduino.h"

//#define _TEST_1

#define DEBUG 1

#define DBG(x) {if(DEBUG) {x;} };

#define DBG_BP(x) {if(DEBUG) { Serial.print("#<"); \
	                         Serial.print(x); \
							 Serial.println(">"); \
							 Serial.flush(); \
							 while(Serial.available() == 0 ) delay(500); \
						     Serial.read();}};

#define DBG_MSG(x) {if(DEBUG){ Serial.print("#["); \
	                 Serial.print(x);  \
	                 Serial.println("]"); \
	                 Serial.flush();}};

#define DBG_VAR(X) { if(DEBUG){ Serial.print("#{"#X" = "); \
	                 Serial.print(X);  \
	                 Serial.println("}"); \
	                 Serial.flush();}};

#define DBG_TEST(x, y) {Serial.print("#(TEST "); \
	                    Serial.print(x); \
	                    Serial.print(": "); \
	                    Serial.println((y)? "OK)" : "FAILED)");};

// Check variables
//DBG( int32_t limit = 1536000;); /*number of steps in 60cm with a motor of 3200 steps per 1.25mm*/


#endif /* DEBUGGER_H_ */
