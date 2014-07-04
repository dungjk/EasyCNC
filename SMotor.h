//  Quest'opera è stata rilasciata con licenza Creative Commons Attribuzione 
//  - Condividi allo stesso modo 4.0 Internazionale. 
//  Per leggere una copia della licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
//
//  Autor: Francesco Giurlanda
//  Project: Easy CNC
//  versione: 0.0.1

#ifndef SMotor_h
#define SMotor_h

#include "Arduino.h"
#include <stdint.h>

//#define SM_DBG

#ifdef SM_DBG


void ledOn(){
  digitalWrite(13, HIGH);
}

void ledOff(){
  digitalWrite(13, LOW);
}

#endif

static int full_step_seq[4][4] = {
                {HIGH, LOW, LOW, HIGH},
                {HIGH, LOW, HIGH, LOW},
                {LOW, HIGH, HIGH, LOW},
                {LOW, HIGH, LOW, HIGH}
              };
              
static int half_step_seq[8][4] = {
                {HIGH, LOW,  LOW,  HIGH},
                {HIGH, LOW,  LOW,  LOW},
                {HIGH, LOW,  HIGH, LOW},
                {LOW,  LOW,  HIGH, LOW},
                {LOW,  HIGH, HIGH, LOW},
                {LOW,  HIGH, LOW,  LOW},
                {LOW,  HIGH, LOW,  HIGH},
                {LOW,  LOW,  LOW,  HIGH}
              };


class SMotor {
  
  uint8_t p1, p2, p3, p4;
  uint32_t spd;         // delay between two steps in microseconds
  uint32_t steps, tot_steps, old_time;
  int8_t pos;          // a value among 0, 1, 2, 3
  int8_t dir;           // -1 backward, 1 forward, 0 all pin out LOW
  boolean full_step;
  boolean m_ready;
  
  void forward_one();
  void backward_one();
  void coilsOff();
  
  public:
  SMotor();
  SMotor(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);
  void forward(uint32_t, uint32_t);
  void backward(uint32_t, uint32_t);
  void setFullStep();
  void setHalfStep();
  void setPins(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);
  
  void speed(uint32_t);
  void stop();
  int32_t update();
  void print();
};


#endif
