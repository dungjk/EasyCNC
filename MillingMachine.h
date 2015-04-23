/*!
 \file      MillingMachine.h
 \brief     The file contains the class which implements the control of milling utensils.
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
            To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */
#ifndef MILLINGMACHINE_H_
#define MILLINGMACHINE_H_

#include <Arduino.h>
#include <stdint.h>
#include "Utensil.h"

/*! \class MillingMachine
 *  \brief Class to control a the milling machine.
 *  \details The class allows to control a milling machine. It is possible change the
 *           speed of the drill bit.
 *  \author    Francesco Giurlanda
 */
class MillingMachine: public Utensil {

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

	void preMotion();
	void postMotion();
};


#endif /* MILLINGMACHINE_H_ */
