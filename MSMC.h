/*!
    \file      MSMC.h
    \brief     Multi Stepper Motor Control Interface.
    \details   The file describes the interface that must be implemented by each motor controller class
    \author    Francesco Giurlanda
    \version   0.0.1
    \date      2014

    \copyright Quest'opera è stata rilasciata con licenza Creative Commons Attribuzione
               - Condividi allo stesso modo 4.0 Internazionale. Per leggere una copia della
               licenza visita il sito web http://creativecommons.org/licenses/by-sa/4.0/.
 */

#ifndef MSMC_H_
#define MSMC_H_

/*! \class MSMC
 *  \brief The multi stepper motor control interface
 */
class MSMC {
public:
	virtual void forward(uint32_t, uint32_t);
	virtual void backward(uint32_t, uint32_t);
	virtual void speed(uint32_t);
	virtual void stop();
	virtual void pause();
	virtual void restart();
	virtual void setMode(uint8_t);
	virtual uint8_t getMode();
	virtual int8_t getDir();
	virtual uint32_t getSteps();
	//virtual void setPins(uint8_t, ...);
	virtual boolean update();
};

#endif /* MSMC_H_ */
