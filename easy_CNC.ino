//  Quest'opera è stata rilasciata con licenza Creative Commons Attribuzione 
//  - Condividi allo stesso modo 4.0 Internazionale. 
//  Per leggere una copia della licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
//
//  Autor: Francesco Giurlanda
//  Project: Easy CNC
//  versione: 0.0.1

#include "config.h"
#include "Utensils.h"
#include <Servo.h>
#include "Position.h"
#include "CNCxy.h" 

PlotterServo mypen(_PLOTTER_SERVO_PIN,_PLOTTER_SERVO_DOWN_POS, _PLOTTER_SERVO_UP_POS);

CNC_Router mycnc(ROUTER_MX_STEPS_PER_MM, ROUTER_MY_STEPS_PER_MM, ROUTER_MX_SPEED, ROUTER_MY_SPEED);  //200.0 at lowPrecision

boolean end_task = false;


void stopButton(){
  mycnc.stopMotion();
}

void processPos(){
  float new_x, new_y;
  new_x = Serial.parseFloat();
  new_y = Serial.parseFloat();
  Serial.read();
  mycnc.moveTo(new_x, new_y);
}

void setup(){
  mycnc.initMotorX();
  mycnc.initMotorY();
  mycnc.resetPos();
  mycnc.lowPrecision();
  mycnc.setAbsolPos();
  mycnc.setLimitSwitchX(ROUTER_LIMIT_X);
  mycnc.setLimitSwitchY(ROUTER_LIMIT_Y);

  attachInterrupt(INTERRUPT_STOP_MOTION, stopButton, FALLING);
  digitalWrite(INTERRUPT_STOP_MOTION, HIGH);

  mypen.init();

  Serial.begin(9600);

}

void loop(){
  if( mycnc.update()){
    
    if(end_task){
      Serial.print('a');
      end_task = false;
    }
    
    if(Serial.available() > 0){
      switch( Serial.parseInt() ) {
        case 0:
           mypen.up();
           processPos();
           break;
        case 1:
           mypen.down();
           processPos();
           break;
        case 2:
           mycnc.resetPos();
           break;
        case 3: 
           mycnc.searchHomePos();
      }

      end_task = true;
    }
  }

}

