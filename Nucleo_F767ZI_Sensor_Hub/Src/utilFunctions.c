/*
 * utilFunctions.c
 *
 *  Created on: Aug 16, 2019
 *      Author: Lars
 */


/* Includes ------------------------------------------------------------------*/
#include "utilFunctions.h"

extern RTC_HandleTypeDef hrtc;

/* Public Function Implementation -----------------------------------------------*/

/**
  * @brief  Turn on the current sink provided by the PSOC to measure the TSEP temperature
  * @param NONE
  * @retval NONE
  */
void turnOnCurrentSink(void)
{
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_0, GPIO_PIN_RESET);
}

/**
  * @brief  Turn off the current sink provided by the PSOC to measure the TSEP temperature
  * @param NONE
  * @retval NONE
  */
void turnOffCurrentSink(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET);
}

/**
  * @brief  Sets the system real time for the sensor timing to the given value
  * @param system_time structure of the RTC peripheral that holds the desired time value
  * @param system_date structure of the RTC peripheral that holds the desired date value
  * @retval NONE
  */
void setSystemTime(RTC_TimeTypeDef system_time, RTC_DateTypeDef system_date)
{
	HAL_RTC_SetTime(&hrtc, &system_time, RTC_FORMAT_BCD);
	HAL_RTC_SetDate(&hrtc, &system_date, RTC_FORMAT_BCD);
}

/**
  * @brief Inits the clock counter of the DWT core module
  * @param NONE
  * @retval NONE
  */
void initDWT(void)
{
	DWT->LAR = 0xC5ACCE55;
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0;
	DWT->CTRL |= 1;
}

/**
  * @brief Delays the programm execution for the given microseconds
  * @param us delay in microseconds
  * @retval NONE
  */
void delayUSDWT(uint32_t us)
{
	uint32_t cycles = 0;
	uint32_t start  = 0;

	cycles = (SystemCoreClock / 1000000L) * us;
	start  = DWT->CYCCNT;
	do {
	} while (DWT->CYCCNT - start < cycles);
}




