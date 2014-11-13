/*!
 \file      utility.cpp
 \brief     Set of utility functions.
 \author    Francesco Giurlanda
 \version   0.1
 \date      2014

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
            To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
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
