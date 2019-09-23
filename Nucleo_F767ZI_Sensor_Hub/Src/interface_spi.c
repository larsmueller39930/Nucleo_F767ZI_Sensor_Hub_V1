/*
 * interface_spi.c
 *
 *  Created on: 30.05.2019
 *      Author: Lars
 */

/* Includes ------------------------------------------------------------------*/
#include "interface_spi.h"


/* Private variables ---------------------------------------------------------*/

/*Private HAL Handler*/
SPI_HandleTypeDef *hspi1p;
SPI_HandleTypeDef *hspi2p;

DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi1_tx;

DMA_HandleTypeDef hdma_sp21_rx;
DMA_HandleTypeDef hdma_sp21_tx;

/* Private function prototypes -----------------------------------------------*/

static void SPI_initBuffer(Sensor_SPIDataType *spi_Data);

/* Private defines -----------------------------------------------------------*/

/*SPI defines */
#define SPI_WRITE_BUF_SIZE		32
#define SPI_READ_BUF_SIZE		32

#define SPI_STANDARD_TIMEOUT 	100

/* Private Function Implementation -----------------------------------------------*/

/**
  * @brief	Sets the transmit and receive buffer to 0 before the first communication
  * @param 	spi_data pointer the to user SPI-data structure
  * @retval NONE
  *
  */
static void SPI_initBuffer(Sensor_SPIDataType *spi_data)
{
	//TODO: Asserts

	//Init WriteBuf
	for(uint8_t i = 0; i < SPI_WRITE_BUF_SIZE; i++)
	{
		spi_data->transmitBuf[i] = 0;
	}

	//Init ReadBuf
	for(uint8_t i = 0; i < SPI_READ_BUF_SIZE; i++)
	{
		spi_data->receiveBuf[i] = 0;
	}
	spi_data->nBytes = 0;
}

/* Public Function Implementation -----------------------------------------------*/

/**
  * @brief	This functions sets the SPI interface for a given sensor, configures the peripheral and call the HAL init functions.
  * 		The varying SPI parameters will already be set from the user in the main application file.
  *	@param	hspi pointer to the SPI HAL peripheral handler
  * @param 	spi_data pointer the to user SPI-data structure
  * @retval NONE
  *
  */
void SPI_initInterface(SPI_HandleTypeDef *hspi, Sensor_SPIDataType *spi_data)
{

	//TODO: Asserts
	hspi->Init.NSS = SPI_NSS_SOFT;
	hspi->Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi->Init.TIMode = SPI_TIMODE_DISABLE;
	hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi->Init.CRCPolynomial = 7;
	hspi->Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	hspi->Init.NSSPMode = SPI_NSS_PULSE_DISABLE;

	if (HAL_SPI_Init(hspi) != HAL_OK)
	{
		//TODO:ERROR
	}

	//Pointer for interrupt handling
	if(hspi->Instance == SPI1)
	{
		hspi1p = hspi;
	}
	if(hspi->Instance == SPI2)
	{
		hspi2p = hspi;
	}
	/*Set the NSS Pin to high initially*/
	HAL_GPIO_WritePin(spi_data->NSS_Port, spi_data->NSS_Pin, GPIO_PIN_SET);

	SPI_initBuffer(spi_data);

}

/**
  * @brief	HALFDUPLEX Mode: Transmit the data over the SPI interface as a blocking function with an timeout of 100 ms
  * @param 	spi_data pointer the to user SPI-data structure
  *	@param	hspi pointer to the SPI HAL peripheral handler
  * @retval NONE
  *
  */
void SPI_sendData(SPI_HandleTypeDef *hspi, Sensor_SPIDataType *spi_data)
{
	//TODO: Asserts
	HAL_GPIO_WritePin(spi_data->NSS_Port, spi_data->NSS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi, spi_data->transmitBuf, spi_data->nBytes, SPI_STANDARD_TIMEOUT);
	HAL_GPIO_WritePin(spi_data->NSS_Port, spi_data->NSS_Pin, GPIO_PIN_SET);
}

/**
  * @brief	HALFDUPLEX Mode: Receives the data over the SPI interface as a blocking function with an timeout of 100 ms
  * @param 	spi_data pointer the to user SPI-data structure
  *	@param	hspi pointer to the SPI HAL peripheral handler
  * @retval NONE
  *
  */
void SPI_receiveData(SPI_HandleTypeDef *hspi, Sensor_SPIDataType *spi_data)
{
	//TODO: Asserts
	HAL_GPIO_WritePin(spi_data->NSS_Port, spi_data->NSS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Receive(hspi, spi_data->receiveBuf, spi_data->nBytes, SPI_STANDARD_TIMEOUT);
	HAL_GPIO_WritePin(spi_data->NSS_Port, spi_data->NSS_Pin, GPIO_PIN_SET);
}


/**
  * @brief	FULLDUPLEX Mode: Receiving and Transmitting the data over the SPI interface as a blocking function
  * 		with an timeout of 100 ms
  * @param 	spi_data pointer the to user SPI-data structure
  *	@param	hspi pointer to the SPI HAL peripheral handler
  * @retval NONE
  *
  */
void SPI_receiveSendData(SPI_HandleTypeDef *hspi, Sensor_SPIDataType *spi_data)
{
	//TODO: Asserts
	HAL_GPIO_WritePin(spi_data->NSS_Port, spi_data->NSS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi, spi_data->transmitBuf, spi_data->receiveBuf, spi_data->nBytes, SPI_STANDARD_TIMEOUT);
	HAL_GPIO_WritePin(spi_data->NSS_Port, spi_data->NSS_Pin, GPIO_PIN_SET);
}

/**
  * @brief	This function flushes all data for the next spi communication
  * @param 	spi_data pointer the to user spi-data structure
  * @retval NONE
  *
  */
void SPI_flushData(Sensor_SPIDataType *spi_data)
{

	spi_data->nBytes = 0;
	for (int i = 0; i < SPI_READ_BUF_SIZE; i++)
	{
		spi_data->receiveBuf[i] = 0;
	}
	for (int i = 0; i < SPI_WRITE_BUF_SIZE; i++)
	{
		spi_data->receiveBuf[i] = 0;
	}

}

/**
  * @brief This function returns the current state of the SPI interface
  * @param 	spi_data pointer the to user SPI-data structure
  * @reval	current spi state
  *			SPI_STATE_NOTINIT
  *			SPI_STATE_READY
  *			SPI_STATE_BUSY
  *			SPI_STATE_ERROR
  */
Sensor_SPIStateType SPI_getState(Sensor_SPIDataType *spi_data)
{
	return spi_data->state;
}






