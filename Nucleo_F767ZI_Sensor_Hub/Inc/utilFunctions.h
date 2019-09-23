
/*
 * utilFunctions.h
 *
 *  Created on: August 16, 2019
 *      Author: Lars
 */


#ifndef UTIL_FUNCTIONS_H_
#define UTIL_FUNCTIONS_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Standard includes ----------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

/* Exported functions prototypes ---------------------------------------------*/
void turnOnCurrentSink(void);
void turnOffCurrentSink(void);
void setSystemTime(RTC_TimeTypeDef system_time, RTC_DateTypeDef system_date);
void delayUSDWT(uint32_t us);
void initDWT(void);

#endif /* UTIL_FUNCTIONS_H_ */
