/*
 * P_Info.h
 *
 *  Created on: 29/mar/2015
 *      Author: francescogiurlanda
 */

#ifndef P_INFO_H_
#define P_INFO_H_

/*! \struct PinInfo
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
