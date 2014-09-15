/*
 * MSMC_A4988.h
 *
 *  Created on: 17/lug/2014
 *      Author: francescogiurlanda
 */

#ifndef MSMC_A4988_H_
#define MSMC_A4988_H_

#include "Arduino.h"
#include <stdint.h>
#include "MSMC.h"

#define FULL_STEP 1			//!< Identify the full step mode \sa MSMC_A4988::setMode(uint8_t)
#define HALF_STEP 2         //!< Identify the half step mode \sa MSMC_A4988::setMode(uint8_t)
#define QUARTER_STEP 4		//!< Identify the quarter step mode \sa MSMC_A4988::setMode(uint8_t)
#define EIGHTH_STEP 8		//!< Identify the eighth step mode \sa MSMC_A4988::setMode(uint8_t)
#define SIXTEENTH_STEP 16	//!< Identify the sixteenth step mode \sa MSMC_A4988::setMode(uint8_t)

/*! \class MSMC_A4988
 *  \brief Multi Stepper Motor Control with A4988
 *  \details The class allows the contemporary control of multi stepper motors using the ICs A4988. After the motor setup, you can drive the motor forward and backward,
 *           specifying the number of steps and the delay in microseconds between two consecutive steps. The function update performs the effective motor motion, it must be call
 *           in the main loop and, if the delay is expired, the function moves the motor of one step.
 *  \author Francesco Giurlanda
 */
class MSMC_A4988: public MSMC {
	uint8_t pin_step, /*!< \brief The pin controls the step. */
	pin_dir, /*!< \brief The pin controls the direction. */
	pin_enable; /*!< \brief The pin controls the enable pin of the A4988. */
	uint32_t spd; /*!< \brief The delay between two steps in microseconds. */
	uint32_t steps, /*!< \brief The number of steps until the motion ends. */
	tot_steps, /*!< \brief The total steps set by forward() and backward(). */
	old_time, /*!< \brief The time value in microseconds set during the last step. */
	pause_time; /*!< \brief The time value in microseconds set by the function MSMC_ULN2003A::pause(). */
	int8_t dir; /*!< \brief Specify the direction of the motor: -1 backward, 1 forward, 0 all pin out LOW */
	boolean m_ready; /*!< \brief True if the motor is ready to take a new command: forward(uint32_t, uint32_t) or backward(uint32_t, uint32_t). */
	boolean step_pin_val; /*!< \brief The value of the pin that drives the step advance */
	uint8_t control_mode; /*!< \brief Specify the control mode of the motor: 1 full step, 2 half step, 4 quarter step, 8 eighth step and 16 sixteenth step. */
	boolean m_pause; /*!< \brief It is True if the motion is paused. */
	int8_t dir_mode; /*!< Specifies if the motion is direct or inverted: 1 direct and -1 inverted*/


	//! \brief It drives the direction pin
	void setDirection();

	//! \brief Enable the FETs of the A4988 that control the coils of the motor, the pin is low active..
	void enable();

	//! \brief Disable the FETs of the A4988 that control the coils of the motor.
	void disable();

public:
	//! \brief Default constructor.
	MSMC_A4988();

	/*! \brief Constructor
	 *  \param pst The number of the pin which is attached to the step control pin of the A4988.
	 *  \param pdr The number of the pin which is attached to the direction control pin of the A4988.
	 *  \param pen The number of the pin which is attached to the enable pin of the A4988.
	 */
	MSMC_A4988(uint8_t pst, uint8_t pdr, uint8_t pen);

	/*! \brief Configure the motor to move forward
	 \details The function moves forward the motor of a number of steps with a specified delay between each step
	 \param st The number of steps
	 \param sp The delay between the steps in microseconds
	 \sa backward(uint32_t, uint32_t)
	 */
	void forward(uint32_t st, uint32_t sp);

	/*! \brief Configure the motor to move backward
	 \details The function moves backward the motor of a number of steps with a specified delay between each step
	 \param st The number of steps
	 \param sp The delay between the steps in microseconds
	 \sa forward(uint32_t, uint32_t)
	 */
	void backward(uint32_t st, uint32_t sp);

	/*! \brief Set up the pins attached to the A4988.
	 *  \param pst The number of the pin which is attached to the step control pin of the A4988.
	 *  \param pdr The number of the pin which is attached to the direction control pin of the A4988.
	 *  \param pen The number of the pin which is attached to the enable pin of the A4988.
	 */
	void setPins(uint8_t pst, uint8_t pdr, uint8_t pen);

	/*! \brief Set up the control mode.
	 *  \param mod The mode used to control the motor. You can use the following macros:
	 *           - #FULL_STEP
	 *           - #HALF_STEP
	 *           - #QUARTER_STEP
	 *           - #EIGHTH_STEP
	 *           - #SIXTEENTH_STEP
	 */
	void setMode(uint8_t mod);

	/*! \brief Return the operative mode of the controller
	 *  \return An int that identifies the operative mode: #FULL_STEP, #HALF_STEP, #QUARTER_STEP, #EIGHTH_STEP and #SIXTEENTH_STEP
	 */
	uint8_t getMode();

	/*! \brief Return the direction
	 *  \return 1 = forward, -1 = backward, 0 = none
	 */
	int8_t getDir();

	/*! \brief Change the delay between each step
	 \details This function can be used to change the speed during an operation of forward or backward.
	 \param sp the delay in microseconds.
	 \sa forward(uint32_t, uint32_t), backward(uint32_t, uint32_t)
	 */
	void speed(uint32_t sp);

	//! Stop any motion.
	void stop();

	//! Pause the motion. \sa MSMC_A4988::restart()
	void pause();

	//! Restart the motion after the call of the function MSMC_A4988::pause()
	void restart();

	/*! \brief Perform the motion of the motor
	 \details The function performs effectively the motor motion.
	 It verifies that the specified delay from the last step is expired and than moves the motor of one step.
	 The function must be call in the main loop with a frequency greater than 1/(spd/1000000).
	 Greater is the frequency to call the function and higher is the precision of the the motion.
	 \return the function returns the number of steps made, up to the end of the motion.
	 If the motion end the motor is idle and it can perform a new command.
	 When the motor is idle the function returns -1.
	 */
	int32_t update();

	/*! \brief It sets the direction mode.
	 *  \param m The direction mode: 1 direct, -1 inverted.
	 */
	void dirMode(int8_t m);

};

#endif /* MSMCA4988_H_ */
