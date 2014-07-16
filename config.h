/*! 
    \file config.h
    \brief The configuration file
    \details   In this file there are all the configurable parameters.
    \author    Francesco Giurlanda
    \version   0.0.1
    \date      2014
    \warning   If you change Arduino board some parameters could change too.
    \copyright Quest'opera è stata rilasciata con licenza Creative Commons Attribuzione 
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
#define SERIAL_BOUND 9600
#define INTERRUPT_STOP_MOTION 0


// ****************************
// *** ROUTER CONFIGURATION ***
// ****************************

#define ROUTER_MX_STEPS_PER_ROUND 48*2  /*!< \brief Nunber of steps to make a round for the motor of the X-axis. */
#define ROUTER_MY_STEPS_PER_ROUND 96*2  /*!< \brief Nunber of steps to make a round for the motor of the Y-axis. */
#define ROUTER_MX_SPEED 300.0           /*!< \brief The maximum speet used to control the motor of X-axis. The unit is steps/s. */
#define ROUTER_MY_SPEED 300.0           /*!< \brief The maximum speet used to control the motor of Y-axis. The unit is steps/s. */
#define ROUTER_MX_COIL1 4                  /*!< \brief Pin connected to the coil 1 of the unipolar motor of X-axis. */
#define ROUTER_MX_COIL2 5                  /*!< \brief Pin connected to the coil 2 of the unipolar motor of X-axis. */
#define ROUTER_MX_COIL3 6                  /*!< \brief Pin connected to the coil 3 of the unipolar motor of X-axis. */
#define ROUTER_MX_COIL4 7                  /*!< \brief Pin connected to the coil 4 of the unipolar motor of X-axis. */

#define ROUTER_MY_COIL1 8                  /*!< \brief Pin connected to the coil 1 of the unipolar motor of Y-axis. */
#define ROUTER_MY_COIL2 9                  /*!< \brief Pin connected to the coil 2 of the unipolar motor of Y-axis. */
#define ROUTER_MY_COIL3 10                 /*!< \brief Pin connected to the coil 3 of the unipolar motor of Y-axis. */
#define ROUTER_MY_COIL4 11                 /*!< \brief Pin connected to the coil 4 of the unipolar motor of Y-axis. */

#define ROUTER_LIMIT_X 12                  /*!< \brief The number of the pin which is connected to the limit switch of the X-axis. */
#define ROUTER_LIMIT_Y 12                  /*!< \brief The number of the pin which is connected to the limit switch of the Y-axis. */
// *****************************
// *          UTENSILS         *
// *****************************

#define _PLOTTER_SERVO                    /*!< \brief Uncomment to active the servo plotter support. */


// *****************************
// *       UTENSIL SETUP       *
// *****************************


#ifdef _PLOTTER_SERVO

#define _PLOTTER_SERVO_PIN       3        /*!< \brief The PWN pin where is attached the serco motor. */
#define _PLOTTER_SERVO_UP_POS    80       /*!< \brief The value on the range [0, 180] that moves up the pen of the plotter. */
#define _PLOTTER_SERVO_DOWN_POS  136      /*!< \brief The value on the range [0, 180] that moves down the pen of the plotter. */

#endif
