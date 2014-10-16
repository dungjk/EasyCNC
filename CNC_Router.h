/*!
 \file CNC_Router.h
 \brief The file contains the class which controls the x y router.
 \author    Francesco Giurlanda
 \version   0.0.1
 \date      2014
 \warning   If you change Arduino board some parameters could change too.
 \copyright Quest'opera �� stata rilasciata con licenza Creative Commons Attribuzione
 - Condividi allo stesso modo 4.0 Internazionale. Per leggere una copia della
 licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
 */

#ifndef CNC_Router_h
#define CNC_Router_h

#include "config.h"
#include "Arduino.h"
#include "tools/Position.h"
#include <stdint.h>

#if defined(ROUTER_MX_CONTROLLER_ULN2003A) || defined(ROUTER_MY_CONTROLLER_ULN2003A)
#include "MSMC_ULN2003A.h"
#endif
#if defined(ROUTER_MX_CONTROLLER_A4988) || defined(ROUTER_MY_CONTROLLER_A4988)
#include "MSMC_A4988.h"
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

	PositionXY old_p, //!< It keeps the position that the utensil has after starting the motion
			end_p, 					//!< The final position of a motion
			actual_p; //!< It keeps the actual position of the utensil during an motion
	float spmmx, //!< Steps to move of a mm in the X-axis (steps/mm). \sa config.h
			spmmy; //!< Steps to move of a mm in the Y-axis (steps/mm). \sa config.h
	float v_max_x, //!< The max speed for the motor that controls the X-axis (steps/s). \sa config.h
			v_max_y; //!< The max speed for the motor that controls the Y-axis (steps/s). \sa config.h

	boolean pos_type; //!< It stores the information about the positioning type: true = incremental; false = absolute.

	int8_t pin_ls_x_down, //!< Pin number where it is attached the down limit switch of the X-axis.
			pin_ls_y_down, //!< Pin number where it is attached the down limit switch of the Y-axis.
			pin_ls_x_up, //!< Pin number where it is attached the up limit switch of the X-axis.
			pin_ls_y_up; //!< Pin number where it is attached the up limit switch of the X-axis.

public:
	//! \brief Default constructor
	CNC_Router();

	/*! \brief Constructor
	 *  \param spx Step per one mm on the X-axis
	 *  \param spy Step per one mm on the Y-axis
	 *  \param vmx Max speed in steps per mm on the X-axis
	 *  \param vmy Max speed in steps per mm on the Y-axis
	 */
	CNC_Router(float spx, float spy, float vmx, float vmy);

	//! \brief It initializes the controller for the motor on the X-axis. It must be called one time before starting the motion operations
	void initMotorX();

	//! \brief It initializes the controller for the motor on the Y-axis. It must be called one time before starting the motion operations
	void initMotorY();

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

	//! It resets the position and assigns the value (0; 0) in the actual position of the utensil.
	void resetPos();

	/*! \brief It initializes a motion operation.
	 *  \param px The new position on the X-axis.
	 *  \param py The new position on the Y-axis
	 *  \param spd The feed rate of the motion, (mm/s). If it is 0, the feed rate will be the maximum speed possible according to CNC_Router::v_max_x and CNC_Router::v_max_y.
	 */
	void moveTo(float px, float py, float spd = 0.0);

	/*! \brief It initializes a motion operation.
	 *  \param np The new position on the XY-axii.
	 *  \param spd The feed rate of the motion, (mm/s). If it is 0, the feed rate will be the maximum speed possible according to CNC_Router::v_max_x and CNC_Router::v_max_y.
	 */
	void moveTo(const PositionXY &np, float spd = 0.0);

	//! \brief It stops the current motion operation.
	void stopMotion();

	//! \brief It pauses the current motion operation. The router waits for CNC_Router::restart() to continue and end the paused motion.
	void pause();

	//! \brief It restarts a paused motion.
	void restart();

	/*! \brief It sets up a high precision mode to control the XY-axii.
	 *  \details The function sets up the EIGHTH_STEP for A4988 and HALF_STEP for ULN2003A. \sa MSMC_A4988.h MSMC_ULN2003A.h
	 */
	void highPrecision();

	/*! \brief It sets up a low precision mode to control the XY-axii. It is also the fastest control mode.
	 *  \details The function sets up the FULL_STEP for A4988 and ULN2003A. \sa MSMC_A4988.h MSMC_ULN2003A.h
	 */
	void lowPrecision();

	//! \brief It selects the incremental positioning mode.
	void setIncrPos();

	//! \brief It selects the absolute positioning mode.
	void setAbsolPos();

	/*! \brief It moves the utensil up to the home position. It is identified by the down limit switches on the YX-axis.
	 *  \details Pay attention to check the correct installation of the down limit switches. If they do not work well, the function can damage your machine.
	 */
	void searchHomePos();

	//! \brief It get the actual position of the utensil.
	PositionXY getPos();

	/*! \brief It performs the movement configured with the CNC_Route::moveTo().
	 *  \return  It returns true if the motion id end, otherwise it returns false.
	 *  \sa MSMC_A4988::update() and MSMC_ULN2003A::update().
	 */
	boolean update();

	/*! \brief The function sets the orientation of X-axis.
	 *  \param v 1 for direct orientation, -1 for inverted orientation
	 */
	void orientationX(int8_t v);

	/*! \brief The function sets the orientation of X-axis.
	 *  \param v 1 for direct orientation, -1 for inverted orientation
	 */
	void orientationY(int8_t v);

};

#endif
