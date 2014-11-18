/*!
 \file CNC_Router.h
 \brief The file contains the class which controls the x y z router.
 \author    Francesco Giurlanda
 \version   0.1
 \date      2014
 \warning   If you change Arduino board some parameters could change too.
 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
            To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#ifndef CNC_Router_h
#define CNC_Router_h

#include "../config.h"
#include "Arduino.h"
#include "../tools/Position.h"
#include "../tools/utility.h"
#include <stdint.h>

#if defined(ROUTER_MX_CONTROLLER_ULN2003A) || defined(ROUTER_MY_CONTROLLER_ULN2003A)
#include "../motor_drivers/MSMC_ULN2003A.h"
#endif
#if defined(ROUTER_MX_CONTROLLER_A4988) || defined(ROUTER_MY_CONTROLLER_A4988)
#include "../motor_drivers/MSMC_A4988.h"
#endif

/*! \class CNC_Router
 *  \brief The class controls the motors of the xy router of the CNC.
 *  \details The class manages the motion control of the XY router.
 *  \author Francesco Giurlanda
 */
class CNC_Router {

#if defined(ROUTER_MX_CONTROLLER_ULN2003A)
	MSMC_ULN2003A mx;
#elif defined(ROUTER_MX_CONTROLLER_A4988)
	MSMC_A4988 mx;
#endif

#if defined(ROUTER_MY_CONTROLLER_ULN2003A)
	MSMC_ULN2003A my;
#elif defined(ROUTER_MY_CONTROLLER_A4988)
	MSMC_A4988 my;
#endif

#if defined(ROUTER_MZ_CONTROLLER_ULN2003A)
	MSMC_ULN2003A mz;
#elif defined(ROUTER_MZ_CONTROLLER_A4988)
	MSMC_A4988 mz;
#endif

	PositionXYZ old_p, //!< It keeps the position that the utensil has after starting the motion
			end_p, 					//!< The final position of a motion
			actual_p; //!< It keeps the actual position of the utensil during an motion
	float spmmx, //!< Steps to move of a mm in the X-axis (steps/mm). \sa config.h
			spmmy, //!< Steps to move of a mm in the Y-axis (steps/mm). \sa config.h
			spmmz; //!< Steps to move of a mm in the Z-axis (steps/mm). \sa config.h
	float v_max_x, //!< The max speed for the motor that controls the X-axis (steps/s). \sa config.h
			v_max_y, //!< The max speed for the motor that controls the Y-axis (steps/s). \sa config.h
			v_max_z; //!< The max speed for the motor that controls the Y-axis (steps/s). \sa config.h

	boolean pos_type; //!< It stores the information about the positioning type: true = incremental; false = absolute.

	float round_off_x,  //!< It contains the difference of numerical rounding to compute the number of step in X-axis
	      round_off_y,	//!< It contains the difference of numerical rounding to compute the number of step in Y-axis
	      round_off_z;  //!< It contains the difference of numerical rounding to compute the number of step in Z-axis

public:
	volatile static boolean ls_x_down, //!< State of the down limit switch of X-axis: true = not triggered, false = triggered
			ls_x_up, //!< State of the up limit switch of X-axis: true = not triggered, false = triggered
			ls_y_down, //!< State of the down limit switch of Y-axis: true = not triggered, false = triggered
			ls_y_up, //!< State of the up limit switch of Y-axis: true = not triggered, false = triggered
			ls_z_down, //!< State of the down limit switch of Z-axis: true = not triggered, false = triggered
			ls_z_up; //!< State of the up limit switch of Z-axis: true = not triggered, false = triggered

	//! \brief Routine for the interrupt attached to the down limit switch of the X-axis.
	static void ls_x_down_routine();
	//! \brief Routine for the interrupt attached to the up limit switch of the X-axis.
	static void ls_x_up_routine();
	//! \brief Routine for the interrupt attached to the down limit switch of the Y-axis.
	static void ls_y_down_routine();
	//! \brief Routine for the interrupt attached to the up limit switch of the Y-axis.
	static void ls_y_up_routine();
	//! \brief Routine for the interrupt attached to the down limit switch of the Z-axis.
	static void ls_z_down_routine();
	//! \brief Routine for the interrupt attached to the up limit switch of the Z-axis.
	static void ls_z_up_routine();

	//! \brief Default constructor
	CNC_Router();

	/*! \brief Constructor
	 *  \param spx Step per one mm on the X-axis
	 *  \param spy Step per one mm on the Y-axis
	 *  \param spz Step per one mm on the Z-axis
	 *  \param vmx Max speed in steps per mm on the X-axis
	 *  \param vmy Max speed in steps per mm on the Y-axis
	 *  \param vmz Max speed in steps per mm on the Z-axis
	 */
	CNC_Router(float spx, float spy, float spz, float vmx, float vmy,
			float vmz);

	//! \brief It initializes the controller for the motor on the X-axis. It must be called one time before starting the motion operations
	void initMotorX();

	//! \brief It initializes the controller for the motor on the Y-axis. It must be called one time before starting the motion operations
	void initMotorY();

	//! \brief It initializes the controller for the motor on the Z-axis. It must be called one time before starting the motion operations
	void initMotorZ();

	//! \brief It initializes the interrupts attached to the limit switches.
	void initInterrupts();

	/*! \brief Sets the pins where are attached the down/up limit switches
	 *  \details The system needs at least the down limit switch
	 *  \param dw The pin number where is attached the down limit switch.
	 *  \param up The pin number where is attached the up limit switch
	 */
	void setLimitSwitchX(int8_t dw, int8_t up = -1);

	/*! \brief Sets the pins where are attached the down/up limit switches
	 *  \details The system needs at least the down limit switch
	 *  \param dw The pin number where is attached the down limit switch.
	 *  \param up The pin number where is attached the up limit switch
	 */
	void setLimitSwitchY(int8_t dw, int8_t up = -1);

	/*! \brief Sets the pins where are attached the down/up limit switches
	 *  \details The system needs at least the down limit switch
	 *  \param dw The pin number where is attached the down limit switch.
	 *  \param up The pin number where is attached the up limit switch
	 */
	void setLimitSwitchZ(int8_t dw, int8_t up = -1);

	//! It resets the position and assigns the value (0; 0) in the actual position of the utensil.
	void resetPos();

	/*! \brief It initializes a motion operation.
	 *  \param px The new position on the X-axis.
	 *  \param py The new position on the Y-axis
	 *  \param spd The feed rate of the motion, (mm/s). If it is 0, the feed rate will be the maximum speed possible according to CNC_Router::v_max_x and CNC_Router::v_max_y.
	 */
	void moveToXY(float px, float py, float spd = 0.0);

	/*! \brief It initializes a motion operation.
	 *  \param px The new position on the X-axis.
	 *  \param py The new position on the Y-axis
	 *  \param pz The new position on the Z-axis
	 *  \param spd The feed rate of the motion, (mm/s). If it is 0, the feed rate will be the maximum speed possible according to CNC_Router::v_max_x and CNC_Router::v_max_y.
	 */
	void moveTo(float px, float py, float pz, float spd = 0.0);

	/*! \brief It initializes a motion operation.
	 *  \param np The new position on the XY-axii.
	 *  \param spd The feed rate of the motion, (mm/s). If it is 0, the feed rate will be the maximum speed possible according to CNC_Router::v_max_x and CNC_Router::v_max_y.
	 */
	void moveToXY(const PositionXYZ &np, float spd = 0.0);

	/*! \brief It initializes a motion operation.
	 *  \param np The new position on the XY-axii.
	 *  \param spd The feed rate of the motion, (mm/s). If it is 0, the feed rate will be the maximum speed possible according to CNC_Router::v_max_x and CNC_Router::v_max_y.
	 */
	void moveTo(const PositionXYZ &np, float spd = 0.0);

	//! \brief It stops the current motion operation.
	void stopMotion();

	//! \brief It pauses the current motion operation. The router waits for CNC_Router::restart() to continue and end the paused motion.
	void pause();

	//! \brief It restarts a paused motion.
	void restart();

	/*! \brief Specifies the motion control mode for the motor on X-axis
	 *  \param m The motion mode can be:
	 *           - with ULN2003A controller
	 *             1. FULL_STEP
	 *             2. HALF_STEP
	 *           - with A4988 controller
	 *             1. FULL_STEP
	 *             2. HALF_STEP
	 *             3. QUARTER_STEP
	 *             4. EIGHTH_STEP
	 *             5. SIXTEENTH_STEP
	 */
	void setMotionModeX(uint8_t m);

	/*! \brief Specifies the motion control mode for the motor on Y-axis
	 *  \param m The motion mode can be:
	 *           - with ULN2003A controller
	 *             1. FULL_STEP
	 *             2. HALF_STEP
	 *           - with A4988 controller
	 *             1. FULL_STEP
	 *             2. HALF_STEP
	 *             3. QUARTER_STEP
	 *             4. EIGHTH_STEP
	 *             5. SIXTEENTH_STEP
	 */
	void setMotionModeY(uint8_t m);

	/*! \brief Specifies the motion control mode for the motor on Z-axis
	 *  \param m The motion mode can be:
	 *           - with ULN2003A controller
	 *             1. FULL_STEP
	 *             2. HALF_STEP
	 *           - with A4988 controller
	 *             1. FULL_STEP
	 *             2. HALF_STEP
	 *             3. QUARTER_STEP
	 *             4. EIGHTH_STEP
	 *             5. SIXTEENTH_STEP
	 */
	void setMotionModeZ(uint8_t m);

	//! \brief It selects the incremental positioning mode.
	void setIncrPos();

	//! \brief It selects the absolute positioning mode.
	void setAbsolPos();

	/*! \brief It moves the utensil up to the home position. It is identified by the down limit switches on the YX-axis.
	 *  \details Pay attention to check the correct installation of the down limit switches. If they do not work well, the function can damage your machine.
	 */
	void searchHomePos();

	/*! \brief It moves the utensil up to the 0 position of the Z-axis.
	 *  \details The function searches the 0 position of the Z-axis. Before running, the function needs that you connect the two ends of
	 *           the wires of the down limit switch of the Z-axis to the utensil tip and to a metal foil that will be placed on the working
	 *           surface (Z=0) under the utensil tip. After done it, you can run the function. The utensil will star to move down. The touch
	 *           of the metal foil by the tip will close the circuit that triggers the limit switch. It will stop the motion.
	 *           Be careful that a wrong setup could damage the machine.
	 */
	void searchZ0Pos();

	//! \brief It get the actual position of the utensil.
	PositionXYZ getPos();

	/*! \brief Configures the position of the utensil
	 *  \param p The new position
	 */

	void setPos(PositionXYZ p);

	/*! \brief It performs the movement configured with the CNC_Route::moveTo().
	 *  \return  It returns an integer that is
	 *           - 1 = the motion is end,
	 *           - 0 =  it is still working,
	 *           - -1 = the motion ended because a limit switch was triggered.
	 *  \sa MSMC_A4988::update() and MSMC_ULN2003A::update().
	 */
	int update();

	/*! \brief The function sets the orientation of X-axis.
	 *  \param v 1 for direct orientation, -1 for inverted orientation
	 */
	void orientationX(int8_t v);

	/*! \brief The function sets the orientation of Y-axis.
	 *  \param v 1 for direct orientation, -1 for inverted orientation
	 */
	void orientationY(int8_t v);

	/*! \brief The function sets the orientation of Z-axis.
	 *  \param v 1 for direct orientation, -1 for inverted orientation
	 */
	void orientationZ(int8_t v);

};

#endif
