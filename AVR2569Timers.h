/*!
 \file      AVR2569Timers.h
 \brief     Macros to configure and uses the AVR2560 timers in CTC mode
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#ifndef AVR2569TIMERS_H_
#define AVR2569TIMERS_H_

#define CS_1     (0x01 << CS10) 					//<! It is the bit set to configure the timer clock with no prescaler. Only on timer 1, 3, 4 and 5
#define CS_8     (0x01 << CS11) 					//<! It is the bit set to configure the timer clock with 8 prescaler. Only on timer 1, 3, 4 and 5
#define CS_64    ((0x01 << CS11) | (0x01 << CS10)) 	//<! It is the bit set to configure the timer clock with 64 prescaler. Only on timer 1, 3, 4 and 5
#define CS_256   (0x01 << CS12) 					//<! It is the bit set to configure the timer clock with 256 prescaler. Only on timer 1, 3, 4 and 5
#define CS_1024  ((0x01 << CS12) | (0x01 << CS10)) 	//<! It is the bit set to configure the timer clock with 1024 prescaler. Only on timer 1, 3, 4 and 5

/**************************
 *  	TIMER 1
 **************************/

/*! \def INIT_TIMER1
 *  \details Below the description of the operations performed by the macro:
 *           - TCCR1A = 0;  set entire TCCR1A register to 0
 *           - TCCR1B = 0;  set entire TCCR1B register to 0
 *           - TCNT1 = 0;   initialize counter value to B0
 *			 - TCCR1B |= (1 << WGM12); turn on CTC mode
 */
#define INIT_TIMER1 {	TCCR1A = 0;    \
	             	 	TCCR1B = 0;    \
	             	 	TCNT1 = 0;   \
	             	 	TIMSK1 |= (0x01 << OCIE1A);    \
	             	 	TCCR31 |= (0x01 << WGM12); }

/*! \def SET_TIMER1
 *  \details It sets the value of the Output Compare Register A of the timer 1.
 *  \param x The value of the register
 */
#define SET_TIMER1(x) { OCR1A = x;}

/*! \def CLEAR_COUNTER_TIMER1
 *  \details Set to 0 the TCNT1 register.
 */
#define CLEAR_COUNTER_TIMER1 { TCNT1 = 0; }

/*! \def START_TIMER1
 *  \details The macro runs the timer 1 with a prescaler that can be CS_1, CS_8, CS_64, CS_256, CS_1024.
 *           - TIMSK1 |= (1 << OCIE1A); turn on CTC mode
 *           - TCCR1B |= x; set the prescaler
 */
#define START_TIMER1(x) {  TCNT1 = 0;  \
						TCCR1B |= x;}
/*! \def STOP_TIMER1
 *  \details The macro stops the timer 1.
 */
#define STOP_TIMER1 {   TCCR1B &= ~((0x01 << CS10) | (0x01 << CS11) | (0x01 << CS12));}

/**************************
 *  	TIMER 3
 **************************/

/*! \def INIT_TIMER3
 *  \details Below the description of the operations performed by the macro:
 *           - TCCR3A = 0;  set entire TCCR3A register to 0
 *           - TCCR3B = 0;  set entire TCCR3B register to 0
 *           - TCNT3 = 0;   initialize counter value to B0
 *			 - TCCR3B |= (1 << WGM32); turn on CTC mode
 */
#define INIT_TIMER3 {	TCCR3A = 0;    \
	             	 	TCCR3B = 0;    \
	             	 	TCNT3 = 0;   \
	             	 	TIMSK3 |= (0x01 << OCIE3A);    \
	             	 	TCCR3B |= (0x01 << WGM32); }

/*! \def SET_TIMER3
 *  \details It sets the value of the Output Compare Register A of the timer 3.
 *  \param x The value of the register
 */
#define SET_TIMER3(x) { OCR3A = x;}

/*! \def CLEAR_COUNTER_TIMER3
 *  \details Set to 0 the TCNT3 register.
 */
#define CLEAR_COUNTER_TIMER3 { TCNT3 = 0; }

/*! \def START_TIMER3
 *  \details The macro runs the timer 3 with a prescaler that can be CS_1, CS_8, CS_64, CS_256, CS_1024.
 *           - TIMSK3 |= (1 << OCIE3A); turn on CTC mode
 *           - TCCR3B |= x; set the prescaler
 */
#define START_TIMER3(x) {  TCNT3 = 0;  \
						TCCR3B |= x;}
/*! \def STOP_TIMER3
 *  \details The macro stops the timer 3.
 */
#define STOP_TIMER3 {   TCCR3B &= ~((0x01 << CS30) | (0x01 << CS31) | (0x01 << CS32));}


/**************************
 *  	TIMER 4
 **************************/

/*! \def INIT_TIMER4
 *  \details Below the description of the operations performed by the macro:
 *           - TCCR4A = 0;  set entire TCCR4A register to 0
 *           - TCCR4B = 0;  set entire TCCR4B register to 0
 *           - TCNT4 = 0;   initialize counter value to B0
 *			 - TCCR4B |= (1 << WGM42); turn on CTC mode
 */
#define INIT_TIMER4 {	TCCR4A = 0;  \
	             	 	TCCR4B = 0;     \
	             	 	TCNT4 = 0;  \
	             	 	TIMSK4 |= (0x01 << OCIE4A); \
	             	 	TCCR4B |= (0x01 << WGM42);}
/*! \def SET_TIMER4
 *  \details It sets the value of the Output Compare Register A of the timer 4.
 *  \param x The value of the register
 */
#define SET_TIMER4(x) { OCR4A = x;}

/*! \def CLEAR_COUNTER_TIMER4
 *  \details Set to 0 the TCNT4 register.
 */
#define CLEAR_COUNTER_TIMER4 { TCNT4 = 0; }

/*! \def START_TIMER4
 *  \details The macro runs the timer 4 with a prescaler that can be CS_1, CS_8, CS_64, CS_256, CS_1024.
 *           - TIMSK4 |= (0x01 << OCIE4A); turn on CTC mode
 *           - TCCR4B |= x; set the prescaler
 */
#define START_TIMER4(x) {  TCNT4 = 0;  \
						TCCR4B |= x;}

/*! \def STOP_TIMER4
 *  \details The macro stops the timer 4.
 */
#define STOP_TIMER4 {	TCCR4B &= ~((0x01 << CS40) | (0x01 << CS41) | (0x01 << CS42));}

/**************************
 *  	TIMER 5
 **************************/

/*! \def INIT_TIMER5
 *  \details Below the description of the operations performed by the macro:
 *           - TCCR5A = 0;  set entire TCCR5A register to 0
 *           - TCCR5B = 0;  set entire TCCR5B register to 0
 *           - TCNT5 = 0;   initialize counter value to B0
 *			 - TCCR5B |= (1 << WGM52); turn on CTC mode
 */
#define INIT_TIMER5 {	TCCR5A = 0;  \
	             	 	TCCR5B = 0;     \
	             	 	TCNT5 = 0;  \
	             	 	TIMSK5 |= (0x01 << OCIE5A); \
	             	 	TCCR5B |= (0x01 << WGM52);}
/*! \def SET_TIMER5
 *  \details It sets the value of the Output Compare Register A of the timer 5.
 *  \param x The value of the register
 */
#define SET_TIMER5(x) { OCR5A = x;}

/*! \def CLEAR_COUNTER_TIMER5
 *  \details Set to 0 the TCNT5 register.
 */
#define CLEAR_COUNTER_TIMER5 { TCNT5 = 0; }

/*! \def START_TIMER5
 *  \details The macro runs the timer 5 with a prescaler that can be CS_1, CS_8, CS_64, CS_256, CS_1024.
 *           - TIMSK5 |= (0x01 << OCIE5A); turn on CTC mode
 *           - TCCR5B |= x; set the prescaler
 */
#define START_TIMER5(x) {  TCNT5 = 0;  \
						TCCR5B |= x;}

/*! \def STOP_TIMER5
 *  \details The macro stops the timer 5.
 */
#define STOP_TIMER5 {	TCCR5B &= ~((0x01 << CS50) | (0x01 << CS51) | (0x01 << CS52));}


#endif /* AVR2569TIMERS_H_ */
