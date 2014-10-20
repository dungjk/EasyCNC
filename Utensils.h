/*!
 \file      Utensils.h
 \brief     The file contains the classes which implement the control of different utensils.
 \details   The classes in this file allow to control different utensils,
 such as the head of a plotter, a spindle with a cutter bit, a blade to cut a paper sheet and so on.
 \author    Francesco Giurlanda
 \version   0.0.1
 \date      2014

 \copyright Quest'opera è stata rilasciata con licenza Creative Commons Attribuzione
 - Condividi allo stesso modo 4.0 Internazionale. Per leggere una copia della
 licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
 */

#ifndef Utensils_h
#define Utensils_h

#include "config.h"

// ******************************
// ***** PLOTTER WHIT SERVO *****
// ******************************

#ifdef _PLOTTER_SERVO

#include <Servo.h>
#include <Arduino.h>

#include <stdint.h>

#define DOWN true  //!< \brief The state for the plotter head down
#define UP false   //!< \brief The state for the plotter head up

/*! \class PlotterServo
 *  \brief Class to control a plotter head.
 *  \details The class allows to control an utensil with a per or a pencil or a marker pen,
 *           moved up and down by a servo motor. This utensil requires a PWM signal which controls the servo.
 *  \author    Francesco Giurlanda
 */
class PlotterServo {
	boolean state;              //!< The state of the plotter head \sa DOWN, UP
	uint8_t pin;//!< The pin where is attached the servo motor
	Servo servo_pen;//!< The object which control the servo motor
	uint8_t down_val,//!< The position of the servo for the plotter head down (deg)
	up_val;//!< The position of the servo for the plotter head up (deg)

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
};

#endif

// ***********************
// *** MILLING MACHINE ***
// ***********************

#ifdef _MILLING_MACHINE

#include <Arduino.h>
#include <stdint.h>

#if defined(_MILLING_MACHINE_MZ_CONTROLLER_ULN2003A)
#include "MSMC_ULN2003A.h"
#endif
#if defined(_MILLING_MACHINE_MZ_CONTROLLER_A4988)
#include "MSMC_A4988.h"
#endif

/*! \class MillingMachine
 *  \brief Class to control a the milling machine and its position on the z-axis.
 *  \details The class allows to control a milling machine. It is possible change the position on the z-axis and the
 *           speed of the drill bit.
 *  \author    Francesco Giurlanda
 */

class MillingMachine {

	uint8_t bit_speed; //!< The speed of the drill bit
	boolean bit_dir; //!< The direction of the drill bit: CW = true  or CCW = false
	boolean state; //!< Specifies if the spindle is ON (true) or OFF (false)
	uint8_t control_pin; //!< The pin number that is attached to the spindle speed controller (it must be a PWM channel)
	uint8_t enable_pin; //!< The pin number that controls the switch on/off of the spindle
public:
	/*! \brief The constructor.
	 *  \param e The pin number that controls whether the spindle is on or off.
	 *  \param c The pin number that controls the speed of the spindle (PWM).
	 */
	MillingMachine(uint8_t e, uint8_t c);

	//! \brief It initializes the spindle.
	void init();

	/*! \brief It sets the spindle speed.
	 *  \param s The value of the speed.
	 */
	void setSpindleSpeed(int s);

	/*! \brief It sets the spindle direction
	 *  \param d CW = true  or CCW = false.
	 */
	void setSpindleDir(boolean d);

	//! \brief It switches on the spindle.
	void enable();

	//! \brief It switches off the spindle.
	void disable();

};

#endif

#endif
