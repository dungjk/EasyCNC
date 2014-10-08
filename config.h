/*! 
    \file config.h
    \brief The configuration file
    \details   In this file there are all the configurable parameters.
    \author    Francesco Giurlanda
    \version   0.0.1
    \date      2014
    \warning   If you change Arduino board some parameters could change too.
    \copyright Quest'opera �� stata rilasciata con licenza Creative Commons Attribuzione
               - Condividi allo stesso modo 4.0 Internazionale. Per leggere una copia della 
               licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
 */

// ****************************
// ***    GENERAL SETUP     ***
// ****************************

/*! \def SERIAL_BOUND
    \brief Define the speed of the serial comunication.
*/
/*! \def INTERRUPT_STOP_MOTION
    \brief Define the number of the interrupt which is attached the stop button.
    
    This value is different from the pin number at which the interrupt refers.
    The pin depends on the arduino board version. See http://arduino.cc/en/Main/Products
*/

#include "pins_arduino.h"

#define SERIAL_BOUND 9600
#define INTERRUPT_STOP_MOTION 1


// ****************************
// *** ROUTER CONFIGURATION ***
// ****************************

//#define ULN2003A
//#define A4988

#define ROUTER_MX_STEPS_PER_ROUND 200.0  /*!< \brief Nunber of steps to make a round for the motor of the X-axis. */
#define ROUTER_MY_STEPS_PER_ROUND 200.0  /*!< \brief Nunber of steps to make a round for the motor of the Y-axis. */
#define ROUTER_MX_STEPS_PER_MM ROUTER_MX_STEPS_PER_ROUND/1.256  /*!< \brief Number of steps to forward of a mm. */
#define ROUTER_MY_STEPS_PER_MM ROUTER_MY_STEPS_PER_ROUND/1.25  /*!< \brief Number of steps to forward of a mm. */
#define ROUTER_MX_SPEED 5000.0           /*!< \brief The maximum speed used to control the motor of X-axis. The unit is steps/s. */
#define ROUTER_MY_SPEED 5000.0           /*!< \brief The maximum speed used to control the motor of Y-axis. The unit is steps/s. */
#define ROUTER_DOWN_LIMIT_SWITCH_X 3                  /*!< \brief The number of the pin which is connected to the limit switch of the X-axis. */
#define ROUTER_DOWN_LIMIT_SWITCH_Y 14                  /*!< \brief The number of the pin which is connected to the limit switch of the Y-axis. */

//#define ROUTER_MX_CONTROLLER_ULN2003A
#define ROUTER_MX_CONTROLLER_A4988

#if defined(ROUTER_MX_CONTROLLER_ULN2003A)
#define ROUTER_MX_COIL1_PIN 4                  /*!< \brief Pin connected to the coil 1 of the unipolar motor of X-axis through the ULN2003A controller. */
#define ROUTER_MX_COIL2_PIN 5                  /*!< \brief Pin connected to the coil 2 of the unipolar motor of X-axis through the ULN2003A controller. */
#define ROUTER_MX_COIL3_PIN 6                  /*!< \brief Pin connected to the coil 3 of the unipolar motor of X-axis through the ULN2003A controller. */
#define ROUTER_MX_COIL4_PIN 7                  /*!< \brief Pin connected to the coil 4 of the unipolar motor of X-axis through the ULN2003A controller. */
#endif

#if defined(ROUTER_MX_CONTROLLER_A4988)
#define ROUTE_MX_STEP_CONTROL_PIN   A0  		 //!< \brief Pin connected to the step control pin of the A4988
#define ROUTE_MX_DIRECTION_CONTROL_PIN A1	 //!< \brief Pin connected to the direction control pin of the A4988
#define ROUTE_MX_ENABLE_CONTROL_PIN 38		 //!< \brief Pin connected to the enable pin of the A4988
#endif

//#define ROUTER_MY_CONTROLLER_ULN2003A
#define ROUTER_MY_CONTROLLER_A4988

#if defined(ROUTER_MX_CONTROLLER_ULN2003A)
#define ROUTER_MY_COIL1_PIN 8                  /*!< \brief Pin connected to the coil 1 of the unipolar motor of Y-axis through the ULN2003A controller. */
#define ROUTER_MY_COIL2_PIN 9                  /*!< \brief Pin connected to the coil 2 of the unipolar motor of Y-axis through the ULN2003A controller. */
#define ROUTER_MY_COIL3_PIN 10                 /*!< \brief Pin connected to the coil 3 of the unipolar motor of Y-axis through the ULN2003A controller. */
#define ROUTER_MY_COIL4_PIN 11                 /*!< \brief Pin connected to the coil 4 of the unipolar motor of Y-axis through the ULN2003A controller. */
#endif

#if defined(ROUTER_MY_CONTROLLER_A4988)
#define ROUTE_MY_STEP_CONTROL_PIN 46    		 //!< \brief Pin connected to the step control pin of the A4988
#define ROUTE_MY_DIRECTION_CONTROL_PIN 48	 //!< \brief Pin connected to the direction control pin of the A4988
#define ROUTE_MY_ENABLE_CONTROL_PIN A8		 //!< \brief Pin connected to the enable pin of the A4988
#endif

// *****************************
// *          UTENSILS         *
// *****************************

//#define _PLOTTER_SERVO                    /*!< \brief Uncomment to active the servo plotter support. */
#define _MILLING_MACHINE                 /*!< \brief Uncomment to active the milling machine support. */

// *****************************
// *       UTENSIL SETUP       *
// *****************************


#ifdef _PLOTTER_SERVO

#define _PLOTTER_SERVO_PIN       5        /*!< \brief The PWN pin where is attached the serco motor. */
#define _PLOTTER_SERVO_UP_POS    80       /*!< \brief The value on the range [0, 180] that moves up the pen of the plotter. */
#define _PLOTTER_SERVO_DOWN_POS  136      /*!< \brief The value on the range [0, 180] that moves down the pen of the plotter. */

#endif

#ifdef _MILLING_MACHINE
#define _MILLING_MACHINE_MZ_STEPS_PER_ROUND 200.0  /*!< \brief Nunber of steps to make a round for the motor of the Z-axis. */
#define _MILLING_MACHINE_MZ_STEPS_PER_MM _MILLING_MACHINE_MZ_STEPS_PER_ROUND/1.256  /*!< \brief Number of steps to forward of a mm. */
#define _MILLING_MACHINE_MZ_FAST_SPEED 5000.0           /*!< \brief The maximum speed used to control the motor of Z-axis. The unit is steps/s. */
#define _MILLING_MACHINE_DOWN_LIMIT_SWITCH_Z 18  /*!< \brief The number of the pin which is connected to the limit switch of the Z-axis. */

//#define _MILLING_MACHINE_MZ_CONTROLLER_ULN2003A
#define _MILLING_MACHINE_MZ_CONTROLLER_A4988

#if defined(_MILLING_MACHINE_MZ_CONTROLLER_ULN2003A)
#define _MILLING_MACHINE_MZ_COIL1_PIN 8                  /*!< \brief Pin connected to the coil 1 of the unipolar motor of Z-axis through the ULN2003A controller. */
#define _MILLING_MACHINE_MZ_COIL2_PIN 9                  /*!< \brief Pin connected to the coil 2 of the unipolar motor of Z-axis through the ULN2003A controller. */
#define _MILLING_MACHINE_MZ_COIL3_PIN 10                 /*!< \brief Pin connected to the coil 3 of the unipolar motor of Z-axis through the ULN2003A controller. */
#define _MILLING_MACHINE_MZ_COIL4_PIN 11                 /*!< \brief Pin connected to the coil 4 of the unipolar motor of Z-axis through the ULN2003A controller. */
#endif

#if defined(_MILLING_MACHINE_MZ_CONTROLLER_A4988)
#define _MILLING_MACHINE_MZ_STEP_CONTROL_PIN   A6  		 //!< \brief Pin connected to the step control pin of the A4988
#define _MILLING_MACHINE_MZ_DIRECTION_CONTROL_PIN A7	 //!< \brief Pin connected to the direction control pin of the A4988
#define _MILLING_MACHINE_MZ_ENABLE_CONTROL_PIN A2		 //!< \brief Pin connected to the enable pin of the A4988
#endif

// ******************
//     TESTING
// ******************

#define _TEST
#define DBGNL(x) Serial.println(x)
#define DBG(x) Serial.print(x)

#endif
