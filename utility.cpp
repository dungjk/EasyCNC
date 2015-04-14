/*!
 \file      utility.cpp
 \brief     Set of utility functions.
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
            To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#include "utility.h"

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

void removeSpaces(String &l) {
	int len = l.length();
	int i = 0;
	while (i < len) {
		if (l[i] == ' ' || l[i] == '\n' || l[i] == '\r') {
			l.remove(i, 1);
			len--;
		} else
			i++;
	}
}


boolean getFloat(uint8_t &pos, float &val, String &l) {
	int ptr = pos;
	while ((l[ptr] >= '0' && l[ptr] <= '9') || l[ptr] == '.'
			|| l[ptr] == '-' || l[ptr] == '+') {
		ptr++;
	}
	if (ptr == pos)
		return true;
	String tmp = l.substring(pos, ptr);
	pos = ptr;
	val = tmp.toFloat();
	return false;
}


boolean getControlComm(char &code, float &val, String &l) {
	uint8_t len = l.length();
	uint8_t pos = 1;

	if (pos == len) {
		return false;
	}
	if (l[pos] < 'a' || l[pos] > 'z') {
		return false;
	}
	code = l[pos];
	pos++;

	if (pos == len) {
		val = 0;
		return true;
	}


	if (getFloat(pos, val, l)) {
		return false;
	}
	return true;
}
