/*!
 \file      PlotterServo.h
 \brief     The file contains the classes which implement the control of an utensil composed by a pen which up/down motion is controlled by a servo motor.
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#ifndef PLOTTERSERVO_H_
#define PLOTTERSERVO_H_

#include <Arduino.h>
#include "SoftwareServo.h"

#include <stdint.h>
#include "Utensil.h"

#define PEN_DOWN true  //!< \brief The state for the plotter head down
#define PEN_UP false   //!< \brief The state for the plotter head up
#define SERVO_REFRESH  750 //!< \brief The number of tick with 1024 prescaler of the timer 1 to allow at least a refresh each 50ms (see SoftwareServo documentation).

/*! \class PlotterServo
 *  \brief Class to control a plotter head.
 *  \details The class allows to control an utensil with a per or a pencil or a marker pen,
 *           moved up and down by a servo motor. This utensil requires a PWM signal which controls the servo.
 *  \author    Francesco Giurlanda
 */
class PlotterServo: public Utensil {
	boolean state;              //!< The state of the plotter head \sa PEN_DOWN, PEN_UP
	uint8_t servo_pin;              //!< The pin where is attached the servo motor
	SoftwareServo servo_pen;              //!< The object which control the servo motor
	uint8_t down_val, //!< The position of the servo for the plotter head down (deg)
			up_val; //!< The position of the servo for the plotter head up (deg)

public:

	/*! \brief Constructor
	 *  \param p The number of the pin where is attached the servo.
	 *  \param dv The value for the plotter head down.
	 *  \param uv The value for the plotter head up.
	 */
	PlotterServo(uint8_t p, uint8_t dv, uint8_t uv);

	//! \brief Initialize the object. It must be call into the setup().
	void init();
	//! \brief It moves the head up.
	boolean up();
	//! \brief It moves the head down.
	boolean down();

	void preMotion();
	void postMotion();
};

#endif /* PLOTTERSERVO_H_ */
