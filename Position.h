/*! 
    \file      Position.h
    \brief     Class to manage cartesian position 
    \details   The class manages the position on a cartesian plane.
    \author    Francesco Giurlanda
    \version   0.0.1
    \date      2014

    \copyright Quest'opera è stata rilasciata con licenza Creative Commons Attribuzione 
               - Condividi allo stesso modo 4.0 Internazionale. Per leggere una copia della 
               licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
 */



#ifndef Position_h
#define Position_h

#include "Arduino.h"
#include <stdint.h>


/*! \class PositionXY
    \brief Class to manage cartesian position.
*/
class PositionXY{
  float x, y;
  public:
  
  PositionXY();
  PositionXY(float px, float py);
  PositionXY(const PositionXY &p);
  
  float X() const;
  PositionXY& X(float px);
  float Y() const;
  PositionXY& Y(float py);
  
  float dist(const PositionXY &p);
  float distX(const PositionXY &p);
  float distY(const PositionXY &p);
  
  float angle(const PositionXY &p);
  
  PositionXY operator +(const PositionXY &p) const;
  PositionXY& operator +=(const PositionXY &p);
};

#endif

