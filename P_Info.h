/*!
 \file      P_Info.h
 \brief     Data structure that keeps the info about the register of a specified pin.
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
            To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#ifndef P_INFO_H_
#define P_INFO_H_

/*! \struct P_Info
 *  \brief It keeps the info about the register of a specified pin
 */
struct P_Info {
	uint8_t bit;  //!< The bit mask of the pin in the register.
	volatile uint8_t *outreg; //!< The output register of the specified pin.

	inline void set() {*outreg |= bit;};      //<! It requires an atomic block, use cli() and sei() functions
	inline void reset() {*outreg &= ~bit;};   //<! It requires an atomic block, use cli() and sei() functions
};

typedef struct P_Info PinInfo;



#endif /* P_INFO_H_ */
