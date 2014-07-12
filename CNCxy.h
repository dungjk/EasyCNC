//  Quest'opera è stata rilasciata con licenza Creative Commons Attribuzione 
//  - Condividi allo stesso modo 4.0 Internazionale. 
//  Per leggere una copia della licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
//
//  Autor: Francesco Giurlanda
//  Project: Easy CNC
//  versione: 0.0.1


#ifndef CNCxy_h
#define CNCxy_h

#include "Arduino.h"
#include "SMotor.h"
#include "Position.h"
#include <stdint.h>

class CNCxy {
  
  SMotor mx, my; 
  PositionXY old_p, end_p, actual_p;
  float spmmx, spmmy;  // (steps/mm)
  float v_max_x, v_max_y; // (steps/s)
  
  boolean pos_type;      // true = incremental; false = absolute;
  
  //pins where are attached the limit switch
  int8_t pin_ls_x_down, pin_ls_y_down, pin_ls_x_up, pin_ls_y_up;
  
  
  
  public:
  CNCxy();
  CNCxy(float spx, float spy, float vmx, float vmy);
  
  void setMotorX(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);
  void setMotorY(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);
  
  void setLimitSwitchX(int8_t dw, int8_t up = -1);
  void setLimitSwitchY(int8_t dw, int8_t up = -1);
  
  void resetPos();
  
  void moveTo(float px, float py);
  void moveTo(const PositionXY &np);
  
  void stopMotion();
  
  void highPrecision();
  void lowPrecision();
  void setIncrPos();
  void setAbsolPos(); 
 
  
  void searchHomePos();
  
  boolean update();
  
};

#endif
