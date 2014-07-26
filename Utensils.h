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
#define Utensilsl_h

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
  uint8_t pin;				  //!< The pin where is attached the servo motor
  Servo servo_pen;			  //!< The object which control the servo motor
  uint8_t down_val, 		  //!< The position of the servo for the plotter head down (deg)
          up_val;   		  //!< The position of the servo for the plotter head up (deg)
  
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

#endif
