/*!
 \file      GCode.h
 \brief     G-Code parser.
 \details   The class allows to parse the G-Code lines and runs the operation to actuate the command.
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
            To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#ifndef GCODE_H_
#define GCODE_H_

#include "GCode_def.h"
#include "Arduino.h"
#include "CNCRouterISR.h"
#include "MillingMachine.h"
#include "Laser.h"
#include "PlotterServo.h"
#include "utility.h"
#include <stdint.h>
#include "config.h"

#define STATUS_FEEDBACK 3000  //15650   //!< It allows about a feedback rate of once per second

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
	 *  \return It returns a boolean value that is true if the operation ends correctly, false if is occurred an error. See GCode::parser_status to get more information about the error.
	 */
	boolean getWord(char &code, float &val, uint8_t &pos);
	//boolean getControlComm(char &code, float &val);
	void cycleG81();
	void motionG2G3();
	void motionG0G1();
	void waitMotionFinish();

	volatile boolean sync;              //!< It synchronizes the Serial print of the functions sendAck() and the status send

public:
	String line;                //!< Here is stored the G-Code line that will be parsed
	float feed_rate;            //!< The variable stores the feed rate during the working process in unit/s (unit can be mm or inch)
	float spindle_speed;	    //!< The speed of the drill

	uint8_t parser_status;      //!< The status of the parser. \sa GCode_def.h
	uint8_t last_word[16];      //!< Here the class stores the last word red for each group. \sa GCode_def.h
	PositionXYZ new_pos;        //!< The class stores the new position red from the G-Code line
	float params[PARAMS];       //!< The array stores the values of the parameters red from the G-Code line. \sa GCode_def.h
	boolean pars_spec[PARAMS];  //!< The array says which parameters were found in parsed G-Code line. \sa GCode_def.h

	CNC_Router_ISR *router;        //!< Pointer to the CNC_Router object


#ifdef _PLOTTER_SERVO
	PlotterServo *utensil;    //!< Pointer to the MillingMachine object
#endif
#ifdef _MILLING_MACHINE
	MillingMachine *utensil;    //!< Pointer to the MillingMachine object
#endif
#ifdef _LASER
	Laser * utensil;             //!< Pointer to the Laser object
#endif


	/*! \brief Constructor
	 * 	\param r Address of the CNC_Router object
	 * 	\param m Address of the Utensil object
	 */
	GCode(CNC_Router_ISR *r, Utensil *m);

	//! \brief It initializes the timer that manage the status feedback to the GUI controller. It mast be call into the setup function.
	void init();

	/*! \brief The function parses a line of G-Code.
	 *  \details In this function is implemented the action for the recognized words of GCode.
	 *           It also executes control commands that starts with the char "$".
	 *  \return The function return the parser status.
	 */
	int parseLine();

	/*! \brief The function reads a integer value from the variable GCode::line at the specified position.
	 *  \param p It mast contains the position in the GCode::line from where the function starts to read an integer. The variable return a value equal the the position of the next char after the integer.
	 *  \return  The integer value that has been red from the GCode::line
	 */
	int getInt(int &p);//TODO: make it similar to getFloat, mi sa che non è più stata usata questa funzione

	/*! \brief It sends a response after a reception of GCode line, with the state of the parser
	 *  \details The function sends a string through the serial connection, the information sent are
	 *           the G-Code parser status and the free space in the
	 */
	void sendAck();

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
