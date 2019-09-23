/*
 * interface_i2c.h
 *
 *  Created on: May 25, 2019
 *      Author: Lars
 */

#ifndef INTERFACE_I2C_H_
#define INTERFACE_I2C_H_

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
	I2C_ERROR_NOERROR		= 0x00U,	 /*!< no error active */
	I2C_ERROR_BERR		    = 0x20U,     /*!< sensor is not initialized */
	I2C_ERROR_ARLO     		= 0x21U,     /*!< arbitration lost LOW -> dominant*/
	I2C_ERROR_OVR      		= 0x22U,     /*!< the rx or tx byte was missed or not sended*/
 	I2C_ERROR_PECERR		= 0x23U,	 /*!< SMBus Error -> mismatch of PEC register*/
	I2C_ERROR_TIMEOUT		= 0x24U,	 /*!< SMBus Error -> timeout detected */
	I2C_ERROR_ALERT			= 0x25U, 	 /*!< SMBus Error -> interrupt caused by alert pin*/
	I2C_ERORR_DMA			= 0x26U,	 /*!< DMA transfer error */
	I2C_ERROR_AF			= 0x27u, 	 /*!< no acknowledge flag error */

} Sensor_I2CErrorType;

typedef enum
{
	I2C_STATE_NOTINIT		= 0x00,
	I2C_STATE_READY			= 0x01,
	I2C_STATE_BUSY			= 0x02,
	I2C_STATE_ERROR			= 0x03,
} Sensor_I2CStateType;

typedef struct
{
	uint8_t 				slaveAdress;
	uint8_t					nBytes;
	uint8_t					transmitBuf[32];
	uint8_t					receiveBuf[32];
	Sensor_I2CErrorType 	error;
	Sensor_I2CStateType		state;

}Sensor_I2CDataType;

/* Exported functions prototypes ---------------------------------------------*/

void I2C_sendData_IT(I2C_HandleTypeDef *hi2c, Sensor_I2CDataType *i2c_data);
void I2C_receiveData_IT(I2C_HandleTypeDef *hi2c, Sensor_I2CDataType *i2c_data);
void I2C_sendData(I2C_HandleTypeDef *hi2c, Sensor_I2CDataType *i2c_data);
void I2C_receiveData(I2C_HandleTypeDef *hi2c, Sensor_I2CDataType *i2c_data);
void I2C_initInterface(I2C_HandleTypeDef *hi2c, Sensor_I2CDataType *i2c_Data);
void I2C_flushData(Sensor_I2CDataType *i2c_data);
Sensor_I2CStateType I2C_getState(Sensor_I2CDataType *i2c_data);

/* Exported constants -----------------------------------------------------------*/


#endif /* INTERFACE_I2C_H_ */
