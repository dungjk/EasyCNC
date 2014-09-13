/*
 * GCode.cpp
 *
 *  Created on: 26/lug/2014
 *      Author: francescogiurlanda
 */

#include "GCode.h"

GCode::GCode(CNC_Router *rt) {
	router = rt;
}

int GCode::getInt(int &i){
  int ptr = i;
  while((line[ptr] >= '0' && line[ptr] <= '9') || line[ptr] == '-' || line[ptr] == '+'){
     ptr++;
  }
  String tmp = line.substring(i, ptr);
  i = ptr - 1;
  return tmp.toInt();
}

float GCode::getFloat(int &i){
  int ptr = i;
  while((line[ptr] >= '0' && line[ptr] <= '9') || line[ptr] == '.' || line[ptr] == '-' || line[ptr] == '+'){
     ptr++;
  }
  String tmp = line.substring(i, ptr);
  i = ptr - 1;
  return tmp.toFloat();
}

int GCode::parseLine(){
        removeSpaces();
        int len = line.length();
        int ptr = 0;
        char type;
	for(int i = 0; i < len ; i++) {
		if(line[i] >= 'A' && line[i] <= 'Z' ){
                    type = line[i];
                    i++;
                    switch (type){
                      case 'G':
                      case 'M':
                        Serial.print(type);
                        Serial.println(getInt(i));
                        break;
                      case 'X':
                      case 'Y':
                      case 'Z':
                        Serial.print(type);
                        Serial.println(getFloat(i));
                        break;


                    };

		}
	}

}

void GCode::removeSpaces(){
  int len = line.length();
  int i = 0;
  while(i < len){
    if(line[i] == ' '){
       line.remove(i, 1);
       len--;
    }
    else
       i++;
  }
}
