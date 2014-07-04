//  Quest'opera è stata rilasciata con licenza Creative Commons Attribuzione 
//  - Condividi allo stesso modo 4.0 Internazionale. 
//  Per leggere una copia della licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
//
//  Autor: Francesco Giurlanda
//  Project: Easy CNC
//  versione: 0.0.1

#include "Utensils.h"

// *******************
// ***** PLOTTER *****
// *******************
#ifdef _PLOTTER


PenControl::PenControl(uint8_t p, uint8_t dv, uint8_t uv) : pin(p), down_val(dv), up_val(uv){}

void PenControl::init(){
  servo_pen.attach(pin);
  state = UP;
  servo_pen.write(up_val);
}

boolean PenControl::up(){
  boolean ret = (state != UP);
  if(ret) {
    state = UP;
    servo_pen.write(up_val);
    delay(500);
  }
  return ret;
}

boolean PenControl::down(){
  boolean ret = (state != DOWN);
  if(ret) {
    state = DOWN;
    servo_pen.write(down_val);
    delay(500);
  }
  return ret;
}

#endif

// **************************
