/*
 * GCode.h
 *
 *  Created on: 26/lug/2014
 *      Author: francescogiurlanda
 */

#ifndef GCODE_H_
#define GCODE_H_

#include "config.h"
#include "GCode_def.h"
#include "Arduino.h"
#include "CNC_Router.h"
#include "Utensils.h"
#include <stdint.h>

/*! \class GCode
 *  \brief The class interprets lines of RS274/NGC language.
 *  \details The class implements an interpreter for lines of RS274/NGC language.
 *           It is a language to control CNC.
 *  \author    Francesco Giurlanda
 */
class GCode {
	/*! \brief The function gets a word of G-Code.
	 *  \details The function processes a line of G-Code. It is stored into the public variable line and the function returns the first occurrence of word of G-Code
	 *           from the specified position.
	 *  \param code It is a return value and it is going to contain an upper case letter A-Z that identify the class of the command.
	 *  \param val It is a return value and it is going to contain a numeric value associated to the code.
	 *  \param pos Here we must specify the position in the line where the function starts to find the word.
	 *             At the end, the function returns in it the position of the next letter after the found word.
	 *  \param len The max length of the line.
	 *  \return It returns a boolean value that is true if the operation ends correctly, false if is occurred an error. See GCode::parser_status to get more information about the error.
	 */
	boolean getWord(char &code, float &val, uint8_t &pos, uint8_t len);
	void cycleG81();
	void motionG2G3();
	void motionG0G1();

public:
	String line;          //!< Here is stored the G-Code line that will be parsed
	float feed_rate;      //!< The variable stores the feed rate during the working process in unit/s (unit can be mm or inch)
	float spindle_speed;	  //!< The speed of the drill

	uint8_t parser_status;    //!< The status of the parser. \sa GCode_def.h
	uint8_t last_word[16];    //!< Here the class stores the last word red for each group. \sa GCode_def.h
	PositionXYZ new_pos;    //!< The class stores the new position red from the G-Code line
	//float new_pos_z;		  //!< The class stores here the new position red from the G-code line
	float params[PARAMS];     //!< The array stores the values of the parameters red from the G-Code line. \sa GCode_def.h
	boolean pars_spec[PARAMS];//!< The array says which parameters were found in parsed G-Code line. \sa GCode_def.h

	CNC_Router *router;       //!< Pointer to the CNC_Router object

	MillingMachine *utensil;   //!< Pointer to the MillingMachine object


	/*! \brief Constructor
	 * 	\param r Address of the CNC_Router object
	 * 	\param m Address of the MillingMachine object
	 */
	GCode(CNC_Router *r, MillingMachine *m);

	/*! \brief The function parses a line of G-Code.
	 *  \details In this function is implemented the action for the recognized words of GCode.
	 *           It also executes control commands that starts with the char "$".
	 *  \return The function return the parser status.
	 */
	int parseLine();

	//! \brief The function removes all spaces and \r \n char from the variable GCode::line.
	void removeSpaces();

	/*! \brief The function reads a integer value from the variable GCode::line at the specified position.
	 *  \param p It mast contains the position in the GCode::line from where the function starts to read an integer. The variable return a value equal the the position of the next char after the integer.
	 *  \return  The integer value that has been red from the GCode::line
	 */
	int getInt(int &p);//TODO: make it similar to getFloat, mi sa che non è più stata usata questa funzione

	//! \brief The function calls the update functions to perform the motors motion. See CNC_Router::update and MillingMachine::update.
	int runMotion();

	/*! \brief The function reads a float value in the GCode::line variable, from the specified position and return false if it is ok, else true if the function is arrived at the end of the line.
	 *  \param pos Is the position where it starts to read a float value. The function returns here the position of the next char after the float value.
	 *  \param val  The function returns here the red value
	 *  \return The function returns false if it is ok, true if the line is end.
	 */
	boolean getFloat(uint8_t &pos, float &val);

	/*! \brief It returns the status of the CNC machine.
	 *  \details The function sends the status of the CNC machine on the serial link.
	 *           It is a string of number separated by ":". The string is formatted as follow:
	 *           "X:Y:Z:FEED_RATE:ERR_MSG".
	 */
	void returnStatus();

	//! \brief In case of status of error, the function resets the parser at the STATUS_OK.
	void resetStatus();
};

#endif /* GCODE_H_ */
