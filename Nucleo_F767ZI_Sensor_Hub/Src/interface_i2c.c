/*
 * interface_i2c.c
 *
 *  Created on: May 25, 2019
 *      Author: Lars
 */

/* Includes ------------------------------------------------------------------*/
#include "interface_i2c.h"


/* Private variables ---------------------------------------------------------*/

/*Private HAL Handler*/
I2C_HandleTypeDef *hi2c1p;
I2C_HandleTypeDef *hi2c2p;
DMA_HandleTypeDef hdma_i2c2_rx;
DMA_HandleTypeDef hdma_i2c2_tx;
DMA_HandleTypeDef hdma_i2c3_tx;
DMA_HandleTypeDef hdma_i2c3_rx;

/* Private function prototypes -----------------------------------------------*/

void I2C1_EV_IRQHandler(void);
void I2C1_ER_IRQHandler(void);

void I2C2_EV_IRQHandler(void);
void I2C2_ER_IRQHandler(void);

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_AbortCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c);

static void I2C_initBuffer(Sensor_I2CDataType *i2c_Data);

/* Private defines -----------------------------------------------------------*/

/*I2C defines */
#define I2C_STANDARD_TIMEOUT 	100

#define I2C_WRITE_BUF_SIZE		32
#define I2C_READ_BUF_SIZE		32

/* Private Function Implementation -----------------------------------------------*/

/**
  * @brief	Sets the transmit and receive buffer to 0 before the first communication
  * @param 	i2c_data pointer the to user I2C-data structure
  * @retval NONE
  *
  */
static void I2C_initBuffer(Sensor_I2CDataType *i2c_data){
	//TODO: Asserts
	//Init WriteBuf
	for(uint8_t i = 0; i < I2C_WRITE_BUF_SIZE; i++)
	{
		i2c_data->transmitBuf[i] = 0;
	}

	//Init ReadBuf
	for(uint8_t i = 0; i < I2C_READ_BUF_SIZE; i++)
	{
		i2c_data->receiveBuf[i] = 0;
	}
	i2c_data->nBytes = 0;
}

/* Public Function Implementation -----------------------------------------------*/

/**
  * @brief	This functions sets the I2C interface for a given sensor, configures the peripheral and call the HAL init functions.
  * 		The varying I2C parameters will already be set from the user in the main application file.
  *	@param	hi2c pointer to the I2C HAL peripheral handler
  * @param 	i2c_data pointer the to user I2C-data structure
  * @retval NONE
  *
  */
void I2C_initInterface(I2C_HandleTypeDef *hi2c, Sensor_I2CDataType *i2c_data){

	//TODO: Asserts
	i2c_data->state = I2C_STATE_NOTINIT;

	hi2c->Init.OwnAddress1 = 192;
	hi2c->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c->Init.OwnAddress2 = 0;
	hi2c->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	if (HAL_I2C_Init(hi2c) != HAL_OK)
	{
		//TODO:ERROR
	}
	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(hi2c, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		//TODO:ERROR
	}
	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(hi2c, 2) != HAL_OK)
	{
		//TODO:ERROR
	}
	if(hi2c->Instance == I2C1)
	{
		hi2c1p = hi2c;
	}
	if(hi2c->Instance == I2C2)
	{
		hi2c2p = hi2c;
	}
	I2C_initBuffer(i2c_data);

	i2c_data->state = I2C_STATE_READY;
}

/**
  * @brief	Transmit the data over the I2C interface as a blocking function with an timeout of 100 ms
  * @param 	i2c_data pointer the to user I2C-data structure
  *	@param	hi2c pointer to the I2C HAL peripheral handler
  * @retval NONE
  *
  */
void I2C_sendData(I2C_HandleTypeDef *hi2c, Sensor_I2CDataType *i2c_data){
	//TODO: Asserts
	i2c_data->state = I2C_STATE_BUSY;
	HAL_I2C_Master_Transmit(hi2c, i2c_data->slaveAdress, i2c_data->transmitBuf, i2c_data->nBytes, I2C_STANDARD_TIMEOUT);
	i2c_data->state = I2C_STATE_READY;
}

/**
  * @brief	Receives the data over the I2C interface as a blocking function with an timeout of 100 ms
  * @param 	i2c_data pointer the to user I2C-data structure
  *	@param	hi2c pointer to the I2C HAL peripheral handler
  * @retval NONE
  *
  */
void I2C_receiveData(I2C_HandleTypeDef *hi2c, Sensor_I2CDataType *i2c_data){
	//TODO: Asserts
	i2c_data->state = I2C_STATE_BUSY;
	HAL_I2C_Master_Receive(hi2c, i2c_data->slaveAdress, i2c_data->receiveBuf, i2c_data->nBytes,I2C_STANDARD_TIMEOUT);
	i2c_data->state = I2C_STATE_READY;
}

/**
  * @brief This function flushes all data for the next i2c communication
  * @param 	i2c_data pointer the to user I2C-data structure
  *
  */
void I2C_flushData(Sensor_I2CDataType *i2c_data)
{

	i2c_data->nBytes = 0;
	for (int i = 0; i < I2C_WRITE_BUF_SIZE; i++)
	{
		i2c_data->receiveBuf[i] = 0;
	}
	for (int i = 0; i < I2C_WRITE_BUF_SIZE; i++)
	{
		i2c_data->receiveBuf[i] = 0;
	}

}

/**
  * @brief This function returns the current state of the I2C interface
  * @param 	i2c_data pointer the to user I2C-data structure
  * @reval	current i2c state
  *			I2C_STATE_NOTINIT
  *			I2C_STATE_READY
  *			I2C_STATE_BUSY
  *			I2C_STATE_ERROR
  */
Sensor_I2CStateType I2C_getState(Sensor_I2CDataType *i2c_data)
{
	return i2c_data->state;
}


/* Callback functions -----------------------------------------------------------*/

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
	//TODO: Hand off the error code to the corresponding sensor

}



