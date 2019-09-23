/*
 * interface_adc.h
 *
 *  Created on: May 25, 2019
 *      Author: Lars
 */

#ifndef INTERFACE_ADC_H_
#define INTERFACE_ADC_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "verbose.h"

/* Standard includes ----------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

/* Exported types ------------------------------------------------------------*/

typedef enum
{
	ADC_ERROR_NONE 			= 0x00U,	 /*!< no error active */
	ADC_ERROR_INTERNAL		= 0x20U,   	 /*!< ADC IP internal error: if problem of clocking, enable/disable, erroneous state */
	ADC_ERROR_OVR     		= 0x21U,     /*!< Overrun error */
	ADC_ERROR_DMA     		= 0x22U,     /*!< DMA transfer error */
} Sensor_ADCErrorType;

typedef enum
{
	ADC_STATE_NOTINIT		= 0x00,		/*!< ADC-Peripheral not yet initialized */
	ADC_STATE_READY			= 0x01,		/*!< ADC-Peripheral ready to be used */
	ADC_STATE_BUSY			= 0x02,		/*!< Conversion currently ongoing */
	ADC_STATE_ERROR			= 0x03,		/*!< Error occured during conversion */
} Sensor_ADCCStateType;

typedef struct
{
	uint32_t				adcBuf[32];			/*!< Internal buffer for the conversion result*/
	Sensor_ADCErrorType		error; 				/*!< Current state of the ADC */
	Sensor_ADCCStateType 	state;				/*!< Active Error description */

}Sensor_ADCDataType;

/* Exported functions prototypes ---------------------------------------------*/

void ADC_initInterface(ADC_HandleTypeDef *hadc, Sensor_ADCDataType *adc_data);
void ADC_singleConversion(ADC_HandleTypeDef *hadc, Sensor_ADCDataType *adc_data);
Sensor_ADCCStateType ADC_getState(Sensor_ADCDataType *adc_data);

/* Exported constants -----------------------------------------------------------*/





#endif /* INTERFACE_ADC_H_ */
