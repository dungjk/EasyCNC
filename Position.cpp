//  Quest'opera è stata rilasciata con licenza Creative Commons Attribuzione 
//  - Condividi allo stesso modo 4.0 Internazionale. 
//  Per leggere una copia della licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
//
//  Autor: Francesco Giurlanda
//  Project: Easy CNC
//  versione: 0.0.1


#include "Position.h"

  PositionXY::PositionXY(): x(0.0), y(0.0) {};
  PositionXY::PositionXY(float px, float py) : x(px), y(py) {};
  PositionXY::PositionXY(const PositionXY &p) : x(p.x), y(p.y){};
  
  inline float       PositionXY::X() const        { return x;}
  PositionXY&        PositionXY::X(float px)      { x = px; return *this;}
  inline float       PositionXY::Y() const        { return y;}
  PositionXY&        PositionXY::Y(float py)      {y = py; return *this;}
  
  inline float PositionXY::dist(const PositionXY &p){
    return sqrt(sq(p.x - x) + sq(p.y - y));
  }
  
  float PositionXY::distX(const PositionXY &p) { return p.x -x;}
  float PositionXY::distY(const PositionXY &p) { return p.y -y;}
  
  inline float PositionXY::angle(const PositionXY &p) { return atan2(p.y -y, p.x -x);}
  
  PositionXY PositionXY::operator +(const PositionXY &p) const{
    return PositionXY(x + p.x, y + p.y);
  }
  
  PositionXY& PositionXY::operator +=(const PositionXY &p){
    x += p.x; y += p.y;
    return *this;
  }
  

