/*!
 \file      Utensil.h
 \brief     The file contains the base class which implement the control of an utensils.
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
            To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#ifndef Utensil_h
#define Utensil_h

class Utensil {
public:
	//! \brief It performs an action before the motion along the X, Y, Z axes.
	virtual void preMotion() = 0;
	//! \brief It performs an action after the motion along the X, Y, Z axes.
	virtual void postMotion() = 0;
};

#endif //Utensil_h
