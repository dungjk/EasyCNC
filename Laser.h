/*!
 \file      Laser.h
 \brief     The file contains the class which implements the control of laser utensil.
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#ifndef LASER_H_
#define LASER_H_

#include <Arduino.h>
#include <stdint.h>
#include "Utensil.h"

/*! \class Laser
 *  \brief Class to control a the laser utensil.
 *  \details The class allows to control a milling machine.
 *  \author    Francesco Giurlanda
 */
class Laser: public Utensil {
	boolean state; //!< Specifies if the lasers is ON (true) or OFF (false)
	boolean active_high; //!< Specifies if the pin that controls the laser is active highs
	uint8_t control_pin; //!< The pin number that is attached to the laser controller (it could be a PWM channel)

public:
	/*! \brief Constructor
	 *  \param p Pin number
	 *  \param ah True if it is active high
	 */
	Laser(uint8_t p, boolean ah);
	//! \brief It initializes the data structure
	void init();
	//! \brief It switches on the laser
	void switchOn();
	//! brief It switches off the laser
	void SwitchOff();

	void preMotion();
	void postMotion();
};

#endif /* LASER_H_ */
