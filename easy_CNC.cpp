/*!
 \file      easy_CNC.cpp
 \brief     Main file.
 \author    Francesco Giurlanda
 \version   0.1
 \date      2014

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#include "easy_CNC.h"

#define LEDPIN 13

void (*fun)() = NULL;

void myfun() {
	digitalWrite(LEDPIN, HIGH);
	cli();
	OCR3A = (OCR3A > 400)? OCR3A * 0.6: 60000;
	TCCR4B |= (1 << CS40) | (1 << CS42);
	sei();
}

//The setup function is called once at startup of the sketch
void setup() {
#ifndef _TEST_1

// Add your initialization code here
	cncrt.initMotorX();
	cncrt.initMotorY();
	cncrt.initMotorZ();
	cncrt.resetPos();
	cncrt.setMotionModeX(QUARTER_STEP);
	cncrt.setMotionModeY(QUARTER_STEP);
	cncrt.setMotionModeZ(QUARTER_STEP);
	cncrt.setAbsolPos(); gc.last_word[GROUP3] = G90;
	cncrt.initInterrupts();
	cncrt.orientationX(-1);

	mill.init();

//attachInterrupt(INTERRUPT_STOP_MOTION, stopButton, FALLING);
//digitalWrite(INTERRUPT_STOP_MOTION, HIGH);

	Serial.begin(SERIAL_BOUND);
#else
	pinMode(LEDPIN, OUTPUT);
	uint8_t oldSREG = SREG;
	cli();
	TCCR3A = 0;     // set entire TCCR3A register to 0
	TCCR3B = 0;     // same for TCCR3
	TCNT3 = 0;     //initialize counter value to 0B
	// set compare match register for 1hz increments
	OCR3A = 60000;				// = (16*10^6) / (1*1024) - 1 (must be <65536)

	TCCR3B |= (1 << WGM32); // turn on CTC mode
	TCCR3B |= (1 << CS30);
	TCCR3B |= (1 << CS32);
	// enable timer compare interrupt:
	TIMSK3 |= (1 << OCIE3A);

	// disable global interrupts
	TCCR4A = 0;     // set entire TCCR1A register to 0
	TCCR4B = 0;     // same for TCCR1B

	// set compare match register to desired timer count:
	OCR4A = 224;
	// turn on CTC mode:
	TCCR4B |= (1 << WGM42);
	// Set CS10 and CS12 bits for 1024 prescaler:
	//TCCR4B |= (1 << CS40);
	//TCCR4B |= (1 << CS42);
	// enable timer compare interrupt:
	TIMSK4 |= (1 << OCIE4A);

	sei();
#endif

}

ISR(TIMER3_COMPA_vect) {
	if (fun != NULL)
		fun();
}

ISR(TIMER4_COMPA_vect) {
	digitalWrite(LEDPIN, LOW);
		cli();
		TCCR4B &= ~((1 << CS40) | (1 << CS42));
		sei();
}

// The loop function is called in an endless loop
void loop() {
#ifndef _TEST_1
	if (Serial.available() > 0) {
		new_line[Serial.readBytesUntil('\n', new_line, 256)] = '\0';
		gc.line = new_line;
		gc.parseLine();
	}
#else
	delay(2000);
	fun = myfun;
#endif
}
