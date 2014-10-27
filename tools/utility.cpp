/*!
 \file      utility.cpp
 \brief     Set of utility functions.
 \author    Francesco Giurlanda
 \version   0.0.1
 \date      2014

 \copyright Quest'opera �� stata rilasciata con licenza Creative Commons Attribuzione
 - Condividi allo stesso modo 4.0 Internazionale. Per leggere una copia della
 licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
 */

int getPinFromInterrupt(int i){
#if defined(__AVR_ATmega2560__)
	switch (i){
	case 0:
		return 2;
	case 1:
		return 3;
	case 2:
		return 21;
	case 3:
		return 20;
	case 4:
		return 19;
	case 5:
		return 18;
	};
#endif
	/* you can add other controls here to support other Arduino boards. */
	return 0;
}
