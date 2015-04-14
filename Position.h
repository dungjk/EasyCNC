/*! 
 \file      Position.h
 \brief     Class to manage Cartesian position
 \details   The class manages the position on a Cartesian plane.
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
            To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#ifndef Position_h
#define Position_h

#include "Arduino.h"
#include <stdint.h>

/*! \class PositionXY
 \brief Class to manage Cartesian positions.
 \details The class provides the basic operations to operate with points of two X/Y coordinates.
 \author    Francesco Giurlanda
 */
class PositionXY {
	float x, y;
public:

	//! \brief Default constructor.
	PositionXY();

	/*! \brief Constructor.
	 *  \param px The X-axis coordinate
	 *  \param py The Y-axis coordinate
	 */
	PositionXY(float px, float py);

	/*! \brief Copy constructor.
	 *  \param p The object that we want copy.
	 */
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

	/*! \brief It return the module of the equivalent vector
	 * 	\return The module.
	 */
	float module();

	/*! \brief It returns the distance between two positions.
	 *  \details The distance between \f$(x_1,y_1)\f$ and \f$(x_2,y_2)\f$ is computed according the formula \f$\sqrt{(x_2-x_1)^2+(y_2-y_1)^2}\f$.
	 *  \param p The reference to the next position.
	 *  \return The distance value.
	 */
	float module(const PositionXY &p);

	/*! \brief It returns the x-axis distance between two positions.
	 *  \details The x-axis distance between \f$(x_1,y_1)\f$ and \f$(x_2,y_2)\f$ is computed according the formula \f$(x_2-x_1)\f$.
	 *  \param p The reference to the next position.
	 *  \return The x-axis distance value.
	 */
	float offsetX(const PositionXY &p);

	/*! \brief It returns the y-axis distance between two positions.
	 *  \details The y-axis distance between \f$(x_1,y_1)\f$ and \f$(x_2,y_2)\f$ is computed according the formula \f$(y_2-y_1)\f$.
	 *  \param p The reference to the next position.
	 *  \return The y-axis distance value.
	 */
	float offsetY(const PositionXY &p);

	/*! \brief It returns the angle of the equivalent vector.
	 *  \return The angle.
	 */
	float angle();

	/*! \brief It returns the angle between the line crossing the two point end the x-axis.
	 *  \details The angle is computed with the formula \f$atan2(y_2 - y_1,x_2 - x_1)\f$.
	 *  \param p The reference to the next position.
	 *  \return The angle between the line crossing the two point end the x-axis.
	 */
	float angle(const PositionXY &p);

	/*! \brief Initializes the equivalent vector with the polar coordinate
	 *  	\param m Module
	 *  	\param a Angle
	 */
	PositionXY& polar(float m, float a);

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

/*! \class PositionXYZ
 \brief Class to manage vectors on a XYZ space.
 \details The class provides the basic operations to operate with vector of three X/Y/Z coordinates.
 \author    Francesco Giurlanda
 */
class PositionXYZ {
	float x, y, z;
public:
	//! \brief Default constructor.
	PositionXYZ();

	/*! \brief Constructor.
	 *  \param px The X-axis coordinate
	 *  \param py The Y-axis coordinate
	 *  \param pz The Z-axis coordinate.
	 */
	PositionXYZ(float px, float py, float pz);

	/*! \brief Copy constructor.
	 *  \param p The object that we want copy.
	 */
	PositionXYZ(const PositionXYZ &p);

	/*! \brief It returns the x-axis value.
	 *  \return X-axis value.
	 */
	float X() const;

	/*! \brief It configures the x-axis value.
	 *  \param px The x-axis value.
	 *  \return The reference to this object.
	 */
	PositionXYZ& X(float px);

	/*! \brief It returns the y-axis value.
	 *  \return Y-axis value.
	 */
	float Y() const;

	/*! \brief It configures the y-axis value.
	 *  \param py The y-axis value.
	 *  \return The reference to this object.
	 */
	PositionXYZ& Y(float py);

	/*! \brief It returns the x-axis value.
	 *  \return Z-axis value.
	 */
	float Z() const;

	/*! \brief It configures the x-axis value.
	 *  \param pz The Z-axis value.
	 *  \return The reference to this object.
	 */
	PositionXYZ& Z(float pz);

	/*! \brief It return the module of the vector
	 * 	\return The module.
	 */
	float module();

	/*! \brief It return the module of the equivalent vector, projected on XY plane
	 * 	\return The module.
	 */
	float moduleXY();

	/*! \brief It returns the module of the difference between the two vectors .
	 *  \details The distance between \f$(x_1,y_1,z_1)\f$ and \f$(x_2,y_2,z_2)\f$ is computed according the formula \f$\sqrt{(x_2-x_1)^2+(y_2-y_1)^2+(z_2-z_1)^2}\f$.
	 *  \param p The reference to the next position.
	 *  \return The distance value.
	 */
	float module(const PositionXYZ &p);

	/*! \brief It returns the module of the difference between the two vectors projected on the XY plane.
	 *  \details The distance between \f$(x_1,y_1,z_1)\f$ and \f$(x_2,y_2,z_2)\f$ is computed according the formula \f$\sqrt{(x_2-x_1)^2+(y_2-y_1)^2}\f$.
	 *  \param p The reference to the next position.
	 *  \return The distance value.
	 */
	float moduleXY(const PositionXYZ &p);

	/*! \brief It returns the x-axis distance between two positions.
	 *  \details The x-axis distance between \f$(x_1,y_1,z_1)\f$ and \f$(x_2,y_2,z_2)\f$ is computed according the formula \f$(x_2-x_1)\f$.
	 *  \param p The reference to the next position.
	 *  \return The x-axis distance value.
	 */
	float offsetX(const PositionXYZ &p);

	/*! \brief It returns the y-axis distance between two positions.
	 *  \details The y-axis distance between \f$(x_1,y_1,z_1)\f$ and \f$(x_2,y_2,z_2)\f$ is computed according the formula \f$(y_2-y_1)\f$.
	 *  \param p The reference to the next position.
	 *  \return The y-axis distance value.
	 */
	float offsetY(const PositionXYZ &p);

	/*! \brief It returns the y-axis distance between two positions.
	 *  \details The y-axis distance between \f$(x_1,y_1,z_1)\f$ and \f$(x_2,y_2,z_2)\f$ is computed according the formula \f$(z_2-z_1)\f$.
	 *  \param p The reference to the next position.
	 *  \return The y-axis distance value.
	 */
	float offsetZ(const PositionXYZ &p);

	/*! \brief It returns the angle of the vector projection on XY plane .
	 *  \return The angle.
	 */
	float angleXY();

	/*! \brief It returns the angle of the vector projection on XY plane, obtained by the difference of the two vectors.
	 *  \details The angle is computed with the formula \f$atan2(y_2 - y_1,x_2 - x_1)\f$.
	 *  \param p The reference to the next position.
	 *  \return The angle between the line crossing the two point end the x-axis.
	 */
	float angleXY(const PositionXYZ &p);

	/*! \brief Initializes the equivalent vector with the polar coordinate, the z value is 0
	 *  	\param m Module
	 *  	\param a Angle
	 */
	PositionXYZ& polarXY(float m, float a);

	/*! \brief It returns a position where each axis value is the sum of the values of the two positions.
	 *  \param p The reference to the added position.
	 *  \return The vector which is the sum of two vectors
	 */
	PositionXYZ operator +(const PositionXYZ &p) const;

	/*! \brief It adds to the current position the specified position.
	 *  \param p The position to add.
	 *  \return The reference to the object.
	 */
	PositionXYZ& operator +=(const PositionXYZ &p);

	/*! \brief It returns a vector that is the difference between two vectors.
	 *  \param p The reference to a vector that will be subtracted.
	 *  \return The vector which is the difference of the two vectors
	 */
	PositionXYZ operator -(const PositionXYZ &p) const;

	/*! \brief It returns the reference to a vector that is the difference between two vectors.
	 *  \param p The reference to a vector that will be subtracted.
	 *  \return The reference to the object.
	 */
	PositionXYZ& operator -=(const PositionXYZ &p);

	/*! \brief The function returns a vector with the same direction of the original one and the module multiplied by the scalar value p.
	 *  \param p Scala value
	 *  \return The vector
	 */
	PositionXYZ operator *(float p);

	/*! \brief The function returns a vector with the same direction of the original one and the module multiplied by the scalar value p.
	 *  \param p Scala value
	 *  \return The reference to the object
	 */
	PositionXYZ& operator *=(float p);

	/*! \brief The function returns a vector with the same direction of the original one and the module divided by the scalar value p.
	 *  \param p Scala value
	 *  \return The vector
	 */
	PositionXYZ operator /(float p);

	/*! \brief The function returns a vector with the same direction of the original one and the module divided by the scalar value p.
	 *  \param p Scala value
	 *  \return The reference to the object
	 */
	PositionXYZ& operator /=(float p);

};

#endif

