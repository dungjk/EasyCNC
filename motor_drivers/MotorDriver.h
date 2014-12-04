/*
 * MotorDriver.h
 *
 *  Created on: 29/nov/2014
 *      Author: francescogiurlanda
 */

#ifndef MOTORDRIVER_H_
#define MOTORDRIVER_H_

#include "Arduino.h"
#include <stdint.h>

#define FULL_STEP 1			//!< Identify the full step mode \sa MSMC_A4988::setMode(uint8_t)
#define HALF_STEP 2         //!< Identify the half step mode \sa MSMC_A4988::setMode(uint8_t)
#define QUARTER_STEP 4		//!< Identify the quarter step mode \sa MSMC_A4988::setMode(uint8_t)
#define EIGHTH_STEP 8		//!< Identify the eighth step mode \sa MSMC_A4988::setMode(uint8_t)
#define SIXTEENTH_STEP 16	//!< Identify the sixteenth step mode \sa MSMC_A4988::setMode(uint8_t)

/*! \struct PinInfo
 *  \brief It keeps the info about the register of a specified pin
 */
struct P_Info {
	uint8_t bit;  //!< The bit mask of the pin in the register.
	volatile uint8_t *outreg; //!< The output register of the specified pin.

};

typedef struct P_Info PinInfo;

/*! \class MotorDriver
 *  \brief The class is the base class to control a stepper driver with a SED interface (step-enable-direction)
 */
class MotorDriver {
public:
	PinInfo pin_step, /*!< \brief The pin controls the step. */
	pin_dir, /*!< \brief The pin controls the direction. */
	pin_enable; /*!< \brief The pin controls the enable pin of the A4988. */
	uint32_t act_steps, /*!< \brief The number of steps until the motion ends. */
	tot_steps; /*!< \brief The total steps set by forward() and backward(). */
	uint8_t control_mode; /*!< \brief Specify the control mode of the motor: 1 full step, 2 half step, 4 quarter step, 8 eighth step and 16 sixteenth step. */
	int8_t orientation; /*!< \brief 1 to actual orientation, -1 to inverted orientation*/
	boolean ready; /*!< \brief True if the motor is ready to motion. */

	//! \brief Default Constructor

	MotorDriver();

	/*! \brief  Constructor
	 * \param pst The number of the pin which is attached to the step control.
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
