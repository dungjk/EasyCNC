/*!
 \file MotionPerformer.h
 \brief Implementation of the class MotionPerformer.
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015
 \warning   If you change Arduino board some parameters could change too.
 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#ifndef MOTIONPERFORMER_H_
#define MOTIONPERFORMER_H_

#include "MotorDriver.h"
#include "MotionPlanner.h"
#include "arduino.h"

#define DELAY_STEP_UP 39        //!<  ticks of timer4 with 8 prescaler to create a valid pulse on the step pin  ~ 20us
#define IDLE_FREQ 3125          //!<  Value of OCR3A, it is the interval to check the motion planner when it is empty ~ 200ms

/*! \class MotionPerformer
 *  \brief The class manages the motors movements.
 *  \details The class manages the motors movements, it is binded to a
 *            MotionPlanner object that keeps all the linear movements.
 *  \author Francesco Giurlanda
 */
class MotionPerformer {
	int32_t d1, d2; //<! \brief Variables used by  Bresenham algorithm.
	MotorDriver *motor[3]; //<! \brief Pointers to the motor drivers, they are ordered according the Bresenham algorithm. The first element stores the driver of the motor of the main axis, the others store the secondary axes.
	boolean idle; //<! \brief It is false if the MotionPerformer is already working, true if the motion performer does not working.

	/*! \brief The function loads a new linear motion from the MotionPlanner.
	 * 	\details The function should be run with the interrupt flag active.
	 * 	\return It returns a boolean that is true if the function can not load
	 * 	        a new linear motion, false otherwise. */
	boolean loadMotion();

	//! \brief It disables the motors.
	void disableMotors();
	//! \brief It enables the motors.
	void enableMotors();

public:

	float current_fr; //!< It is updated with the feed rate of the last linear motion that was loaded.
	MotorDriver mx;
	MotorDriver my;
	MotorDriver mz;
	MotionPlanner *planner;

	/*! \brief Constructor
	 *  \param mp It is the pointer to the MotionPlanner object.
	 */
	MotionPerformer(MotionPlanner *mp);
	//! \brief It initializes the motion performer and the timers.
	void init();

	//void setIntNextStep();
	//void setIntStepDown();
	//boolean loadMotion();

	//! \brief It starts the motion. It is used after a stopMotion() or pauseMotion().
	void startMotion();
	/*! \brief It stops the motion.
	 *  \details It works on the timer3. Moreover, it puts to zero the actual and total steps variables of the motors*/
	void stopMotion();
	/*! \brief It pauses the motion.
	 *  \brief It works on the timer3.
	 */
	void pauseMotion();
	//! \brief It restarts the motion. It is used after a stopMotion() or pauseMotion().
	void restartMotion();
	/*! \brief It is the core function that performs the steps forward of the motors and
	 *         checks the MotionPlanner for a new linear motion.
	 *  \details The function is called by ISR(TIMER3_COMPA_vect) on the timer 3.
	 * 			 If the motion performer is idle, the function runs with a constant frequency DELAY_STEP_UP
	 * 			 to check the motion planner for a new linear motion. If the new linear motion is loaded, the
	 * 			 function runs with a frequency configured by that linear motion and forwards
	 * 			 the motor with a coordinated motion.*/
	void update();
	/*! \brief The function performs the step pulse down
	 *  \details The function is binded to the timer 4, it is called by ISR(TIMER4_COMPA_vect) and it has the
	 *           step pin go down. The timer 4 is activated by update() and after a value defined in IDLE_FREQ
	 *           the function is triggered.
	 */
	void stepPulse();
	/*! \brief The function return true if the motion planner is not working.
	 *  \return True if it is not working, false otherwise.	 */
	bool isNotWorking();

};

#endif /* MOTIONPERFORMER_H_ */
