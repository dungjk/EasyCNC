//  Quest'opera è stata rilasciata con licenza Creative Commons Attribuzione 
//  - Condividi allo stesso modo 4.0 Internazionale. 
//  Per leggere una copia della licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
//
//  Autor: Francesco Giurlanda
//  Project: Easy CNC
//  versione: 0.0.1


#include "CNCxy.h"

CNCxy::CNCxy(): mx(), my(), old_p(), end_p(), actual_p(), spmmx(0.0), spmmy(0.0), v_max_x(0.0), v_max_y(0.0), pin_ls_x_down(-1), pin_ls_y_down(-1), pin_ls_x_up(-1), pin_ls_y_up(-1) {}
CNCxy::CNCxy(float spx, float spy, float vmx, float vmy) : mx(), my(), old_p(), end_p(), actual_p(), spmmx(spx), spmmy(spy), v_max_x(vmx), v_max_y(vmy), pin_ls_x_down(-1), pin_ls_y_down(-1), pin_ls_x_up(-1), pin_ls_y_up(-1) {}
  
void CNCxy::setMotorX(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4){
  mx.setPins(pin1, pin2, pin3, pin4);
}

void CNCxy::setMotorY(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4){
  my.setPins(pin1, pin2, pin3, pin4);
}

void CNCxy::resetPos() { old_p = end_p = actual_p = PositionXY(0.0, 0.0); }
  
void CNCxy::moveTo(float px, float py){
  float dx, dy;
  
  old_p = actual_p;
  end_p.X(px).Y(py);
  
  if(pos_type){
    dx = PositionXY().distX(end_p);
    dy = PositionXY().distY(end_p);
  }else{
    dx = actual_p.distX(end_p);
    dy = actual_p.distY(end_p);
  }
    
  float steps_x = abs(dx * spmmx); // (mm * steps/mm = steps)
  float steps_y = abs(dy * spmmy);
  
  float tx = steps_x / v_max_x;
  float ty = steps_y / v_max_y;
  float v_x, v_y;
  
  if(tx > ty){
    v_y = steps_y / tx;
    v_x = v_max_x;
  }else{
    v_x = steps_x / ty;
    v_y = v_max_y;
  }
  
  if(dx > 0.0){
    mx.forward(steps_x, 1000000 / v_x);  // the delay per step is in microseconds
  }else if(dx < 0.0){
    mx.backward(steps_x, 1000000 / v_x);
  }
  
  if(dy > 0.0){
    my.forward(steps_y, 1000000 / v_y);
  }else if(dy < 0.0){
    my.backward(steps_y, 1000000 / v_y);
  }
  
}

void CNCxy::moveTo(const PositionXY &np){
  moveTo(np.X(), np.Y());
}

void CNCxy::stopMotion(){
  mx.stop();
  my.stop();
}
  
boolean CNCxy::update() {
  int32_t a = mx.update();
  int32_t b = my.update();
  if(a == -1 || b == -1){
    actual_p = end_p;
  }else{
    actual_p = old_p + PositionXY(a/spmmx, b/spmmy);
  }
  
  return a == -1 && b == -1;
}

void CNCxy::highPrecision(){
  mx.setHalfStep();
  my.setHalfStep();
}

void CNCxy::lowPrecision(){
  mx.setFullStep();
  my.setFullStep();
}

void CNCxy::setIncrPos(){
  pos_type = true;
}

void CNCxy::setAbsolPos(){
  pos_type = false;
}

void CNCxy::searchHomePos(){
  mx.backward(100000, 1000000 / v_max_x);
  while(digitalRead(pin_ls_x_down) != LOW){
      mx.update();
  }
  mx.stop();
  delay(500);
  
  my.backward(100000, 1000000 / v_max_y);
  while(digitalRead(pin_ls_y_down) != LOW){
      my.update();
  }
  my.stop();
  
  resetPos();
}

void CNCxy::setLimitSwitchX(int8_t dw, int8_t up){
  pin_ls_x_down = dw;
  if(pin_ls_x_down >= 0){
    pinMode(pin_ls_x_down, INPUT_PULLUP);
  }
  
  pin_ls_x_up = up;
  if(pin_ls_x_up >= 0){
    pinMode(pin_ls_x_up, INPUT_PULLUP);
  }
}

void CNCxy::setLimitSwitchY(int8_t dw, int8_t up){
   pin_ls_y_down = dw;
  if(pin_ls_y_down >= 0){
    pinMode(pin_ls_y_down, INPUT_PULLUP);
  }
  
  pin_ls_y_up = up;
  if(pin_ls_y_up >= 0){
    pinMode(pin_ls_y_up, INPUT_PULLUP);
  }
}

