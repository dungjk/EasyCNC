//  Quest'opera è stata rilasciata con licenza Creative Commons Attribuzione 
//  - Condividi allo stesso modo 4.0 Internazionale. 
//  Per leggere una copia della licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
//
//  Autor: Francesco Giurlanda
//  Project: Easy CNC
//  versione: 0.0.1

// ****************************
// ***    GENERAL SETUP     ***
// ****************************
#define SERIAL_BOUND 9600


// ****************************
// *** ROUTER CONFIGURATION ***
// ****************************

// Nunber of steps to make a round
#define ROUTER_MX_STEPS_PER_ROUND 48*2
#define ROUTER_MY_STEPS_PER_ROUND 96*2
// the unit for the speed is steps/s
#define ROUTER_MX_SPEED 300.0
#define ROUTER_MY_SPEED 300.0
// Motor pin mapping
#define ROUTER_MX_P1 4
#define ROUTER_MX_P2 5
#define ROUTER_MX_P3 6
#define ROUTER_MX_P4 7
#define ROUTER_MY_P1 8
#define ROUTER_MY_P2 9
#define ROUTER_MY_P3 10
#define ROUTER_MY_P4 11
// Limit switch pin mapping
#define ROUTER_LIMIT_X 2
#define ROUTER_LIMIT_Y 1
// *****************************
// *          UTENSILS         *
// *****************************

#define _PLOTTER_SERVO



// *****************************
// *       UTENSIL SETUP       *
// *****************************


#ifdef _PLOTTER_SERVO

#define _PLOTTER_SERVO_PIN       3
#define _PLOTTER_SERVO_UP_POS    80
#define _PLOTTER_SERVO_DOWN_POS  136

#endif
