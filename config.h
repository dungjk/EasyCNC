/*! 
    \file      config.h
    \brief     The configuration file
    \details   In this file there are all the configurable parameters.
    \author    Francesco Giurlanda
    \version   0.2
    \date      2015
    \warning   If you change Arduino board some parameters could change too.
    \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
               To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */
#ifndef CONFIG_H
#define CONFIG_H

// ****************************
// ***    GENERAL SETUP     ***
// ****************************

/*! \def SERIAL_BOUND
    \brief Define the speed of the serial communication.
*/
/*! \def INTERRUPT_STOP_MOTION
    \brief Define the number of the interrupt which is attached the stop button.
    
    This value is different from the pin number at which the interrupt refers.
    The pin depends on the arduino board version. See http://arduino.cc/en/Main/Products
*/

#include "pins_arduino.h"

#define SERIAL_BOUND 115200
//#define INTERRUPT_STOP_MOTION 1


// ****************************
// *** ROUTER CONFIGURATION ***
// ****************************

#define ROUTER_MX_STEPS_PER_ROUND 200L  /*!< \brief Number of steps to make a round for the motor of the X-axis. */
#define ROUTER_MY_STEPS_PER_ROUND 200L  /*!< \brief Number of steps to make a round for the motor of the Y-axis. */
#define ROUTER_MZ_STEPS_PER_ROUND 200L  /*!< \brief Number of steps to make a round for the motor of the Z-axis. */
#define ROUTER_MX_STEPS_PER_MM ROUTER_MX_STEPS_PER_ROUND/1.256  /*!< \brief Number of steps to forward of a mm. */
#define ROUTER_MY_STEPS_PER_MM ROUTER_MY_STEPS_PER_ROUND/1.25  /*!< \brief Number of steps to forward of a mm. */
#define ROUTER_MZ_STEPS_PER_MM ROUTER_MZ_STEPS_PER_ROUND/1.256  /*!< \brief Number of steps to forward of a mm. */
#define ROUTER_MX_ORIENTATION 1         /*!< \brief The value can be 1 or -1 and it is used to invert the axis orientation. */
#define ROUTER_MY_ORIENTATION -1		/*!< \brief The value can be 1 or -1 and it is used to invert the axis orientation. */
#define ROUTER_MZ_ORIENTATION 1			/*!< \brief The value can be 1 or -1 and it is used to invert the axis orientation.*/
#define ROUTER_MX_SPEED 160.0           /*!< \brief The maximum speed used to control the motor of X-axis (mm/min). */
#define ROUTER_MY_SPEED 160.0           /*!< \brief The maximum speed used to control the motor of Y-axis (mm/min). */
#define ROUTER_MZ_SPEED 160.0           /*!< \brief The maximum speed used to control the motor of Z-axis (mm/min). */
#define ROUTER_DOWN_LIMIT_SWITCH_X_INTERRUPT  1                 /*!< \brief The number of the interrupt which is connected to the down limit switch of the X-axis. It is different by the pin number. More details <a href="http://arduino.cc/en/Reference/AttachInterrupt">here</a>. The value -1 means that it is disconnected.*/
#define ROUTER_DOWN_LIMIT_SWITCH_Y_INTERRUPT -1// 0                 /*!< \brief The number of the interrupt which is connected to the down limit switch of the Y-axis. It is different by the pin number. More details <a href="http://arduino.cc/en/Reference/AttachInterrupt">here</a>. The value -1 means that it is disconnected.*/
#define ROUTER_DOWN_LIMIT_SWITCH_Z_INTERRUPT -1 //5                  /*!< \brief The number of the interrupt which is connected to the down limit switch of the Z-axis. It is different by the pin number. More details <a href="http://arduino.cc/en/Reference/AttachInterrupt">here</a>. The value -1 means that it is disconnected.*/
#define ROUTER_UP_LIMIT_SWITCH_X_INTERRUPT  -1                  /*!< \brief The number of the interrupt which is connected to the up limit switch of the X-axis. It is different by the pin number. More details <a href="http://arduino.cc/en/Reference/AttachInterrupt">here</a>. The value -1 means that it is disconnected.*/
#define ROUTER_UP_LIMIT_SWITCH_Y_INTERRUPT  -1                  /*!< \brief The number of the interrupt which is connected to the up limit switch of the Y-axis. It is different by the pin number. More details <a href="http://arduino.cc/en/Reference/AttachInterrupt">here</a>. The value -1 means that it is disconnected.*/
#define ROUTER_UP_LIMIT_SWITCH_Z_INTERRUPT  -1                  /*!< \brief The number of the interrupt which is connected to the up limit switch of the Z-axis. It is different by the pin number. More details <a href="http://arduino.cc/en/Reference/AttachInterrupt">here</a>. The value -1 means that it is disconnected.*/
#define ROUTER_DOWN_LIMIT_SWITCH_X_ACTIVE_HIGH 1				/*!< \brief 1 if the switch is active high, 0 otherwise */
#define ROUTER_DOWN_LIMIT_SWITCH_Y_ACTIVE_HIGH 0				/*!< \brief 1 if the switch is active high, 0 otherwise */
#define ROUTER_DOWN_LIMIT_SWITCH_Z_ACTIVE_HIGH 0				/*!< \brief 1 if the switch is active high, 0 otherwise */
#define ROUTER_UP_LIMIT_SWITCH_X_ACTIVE_HIGH 1				/*!< \brief 1 if the switch is active high, 0 otherwise */
#define ROUTER_UP_LIMIT_SWITCH_Y_ACTIVE_HIGH 1				/*!< \brief 1 if the switch is active high, 0 otherwise */
#define ROUTER_UP_LIMIT_SWITCH_Z_ACTIVE_HIGH 1				/*!< \brief 1 if the switch is active high, 0 otherwise */

#define ROUTER_DRIVERS_COOLING 8							/*!< \brief Pin that controls the cooling of the motors drivers*/

#define ROUTER_MX_CONTROLLER_A4988

#if defined(ROUTER_MX_CONTROLLER_A4988)
#define ROUTE_MX_STEP_CONTROL_PIN   A0  		 //!< \brief Pin connected to the step control pin of the A4988
#define ROUTE_MX_DIRECTION_CONTROL_PIN A1	 //!< \brief Pin connected to the direction control pin of the A4988
#define ROUTE_MX_ENABLE_CONTROL_PIN 38		 //!< \brief Pin connected to the enable pin of the A4988
#define ROUTE_MX_MODE EIGHTH_STEP           //!< \brief The microstep mode of the driver of the motor X
#endif

#define ROUTER_MY_CONTROLLER_A4988

#if defined(ROUTER_MY_CONTROLLER_A4988)
#define ROUTE_MY_STEP_CONTROL_PIN 46    		 //!< \brief Pin connected to the step control pin of the A4988
#define ROUTE_MY_DIRECTION_CONTROL_PIN 48	 //!< \brief Pin connected to the direction control pin of the A4988
#define ROUTE_MY_ENABLE_CONTROL_PIN A8		 //!< \brief Pin connected to the enable pin of the A4988
#define ROUTE_MY_MODE EIGHTH_STEP           //!< \brief The microstep mode of the driver of the motor Y
#endif

#define ROUTER_MZ_CONTROLLER_A4988

#if defined(ROUTER_MZ_CONTROLLER_A4988)
#define ROUTE_MZ_STEP_CONTROL_PIN A6    		 //!< \brief Pin connected to the step control pin of the A4988
#define ROUTE_MZ_DIRECTION_CONTROL_PIN A7	 //!< \brief Pin connected to the direction control pin of the A4988
#define ROUTE_MZ_ENABLE_CONTROL_PIN A2		 //!< \brief Pin connected to the enable pin of the A4988
#define ROUTE_MZ_MODE EIGHTH_STEP           //!< \brief The microstep mode of the driver of the motor Z
#endif

// *****************************
// *          UTENSILS         *
// *****************************

#define _PLOTTER_SERVO                    /*!< \brief Uncomment to enable the servo plotter support. */
//#define _MILLING_MACHINE                 /*!< \brief Uncomment to enable the milling machine support. */
//#define _LASER							/*! \brief Uncomment to enable the laser support. */

// *****************************
// *       UTENSIL SETUP       *
// *****************************


#ifdef _PLOTTER_SERVO

#define _PLOTTER_SERVO_PIN       11        /*!< \brief The PWN pin where is attached the servo motor. */
#define _PLOTTER_SERVO_UP_POS    80       /*!< \brief The value on the range [0, 180] that moves up the pen of the plotter. */
#define _PLOTTER_SERVO_DOWN_POS  136      /*!< \brief The value on the range [0, 180] that moves down the pen of the plotter. */

#endif

#ifdef _MILLING_MACHINE
#define _MILLING_MACHINE_ENABLE_PIN 32    /*!< \brief The pin number connected to the enable circuit of the milling machine*/
#define _MILLING_MACHINE_SPEED_PIN   11    /*!< \brief The pin number connected to the speed control circuit of the milling machine.*/
#endif

#ifdef _LASER
#define _LASER_CONTROL_PIN 32				/*!< \brief The pin number connected to the enable circuit of the laser */
#define _LASER_CONTROL_ACTIVE_HIGH 0		/*!< \brief 1 if the laser driver is active high, 0 otherwise. */
#endif

#endif //CONFIG_H


