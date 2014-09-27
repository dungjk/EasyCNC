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

#if defined(_MILLING_MACHINE_MZ_CONTROLLER_ULN2003A)
	MSMC_ULN2003A mz;
#elif defined(_MILLING_MACHINE_MZ_CONTROLLER_A4988)
	MSMC_A4988 mz;
#endif

	float old_p, //!< The old position in mm
			end_p, //!< The end position in mm
			actual_p; //!< The actual position in mm
	float bit_speed; //!< The speed of the drill bit
	boolean bit_dir; //!< The direction of the drill bit: CW = true  or CCW = false
	float spmm; //!< Number of steps to move of a mm the milling machine on the z-axis (steps/mm)
	float v_max_z; //!< The max speed of the milling machine on the z-axis (steps/s)
	boolean pos_type; //!< The type of the position info: true = incremental; false = absolute.
	int8_t pin_ls_z_down, //!< The pin number connected to the down limit swtch
			pin_ls_z_up; //!< The pin number connected to the up limit swtch

public:
	//! \brief The constructor.
	MillingMachine();
	/*! \brief The constructor with arguments.
	 *  \param s The number of steps to move forward/backward of one mm.
	 *  \param vmax The max speed of the motor, specified as delay between to steps (us).
	 *  \param vmin The min speed of the motor, specified as delay between to steps (us).
	 */
	MillingMachine(float s, float vmax);
	//! \brief It initializes the motor.
	void init();
	//! \brief The function returns the position of the utensil in the Z-axis
	float getPos();
	//! \brief It sets the current position with the value zero.
	void resetPos();
	/*! \brief The function moves the utensil in a specified position with fast speed.
	 *  \param p The position in mm, it can be positive or negative.
	 *  \param s The speed of motion of the Z-axis (mm/s)
	 */
	void moveTo(float p, float s = 0.0);
	//! \brief The function stops the motion of the utensil.
	void stopMotion();
	//! \brief The function pauses the current operation.
	void pause();
	//! \brief The function restarts an operation that was stopped.
	void restart();
	//! \brief It sets the motion to be more precise.
	void highPrecision();
	//! \brief It sets the motion to be less precise.
	void lowPrecision();
	//! \brief It sets the mode to specify the position: incremental position.
	void setIncrPos();
	//! \brief It sets the mode to specify the position: absolute position.
	void setAbsolPos();
	/*! \brief The procedure search the zero position.
	 *  \details It is a procedure that require an aluminum foil (you can find it in any market) and a alligator clip.
	 *           The two component are connected to two copper wire that are connected respectively to GND and the pin specified for the down limit switch.
	 *           Before to call the function, you place the foil on the working table under the drilling bit and attach the alligator clip to the drill bit.
	 *           The function moves down the utensil until the drill bit touches the aluminum foil. It stops the operation.
	 */

	void searchZeroPos();
	/*! \brief Sets the pins where are attached the down/up limit switches
	 *  \details The system needs at least the down limit switch
	 *  \param dw The pin number where is attached the down limit switch.
	 *  \param up The pin number where is attached the up limit switch
	 */
	void setLimitSwitch(int8_t dw, int8_t up = -1);
	/*! \brief It sets the bit speed.
	 *  \param s The value of the speed.
	 */
	void setBitSpeed(int s);
	/*! \brief It sets the bit direction
	 *  \param d CW = true  or CCW = false.
	 */
	void setBitDir(boolean d);
	//! \brief The function performs the motor motion. It must be called with a frequency greater than 1/(v/1000000) where v is the maximum speed of the motor specified as the minimum delay in microseconds between two consecutive steps.
	boolean update();
};

#endif

#endif
