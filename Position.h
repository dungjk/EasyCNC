/*! 
    \file      Position.h
    \brief     Class to manage Cartesian position
    \details   The class manages the position on a Cartesian plane.
    \author    Francesco Giurlanda
    \version   0.0.1
    \date      2014

    \copyright Quest'opera �� stata rilasciata con licenza Creative Commons Attribuzione
               - Condividi allo stesso modo 4.0 Internazionale. Per leggere una copia della 
               licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
 */



#ifndef Position_h
#define Position_h

#include "Arduino.h"
#include <stdint.h>


/*! \class PositionXY
    \brief Class to manage Cartesian positions.
    \details The class provides the basic operations between points of two coordinates.
    \author    Francesco Giurlanda
*/
class PositionXY{
  float x, y;
  public:
  
  //! \brief Default constructor.
  PositionXY();
  //! \brief Constructor.
  PositionXY(float px, float py);
  //! \brief Copy constructor.
  PositionXY(const PositionXY &p);
  
  /*! \brief It returns the x-axis value.
   *  \return X-axis value.
   */
  float X() const;

  /*! \brief It configures the x-axis value.
   *  \param px The x-axis value.
   *  \return The reference to this object.
   */
  PositionXY& X(float px);

  /*! \brief It returns the y-axis value.
   *  \return Y-axis value.
   */
  float Y() const;

  /*! \brief It configures the y-axis value.
   *  \param py The y-axis value.
   *  \return The reference to this object.
   */
  PositionXY& Y(float py);
  
  /*! \brief It returns the distance between two positions.
   *  \details The distance between \f$(x_1,y_1)\f$ and \f$(x_2,y_2)\f$ is computed according the formula \f$\sqrt{(x_2-x_1)^2+(y_2-y_1)^2}\f$.
   *  \param p The reference to the next position.
   *  \return The distance value.
   */
  float dist(const PositionXY &p);

  /*! \brief It returns the x-axis distance between two positions.
   *  \details The x-axis distance between \f$(x_1,y_1)\f$ and \f$(x_2,y_2)\f$ is computed according the formula \f$(x_2-x_1)\f$.
   *  \param p The reference to the next position.
   *  \return The x-axis distance value.
   */
  float distX(const PositionXY &p);

  /*! \brief It returns the y-axis distance between two positions.
   *  \details The y-axis distance between \f$(x_1,y_1)\f$ and \f$(x_2,y_2)\f$ is computed according the formula \f$(y_2-y_1)\f$.
   *  \param p The reference to the next position.
   *  \return The y-axis distance value.
   */
  float distY(const PositionXY &p);
  
  /*! \brief It returns the angle between the line crossing the two point end the x-axis.
   *  \details The angle is computed with the formula \f$atan2(y_2 - y_1,x_2 - x_1)\f$.
   *  \param p The reference to the next position.
   *  \return The angle between the line crossing the two point end the x-axis.
   */
  float angle(const PositionXY &p);
  
  /*! \brief It returns a position where each axis value is the sum of the values of the two positions.
   *  \param p The reference to the added position.
   *  \return The position which is the sum of two positions
   */
  PositionXY operator +(const PositionXY &p) const;

  /*! \brief It adds to the current position the specified position.
   *  \param p The position to add.
   *  \return the reference to this object.
   */
  PositionXY& operator +=(const PositionXY &p);
};

#endif

