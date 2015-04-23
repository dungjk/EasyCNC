/*!
 \file      utility.h
 \brief     Set of utility functions.
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#ifndef UTILITY_H_
#define UTILITY_H_
#include "Arduino.h"

/* \brief The function returns the pin number of a specified interrupt.
 * \details The function recognize the arduino board to match pin number and interrupt number.
 * \param i Interrupt number.
 * \return The pin number. It returns 0 if no match was found.
 */
int getPinFromInterrupt(int i);

/*! \brief The function removes all spaces and \\r \\n chars from the string.
 *  \param l String
 */
void removeSpaces(String &l);

/*! \brief The function reads a float value in the String variable, from the specified position and return false if it is ok, else true if the function is arrived at the end of the line.
 *  \param pos Is the position where it starts to read a float value. The function returns here the position of the next char after the float value.
 *  \param val  The function returns here the red value
 *  \param l The string
 *  \return The function returns false if it is ok, true if the line is end.
 */
boolean getFloat(uint8_t &pos, float &val, String &l);

boolean getControlComm(char &code, float &val, String &l);

#endif /* UTILITY_H_ */
