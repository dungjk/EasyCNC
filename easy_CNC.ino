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


/*
#define MX_STEPS_PER_ROUND 48*2
#define MY_STEPS_PER_ROUND 96*2
// the unit for the speed is steps/s
#define MX_SPEED 300.0
#define MY_SPEED 300.0
*/


PenControl mypen(3, 112, 80);

CNCxy mycnc(MX_STEPS_PER_ROUND, MY_STEPS_PER_ROUND, MX_SPEED, MY_SPEED);  //200.0 at lowPrecision

boolean end_task = false;



void processPos(){
  float new_x, new_y;
  new_x = Serial.parseFloat();
  new_y = Serial.parseFloat();
  Serial.read();
  mycnc.moveTo(new_x, new_y);
}

void setup(){
  mycnc.setMotorX(4, 5, 6, 7);
  mycnc.setMotorY(8, 9, 10, 11);
  mycnc.resetPos();
  mycnc.highPrecision();
  mycnc.setAbsolPos();
  
  mypen.init();
  //pen.write(80);
  
  Serial.begin(9600);

}

void loop(){
  if( mycnc.update()){
    
    if(end_task){
      Serial.print('a');
      end_task = false;
    }
    
    if(Serial.available()){
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
      }

      end_task = true;
    }
  }

}



/*
SMotor m1(4, 5, 6, 7);
SMotor m2(8, 9, 10, 11);
Servo pen;

boolean vm1, vm2;

boolean ok = false;

int d;
int s;
int v;
int p;

void penUp(){
  pen.write(80);
  delay(500);
}

void penDown(){
  pen.write(109);
  delay(500);
}


void setup() {
  
  Serial.begin(9600);
  
   Serial.println("Setup...");
   m1.print();
   m2.print();
   pen.attach(3);
   pen.write(80);

}

void loop(){
  vm1 = m1.update();
  vm2 = m2.update();
  
  if( vm1 && vm2){
    if(ok){
      Serial.println("OK");
      ok = false;
    }
    if (Serial.available() > 0) {
      p = Serial.parseInt();
      //Serial.print("Pen: ");
      //Serial.println(p);
      if(p == 0)
        penUp();
      else if(p == 1)
        penDown();
      
      
      d = Serial.parseInt();
      //Serial.print("Dir: ");
      //Serial.print(d);
      
      s = Serial.parseInt();
      //Serial.print("; Steps: ");
      //Serial.print(s);
      
      //Serial.print("; Speed: ");
      v = Serial.parseInt();
      //Serial.println(v);
      
      if(d == 1)
        m1.forward(s, v*1000);
      else if(d == -1)
        m1.backward(s, v*1000);
        
      d = Serial.parseInt();
      //Serial.print("Dir: ");
      //Serial.print(d);
      
      s = Serial.parseInt();
      //Serial.print("; Steps: ");
      //Serial.print(s);
      
      //Serial.print("; Speed: ");
      v = Serial.parseInt();
      //Serial.println(v);
      Serial.read();
      
      if(d == 1)
        m2.forward(s, v*1000);
      else if(d == -1)
        m2.backward(s, v*1000);
        
      ok = true;
    }
  }

}

*/

