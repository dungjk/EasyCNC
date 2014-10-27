/*!
 \file      utility.h
 \brief     Set of utility functions.
 \author    Francesco Giurlanda
 \version   0.0.1
 \date      2014

 \copyright Quest'opera �� stata rilasciata con licenza Creative Commons Attribuzione
 - Condividi allo stesso modo 4.0 Internazionale. Per leggere una copia della
 licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
 */

#ifndef UTILITY_H_
#define UTILITY_H_

/* \brief The function returns the pin number of a specified interrupt.
 * \details The function recognize the arduino board to match pin number and interrupt number.
 * \param i Interrupt number.
 * \return The pin number. It returns 0 if no match was found.
 */
int getPinFromInterrupt(int i);


#endif /* UTILITY_H_ */
