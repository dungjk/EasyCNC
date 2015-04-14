/*!
 \file      MotorDriver.h
 \brief     Data structure that manages the motor driver.
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
            To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */


#ifndef MOTORDRIVER_H_
#define MOTORDRIVER_H_

#include "Arduino.h"
#include <stdint.h>
#include "P_Info.h"

#define FULL_STEP 1			//!< Identify the full step mode \sa MSMC_A4988::setMode(uint8_t)
#define HALF_STEP 2         //!< Identify the half step mode \sa MSMC_A4988::setMode(uint8_t)
#define QUARTER_STEP 4		//!< Identify the quarter step mode \sa MSMC_A4988::setMode(uint8_t)
#define EIGHTH_STEP 8		//!< Identify the eighth step mode \sa MSMC_A4988::setMode(uint8_t)
#define SIXTEENTH_STEP 16	//!< Identify the sixteenth step mode \sa MSMC_A4988::setMode(uint8_t)

/*! \class MotorDriver
 *  \brief The class is the base class to control a stepper driver with a SED interface (step-enable-direction)
 */
class MotorDriver {
public:
	PinInfo pin_step, /*!< \brief The pin controls the step. */
	pin_dir, /*!< \brief The pin controls the direction. */
	pin_enable; /*!< \brief The pin controls the enable pin of the A4988. */
	int32_t act_steps, /*!< \brief The number of steps until the motion ends. */
	tot_steps; /*!< \brief The total steps set by forward() and backward(). */
	uint8_t control_mode; /*!< \brief Specify the control mode of the motor: 1 full step, 2 half step, 4 quarter step, 8 eighth step and 16 sixteenth step. */
	int8_t orientation; /*!< \brief 1 to actual orientation, -1 to inverted orientation*/
	int8_t direction; /*!< \brief It can be 1 or -1, only at the initialization it is 0*/
	boolean ready; /*!< \brief True if the motor is ready to motion. */
	volatile int32_t global_steps; /*!< \brief The global number of steps from the position 0*/

	//! \brief Default Constructor
	MotorDriver();

	/*! \brief  Constructor
	 *  \param pst The number of the pin which is attached to the step control.
	 *  \param pdr The number of the pin which is attached to the direction control.
	 *  \param pen The number of the pin which is attached to the enable control.
	 */
	MotorDriver(uint8_t pst, uint8_t pdr, uint8_t pen);

	/*! \brief Set up the pins attached to the stepper driver.
	 *  \param pst The number of the pin which is attached to the step control.
	 *  \param pdr The number of the pin which is attached to the direction control.
	 *  \param pen The number of the pin which is attached to the enable control.
	 */
	void setPins(uint8_t pst, uint8_t pdr, uint8_t pen);
};

#endif /* MOTORDRIVER_H_ */
