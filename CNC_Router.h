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
#include "Position.h"
#include <stdint.h>

#if defined(ROUTER_MX_CONTROLLER_ULN2003A) || defined(ROUTER_MY_CONTROLLER_ULN2003A)
#include "MSMC_ULN2003A.h"
#endif
#if defined(ROUTER_MX_CONTROLLER_A4988) || defined(ROUTER_MY_CONTROLLER_A4988)
#include "MSMC_A4988.h"
#endif

/*! \class CNC_Router
 *  \brief The class controls the motors of the xy router of the CNC.
 *  \details TODO
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

  PositionXY old_p, end_p, actual_p;
  float spmmx, spmmy;  // (steps/mm)
  float v_max_x, v_max_y; // (steps/s)
  
  boolean pos_type;      // true = incremental; false = absolute;
  
  //pins where are attached the limit switch
  int8_t pin_ls_x_down, pin_ls_y_down, pin_ls_x_up, pin_ls_y_up;
  
  public:
  CNC_Router();
  CNC_Router(float spx, float spy, float vmx, float vmy);
  
  void initMotorX();
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
  
  void resetPos();
  
  void moveTo(float px, float py);
  void moveTo(const PositionXY &np);
  
  void stopMotion();
  void pause();
  void restat();
  
  void highPrecision();
  void lowPrecision();
  void setIncrPos();
  void setAbsolPos(); 
 
  
  void searchHomePos();
  
  boolean update();
  
};

#endif
