//  Quest'opera è stata rilasciata con licenza Creative Commons Attribuzione 
//  - Condividi allo stesso modo 4.0 Internazionale. 
//  Per leggere una copia della licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
//
//  Autor: Francesco Giurlanda
//  Project: Easy CNC
//  versione: 0.0.1

#ifndef Utensils_h
#define Utensilsl_h

#include "config.h"

// *******************
// ***** PLOTTER *****
// *******************

#ifdef _PLOTTER

#include <Servo.h>
#include <Arduino.h>

#include <stdint.h>

#define DOWN true
#define UP false

class PenControl {
  boolean state;
  uint8_t pin;
  Servo servo_pen;
  uint8_t down_val, up_val;
  
  public:
  
  PenControl(uint8_t p, uint8_t dv, uint8_t uv);
  void init();
  boolean up();
  boolean down();
};

#endif

// ***********************


#endif
