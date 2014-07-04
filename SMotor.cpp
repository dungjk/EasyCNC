//  Quest'opera è stata rilasciata con licenza Creative Commons Attribuzione 
//  - Condividi allo stesso modo 4.0 Internazionale. 
//  Per leggere una copia della licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
//
//  Autor: Francesco Giurlanda
//  Project: Easy CNC
//  versione: 0.0.1

#include "SMotor.h"


//********** COSTRUCTOR **********
SMotor::SMotor(){
   p1 = p2 = p3 = p4 = old_time = steps = tot_steps = spd = pos = dir = 0;
   full_step = true;
   m_ready = false;    //the motor is not ready because the pins are not specified yet.
}

SMotor::SMotor(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4){
  p1 = pin1;
  p2 = pin2;
  p3 = pin3;
  p4 = pin4;
  pinMode(p1, OUTPUT);
  pinMode(p2, OUTPUT);
  pinMode(p3, OUTPUT);
  pinMode(p4, OUTPUT);
  full_step = true;
  old_time = steps = tot_steps = spd = pos = dir = 0;
  m_ready = true;
}

//********** SETUP ************

void SMotor::speed(uint32_t s){
   spd = s;
}

void SMotor::setFullStep(){
    full_step = true;
}
  
void SMotor::setHalfStep(){
    full_step = false;  
}

void SMotor::setPins(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4){
  if(!m_ready){
    p1 = pin1;
    p2 = pin2;
    p3 = pin3;
    p4 = pin4;
    pinMode(p1, OUTPUT);
    pinMode(p2, OUTPUT);
    pinMode(p3, OUTPUT);
    pinMode(p4, OUTPUT);
    m_ready = true;
  }
}

//********** MOVING ************
void SMotor::forward(uint32_t st, uint32_t sp){
  steps = tot_steps = st;
  
  spd = sp;
  dir = 1;
  old_time = micros();
  m_ready = false;
}

void SMotor::backward(uint32_t st, uint32_t sp){
  steps = tot_steps = st;
  spd = sp;
  dir = -1;
  old_time = micros();
  m_ready = false;
}

void SMotor::forward_one(){
  if(full_step){
     pos = (pos + 1) % 4;
     digitalWrite(p1, full_step_seq[pos][0]);
     digitalWrite(p2, full_step_seq[pos][1]);
     digitalWrite(p3, full_step_seq[pos][2]);
     digitalWrite(p4, full_step_seq[pos][3]);
  }else{
     pos = (pos + 1) % 8;
     digitalWrite(p1, half_step_seq[pos][0]);
     digitalWrite(p2, half_step_seq[pos][1]);
     digitalWrite(p3, half_step_seq[pos][2]);
     digitalWrite(p4, half_step_seq[pos][3]);
  }
}

void SMotor::backward_one(){
  if(full_step){
     pos = (pos != 0)? (pos - 1): 3;
     digitalWrite(p1, full_step_seq[pos][0]);
     digitalWrite(p2, full_step_seq[pos][1]);
     digitalWrite(p3, full_step_seq[pos][2]);
     digitalWrite(p4, full_step_seq[pos][3]);
  }else{
     pos = (pos != 0)? (pos - 1): 7;
     digitalWrite(p1, half_step_seq[pos][0]);
     digitalWrite(p2, half_step_seq[pos][1]);
     digitalWrite(p3, half_step_seq[pos][2]);
     digitalWrite(p4, half_step_seq[pos][3]);
  }
}

void SMotor::coilsOff(){
   digitalWrite(p1, LOW);
   digitalWrite(p2, LOW);
   digitalWrite(p3, LOW);
   digitalWrite(p4, LOW);
}

void SMotor::stop(){
  coilsOff();
  dir = 0;
  m_ready = true;
  steps = 0;
}

//********* STEP UPDATE **************  
int32_t SMotor::update(){
   uint32_t us = micros();
   
   if(dir != 0 && (us - old_time) > spd){
     
     if(steps == 0){    //after the last step, the system turns off the motor
       coilsOff();
       dir = 0;
       m_ready = true;
       return -1;
     }
     
     steps--;
     old_time = us;
     if( dir == 1){
       forward_one();
     }else{
       backward_one();
     }   
   }
   return tot_steps - steps;
}  

//********* DEBUGGING ***************
void SMotor::print(){
  Serial.println(p1);
  Serial.println(p2);
  Serial.println(p3);
  Serial.println(p4);
  Serial.println(dir);
  Serial.println(pos);
  Serial.println(spd);
  Serial.println(steps);
  Serial.println(old_time);
  #ifdef SM_DBG
  ledOn();
  delay(1000);
  ledOff();
  delay(1000);
  #endif
}

