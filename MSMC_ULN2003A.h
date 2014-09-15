/*! 
    \file      MSMC_ULN2003A.h
    \brief     Multi Stepper Motor Control with ULN2003A.
    \details   The class allows the contemporary control of multi stepper motors using the ICs ULN2003A. After the motor setup, you can drive the motor forward and backward,
               specifying the number of steps and the delay in microseconds between two consecutive steps. The function update performs the effective motor motion, it must be call 
               in the main loop and, if the delay is expired, the function moves the motor of one step.
    \author    Francesco Giurlanda
    \version   0.0.1
    \date      2014

    \copyright Quest'opera è stata rilasciata con licenza Creative Commons Attribuzione
               - Condividi allo stesso modo 4.0 Internazionale. Per leggere una copia della 
               licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
 */

#ifndef MSMC_ULN2003A_H_
#define MSMC_ULN2003A_H_

#include "Arduino.h"
#include <stdint.h>
#include "MSMC.h"

#define FULL_STEP 1      /*!< Identify the full step mode, \sa MSMC_ULN2003A::setMode(uint8_t) */
#define HALF_STEP 2      /*!< Identify the half step mode, \sa MSMC_ULN2003A::setMode(uint8_t) */

static int full_step_seq[4][4] = {
                {HIGH, LOW, LOW, HIGH},
                {HIGH, LOW, HIGH, LOW},
                {LOW, HIGH, HIGH, LOW},
                {LOW, HIGH, LOW, HIGH}
              };                           /*!< \brief Define the full step sequence to drive an unipolar motor.*/
              
static int half_step_seq[8][4] = {
                {HIGH, LOW,  LOW,  HIGH},
                {HIGH, LOW,  LOW,  LOW},
                {HIGH, LOW,  HIGH, LOW},
                {LOW,  LOW,  HIGH, LOW},
                {LOW,  HIGH, HIGH, LOW},
                {LOW,  HIGH, LOW,  LOW},
                {LOW,  HIGH, LOW,  HIGH},
                {LOW,  LOW,  LOW,  HIGH}
              };							/*!< \brief Define the half step sequence to drive an unipolar motor. */

/*! \class MSMC_ULN2003A
    \brief Multi Stepper Motor Control with ULN2003A.
    \details The class allows the contemporary control of multi stepper motors using the ICs ULN2003A. After the motor setup, you can drive the motor forward and backward,
             specifying the number of steps and the delay in microseconds between two consecutive steps. The function update performs the effective motor motion, it must be call
             in the main loop and, if the delay is expired, the function moves the motor of one step.
    \author    Francesco Giurlanda
*/
class MSMC_ULN2003A : public MSMC {
  
  uint8_t p1, p2, p3, p4;                  /*!< \brief Pins where is mapped the coils of the unipolar motor. */
  uint32_t spd;                            /*!< \brief The delay between two steps in microseconds. */
  uint32_t steps,                          /*!< \brief The number of steps until the motion ends. */
           tot_steps,                      /*!< \brief The total steps set by forward() and backward(). */
           old_time,                       /*!< \brief The time value in microseconds set during the last step. */
           pause_time;					   /*!< \brief The time value in microseconds set by the function MSMC_ULN2003A::pause()*/
  int8_t pos;                              /*!< \brief A value among 0, 1, 2, 3 if the full step control is active, otherwise the value is one among 0, 1, 2, 3, ,4 ,5 ,6 7 */ 
  int8_t dir,                              /*!< \brief Specify the direction of the motor: -1 backward, 1 forward, 0 all pins out LOW */
         dir_mode;						   /*!< Specifies if the motion is direct or inverted: 1 direct and -1 inverted*/
  uint8_t control_mode;                    /*!< \brief Specify the control mode of the controller. */
  boolean m_ready;                         /*!< \brief True if the motor is ready to take a new command: forward(uint32_t, uint32_t) or backward(uint32_t, uint32_t). */
  boolean m_pause;                           /*!< \brief It is True if the motion is paused. */

 
  //! Move the motor forward of one step.
  /*! \sa backward_one()
  */
  void forward_one();
  //! Move the motor backward of one step.
  /*! \sa forward_one()
  */
  void backward_one();
  //! Turn off all coils of the motor
  void coilsOff();
  
  public:
  //! Default constructor
  MSMC_ULN2003A();
  
  //! Constructor
  /*! Setup the pins where are attached the four coils of the unipolar motor.
      \param pin1 the pin connected to the coil 1a
      \param pin2 the pin connected to the coil 2a
      \param pin3 the pin connected to the coil 1b
      \param pin4 the pin connected to the coil 2b      
  */
  MSMC_ULN2003A(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);
  
  //! Configure the motor to move forward
  /*! The function moves forward the motor of a number of steps with a specified delay betweet each step
      \param st the number of steps
      \param sp the delay between the steps in microseconds
      \sa backward(uint32_t, uint32_t)
  */
  void forward(uint32_t st, uint32_t sp);
  
  //! Configure the motor to move backward
  /*! The function moves backward the motor of a number of steps with a specified delay betweet each step
      \param st the number of steps
      \param sp the delay between the steps in microseconds
      \sa forward(uint32_t, uint32_t)
  */
  void backward(uint32_t, uint32_t);
  
  /*! \brief Configure the operative mode of the controller.
   *  \param mod The mode used by the controller, you can use the following macros:
   *         - #FULL_STEP
   *         - #HALF_STEP
   */
  void setMode(uint8_t mod);

  /*! \brief It sets the direction mode.
  	 *  \param m The direction mode: 1 direct, -1 inverted.
  	 */
  void dirMode(int8_t m);

  /*! \brief Return the operative mode of the controller
   *  \return An int that identifies the operative mode: #FULL_STEP and #HALF_STEP
   */
  uint8_t getMode();
  //! Change the pin mapping
  /*! \param pin1 the pin connected to the coil 1a
      \param pin2 the pin connected to the coil 2a
      \param pin3 the pin connected to the coil 1b
      \param pin4 the pin connected to the coil 2b 
  */
  void setPins(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);
  
  /*! \brief Change the delay between each step.
      \details This function can be used to change the speed during an operation of forward or backward.
      \param sp the delay in microseconds.
      \sa forward(uint32_t, uint32_t), backward(uint32_t, uint32_t)
  */
  void speed(uint32_t sp);
  
  //! Stop any motion.
  void stop();
  
  //! Pause the motion. \sa MSMC_ULN2003A::restart()
  void pause();

  //! Restart the motion after the call of the function MSMC_ULN2003A::pause()
  void restart();

  //! Perform the motion of the motor
  /*! The function performs effectively the motor motion. 
      It verifies that the specified delay from the last step is expired and than moves the motor of one step.
      The function must be call in the main loop with a frequency greater than 1/(delay*1000000).
      Greater is the frequency to call the function and higher is the precision of the the motion.
      \return the function number of steps made, up to the end of the motion. 
              If the motion end the motor is idle and it can perform a new command.
              When the motor is idle the function returns -1.
  */
  int32_t update();
  
  //! Print in the serial channel the state of all inner variables.
  void print();
};


#endif
