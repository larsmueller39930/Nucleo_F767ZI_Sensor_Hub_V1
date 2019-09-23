/*
 * verbose.h
 *
 *  Created on: 17.06.2019
 *      Author: Lars
 */

#ifndef VERBOSE_H_
#define VERBOSE_H_

/* Includes ------------------------------------------------------------------*/

#include "stm32f7xx_hal.h"

/* Exported functions prototypes ---------------------------------------------*/

/*Prints out the interrupt flags*/
void I2C1_EV_IRQVerbose(void);
void I2C1_ER_IRQVerbose(void);
void I2C1_EV_IRQVerbose(void);
void I2C2_ER_IRQVerbose(void);

void SPI2_IRQVerbose(void);
void SPI3_IRQVerbose(void);


#endif /* VERBOSE_H_ */
