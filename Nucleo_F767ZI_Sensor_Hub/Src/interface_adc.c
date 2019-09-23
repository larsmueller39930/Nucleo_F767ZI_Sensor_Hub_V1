/*
 * interface_adc.c
 *
 *  Created on: Aug 15, 2019
 *      Author: Lars
 */

/* Includes ------------------------------------------------------------------*/
#include "interface_adc.h"


/* Private variables ---------------------------------------------------------*/

/*Private HAL Handler*/
ADC_HandleTypeDef *hadc1p;
ADC_HandleTypeDef *hadc2p;

/* Private function prototypes -----------------------------------------------*/
void ADC_IRQHandler(void);

static void ADC_initBuffer(Sensor_ADCDataType *adc_Data);

/* Private defines -----------------------------------------------------------*/

/*ADC defines */
#define ADC_DATA_BUF_SIZE 	32
#define ADC_ZERO_ERROR 		3 //Calculating over 500 samples of 0 Voltage

/* Private Function Implementation -----------------------------------------------*/

/**
  * @brief	Sets the transmit and receive buffer to 0 before the first communication
  * @param 	adc_data pointer the to user ADC-data structure
  * @retval NONE
  *
  */
static void ADC_initBuffer(Sensor_ADCDataType *adc_data)
{
	//TODO: Asserts

	//Init DataBuf
	for(uint8_t i = 0; i < ADC_DATA_BUF_SIZE ; i++)
	{
		adc_data->adcBuf[i] = 0;
	}
}

/* Public Function Implementation -----------------------------------------------*/

/**
  * @brief	This functions sets the ADC interface for a given sensor, configures the peripheral and call the HAL init functions.
  * 		The varying ADC parameters will already be set from the user in the main application file.
  *	@param	hadc pointer to the ADC HAL peripheral handler
  * @param 	adc_data pointer the to user ADC-data structure
  * @retval NONE
  *
  */
void ADC_initInterface(ADC_HandleTypeDef *hadc, Sensor_ADCDataType *adc_data){

	//TODO: Asserts

	adc_data->state = ADC_STATE_NOTINIT;

	ADC_ChannelConfTypeDef sConfig = {0};

	hadc->Init.ScanConvMode = DISABLE;
	hadc->Init.ContinuousConvMode = DISABLE;
	hadc->Init.DiscontinuousConvMode = DISABLE;
	hadc->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc->Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc->Init.DMAContinuousRequests = DISABLE;
	hadc->Init.EOCSelection = ADC_EOC_SINGLE_CONV;

	if (HAL_ADC_Init(hadc) != HAL_OK)
	{
		//TODO: ERROR
	}

	/* Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.*/
	if(hadc->Instance == ADC1)
	{
		hadc1p = hadc;	//Pointer for interrupt handling
		sConfig.Channel = ADC_CHANNEL_0;
		sConfig.Rank = ADC_REGULAR_RANK_1;
		sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
		if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
		{
			//TODO: ERROR
		}
	}
	if(hadc->Instance == ADC2)
	{
		hadc2p = hadc;	//Pointer for interrupt handling
		sConfig.Channel = ADC_CHANNEL_3;
		sConfig.Rank = ADC_REGULAR_RANK_1;
		sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
		if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
		{
			//TODO: ERROR
		}
	}

	ADC_initBuffer(adc_data);

	adc_data->state = ADC_STATE_READY;
}

/**
  * @brief	Single conversion of the voltage for the current chosen channel of the given adc
  * @param 	adc_data pointer the to user ADC-data structure
  *	@param	hadc pointer to the ADC HAL peripheral handler
  * @retval NONE
  *
  */
void ADC_singleConversion(ADC_HandleTypeDef *hadc, Sensor_ADCDataType *adc_data)
{
	//TODO: Asserts

	uint32_t adcsum = 0;
	uint8_t loop = 16;
	int32_t temp;

	adc_data->state = ADC_STATE_BUSY;

	/*Averaging over 16 samples*/
	for(uint8_t i = 0; i < loop; i++)
	{
		HAL_ADC_Start(hadc);
		HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY);
		adcsum += HAL_ADC_GetValue(hadc);
	}

	temp = (adcsum / loop) - ADC_ZERO_ERROR;

	if(temp > 0) /* Positive conversion result */
	{
		adc_data->adcBuf[0] = temp;
	}
	if(temp < 0) /* Negative conversion result due to ADC_ZERO_ERROR*/
	{
		adc_data->adcBuf[0] = 0;
	}
	adc_data->state = ADC_STATE_READY;

}

/**
  * @brief 	This function returns the current state of the ADC interface
  * @param	adc_data pointer the to user SPI-data structure
  * @reval	current adc state
  *			ADC_STATE_NOTINIT
  *			ADC_STATE_READY
  *			ADC_STATE_BUSY
  *			ADC_STATE_ERROR
  */
Sensor_ADCCStateType ADC_getState(Sensor_ADCDataType *adc_data)
{
	return adc_data->state;
}

