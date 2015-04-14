/*!
 \file      GCode_def.h
 \brief     Definition useful to GCode class.
 \details   The file contains all the defines useful to the GCode class.
 \author    Francesco Giurlanda
 \version   0.2
 \date      2015

 \copyright This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
            To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
 */

#ifndef GCODE_DEF_H_
#define GCODE_DEF_H_

#define GROUP_MASK B11110000
#define GROUP(x) (x>>4)

//NON MODAL GROUP
#define GROUP0 0
#define G4 1
#define G10 2
#define G28 3
#define G30 4
#define G53 5
#define G92 6
#define G92_1 7
#define G92_2 8
#define G92_3 9

//MODAL GROUP 1: MOVING
#define GROUP1 1
#define G0 16		//!< Rapid positioning
#define G1 17		//!< Linear interpolation
#define G2 18		//!< Circular interpolation (clockwise)
#define G3 19		//!< Circular interpolation (counter clockwise)
#define G38_2 20
#define G80 21		//!< Delete the moving mode
#define G81 22		//!< Drilling cycle
#define G82 23		//!< Drilling cycle with pause
#define G83 24		//!< Deep drilling cycle with chip breaking
#define G84 25		//!< Internal thread cycle, clockwise
#define G85 26		//!< Boring cycle, without pause, exit at working speed
#define G86 27		//!< Boring cycle, without spindle stop, rapid exit
#define G87 28		//!< Boring back
#define G88 29		//!< Boring cycle, with spindle stop and manual exit
#define G89 30		//!< Boring cycle, with pause and exit at working speed

//MODAL GROUP 2: PLANE SELECTION
#define GROUP2 2
#define G17 32      //!< XY plane selection
#define G18 33		//!< XZ plane selection
#define G19 34		//!< YZ plane selection

//MODAL GROUP 3: DISTANSE MODE
#define GROUP3 3
#define G90 48		//|< Absolute distance mode
#define G91 49		//!< Incremental distance mode

//MODAL GROUP 4: STOP
#define GROUP4 4
#define M0 64
#define M1 65
#define M2 66
#define M30 67
#define M60 68

//MODAL GROUP 5: FORWARDING MODE
#define GROUP5 5
#define G93 80		//!< Forwarding mode in time inverse
#define G94 81		//!< Forwarding mode in units per minute

//MODEL GROUP 6: UNIT
#define GROUP6 6
#define G20 96		//!< Inches
#define G21 97		//!< Millimeters

//MODAL GROUP 7: UTENSIL OFFSET COMPENSATION
#define GROUP7 7
#define G40 112		//!< Disable the radius compensation
#define G41 113		//!< Start the left utensil radius compensation
#define G42 114		//!< Start the right utensil radius compensation

//MODAL GROUP 8: UTENSIL LENGTH
#define GROUP8 8
#define G43 128		//!< Utensil length offset (positive value)
#define G49 129		//!< Delete utensil length offset

//MODAL GROUP 9: SWITCH ON/OFF FEED OR SPINDLE OVERRIDE
#define GROUP9 9
#define M48 144
#define M49 145

//MODAL GROUP 10: MODE OF LOOP RETOURN
#define GROUP10 10
#define G98 160		//!< Initial level of return from a loop
#define G99 161		//!< Point R of return from a loop

//MODAL GROUP 11: CHANGE UTENSIL
#define GROUP11 11
#define M6 176

//MODAL GROUP 12: COORDINATE SELECTION
#define GROUP12 12
#define G54 192		//!< Coordinate job 1
#define G55 193		//!< Coordinate job 2
#define G56 194		//!< Coordinate job 3
#define G57 195		//!< Coordinate job 4
#define G58 196		//!< Coordinate job 5
#define G59 197		//!< Coordinate job 6
#define G59_1 198	//!< Coordinate job 7
#define G59_2 199	//!< Coordinate job 8
#define G59_3 200	//!< Coordinate job 9

//MODAL GROUP 13: PATH CONTROL
#define GROUP13 13
#define G61 208		//!< Exact path mode
#define G61_1 209	//!< Exact stop mode
#define G64 210		//!< Continuous mode

//MODAL GROUP 14: SPINDLE ROTATION
#define GROUP14 14
#define M3 224      //!< Power on spindle (CW)
#define M4 225		//!< Power on spindle (CCW)
#define M5 226		//!< Power off spindle

//MODAL GROUP 15: COOLING (M7 AND M8 CAN BE ACTIVE TOGETER IN THE SAME TIME)
#define GROUP15 15
#define M7 240
#define M8 241
#define M9 242

//PARAMETER UNSPECIFIED
#define UNSPECIFIED 0

//other parameters
#define PARAMS	12
#define PARAM_X 0
#define PARAM_Y 1
#define PARAM_Z 2
#define PARAM_A 3
#define PARAM_B 4
#define PARAM_C 5
#define PARAM_P 6
#define PARAM_I 7
#define PARAM_J 8
#define PARAM_K 9
#define PARAM_R 10
#define PARAM_L 11

//PARSER STATUS
#define STATUS_OK 0
#define STATUS_BAD_WORD 1
#define STATUS_SYNTAX_ERROR 2
#define STATUS_UNSUPPORTED 3
#define STATUS_WORKING 4
#define STATUS_LIMITI_SWITCH_TRG 5
#define STATUS_TOOL_CHANGE 6
#define STATUS_OP_ERROR 7

//THE LENGHT OF THE SEGMENTS THAT FORM AN ARCH
#define ARCH_DEFINITION 0.3     //!< The segments size that composes an arch (mm)
#define TOOL_CHANGE_HEIGHT 30.0  //!< The Z offset to allow the tool change (mm)

#endif
