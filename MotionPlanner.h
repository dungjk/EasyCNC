/*!
 \file      MotionPlanner.h
 \brief     Data structure that plans the machine motions.
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
            To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#ifndef MOTIONPLANNER_H_
#define MOTIONPLANNER_H_

#include "Arduino.h"
#include "P_Info.h"

#define BUFF_LEN 20  //!< Max number of linear motions in the buffer.

/*! \struct LinearMotion
 *  \brief The structure keeps the data of a linear motion.
 *  \details The structure keeps the data of a linear motion:
 *           coordinates and delay between each step update.
 *  \author Francesco Giurlanda
 */
struct LinearMotion {
	uint16_t delay;				//!< It will be the value of the OCR3A register.
	int32_t steps_x, steps_y, steps_z;  //!< They could be negative, it specify the direction.
	float act_fr;     //!< The feed rate related to this linear motion. It is useful to get the current feed rate of the linear motion in execution
};

typedef struct LinearMotion LM;

/*! \class MotionPlanner
 *  \brief The class keeps all the G-Code command parsed and
 *  	   translated into linear motion sequences.
 *  \author Francesco Giurlanda
 */
class MotionPlanner {
	LM movs[BUFF_LEN];   //!< Circular  buffer of linear motions.
	uint8_t last, first;
	boolean empty;     //!< It is false if at least one element is stored in the buffer, true otherwise.
	PinInfo pin_cooling;  //!< Pin that controls the cooling of the motors drivers
public:
	/*! Default constructor
	 * \param pcl Pin connected to the motors drivers cooling system. -1 if there is not a cooling system
	 */
	MotionPlanner(int8_t pcl);
	/*! \brief The function returns a LinearMotion object through a
	 *         return argument and a boolean value that is false if the operation is done, true otherwise.
	 *  \param mov It is a reference to a return value that will store the LinearMotion object.
	 *  \return The function returns false if the extraction is done, true otherwise.
	 */
	boolean getMotion(LM &mov);
	/*!
	 * \brief The function adds a LinearMotion object into the motion planner buffer.
	 * \param mov LinearMotion object that plans a further motion
	 * \return It returns false if the insertion is done, true otherwise.
	 */
	boolean addMotion(const LM &mov);

	/*! \brief It empties the buffer.
	 */
	void clear();

	/*! \brief It returns the status of the buffer
	 *  \return True if it is empty, false otherwise.
	 */
	boolean isEmpty() const;

	/*! \brief It returns how many free position are into the buffer
	 *  \return Number of free buffer slots
	 */
	int getFreeBuffSize() const;
};

#endif /* MOTIONPLANNER_H_ */
