/*
 * interface_spi.h
 *
 *  Created on: May 25, 2019
 *      Author: Lars
 */

#ifndef INTERFACE_SPI_H_
#define INTERFACE_SPI_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Standard includes ----------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>


typedef enum
{
	SPI_ERROR_NONE            =  	0x00,   /*!< No error                               */
	SPI_ERROR_MODF            =	  	0x20,   /*!< MODF error                             */
	SPI_ERROR_CRC             =	   	0x21,   /*!< CRC error                              */
	SPI_ERROR_OVR             =	  	0x22,   /*!< OVR error                              */
	SPI_ERROR_FRE             =	 	0x23,   /*!< FRE error                              */
	SPI_ERROR_DMA             =		0x24,   /*!< DMA transfer error                     */
	SPI_ERROR_FLAG            =	  	0x25,   /*!< Error on RXNE/TXE/BSY/FTLVL/FRLVL Flag */
	SPI_ERROR_ABORT           =	  	0x26,   /*!< Error during SPI Abort procedure       */
} Sensor_SPIErrorType;

typedef enum
{
	SPI_STATE_NOTINIT		= 0x00,
	SPI_STATE_READY			= 0x01,
	SPI_STATE_BUSY			= 0x02,
	SPI_STATE_ERROR			= 0x03,
} Sensor_SPIStateType;

/* Exported types ------------------------------------------------------------*/
typedef struct
{
	uint16_t 					NSS_Pin;
	GPIO_TypeDef				*NSS_Port;
	uint8_t						nBytes;
	uint8_t						transmitBuf[32];
	uint8_t						receiveBuf[32];
	Sensor_SPIErrorType 		error;
	Sensor_SPIStateType			state;

}Sensor_SPIDataType;

/* Exported functions prototypes ---------------------------------------------*/

void SPI_sendData(SPI_HandleTypeDef *hspi, Sensor_SPIDataType *spi_data);
void SPI_receiveData(SPI_HandleTypeDef *hspi, Sensor_SPIDataType *spi_data);
void SPI_receiveSendData(SPI_HandleTypeDef *hspi, Sensor_SPIDataType *spi_data);
void SPI_initInterface(SPI_HandleTypeDef *hspi, Sensor_SPIDataType *spi_data);
void SPI_flushData(Sensor_SPIDataType *spi_data);
Sensor_SPIStateType SPI_getState(Sensor_SPIDataType *spi_data);

/* Exported constants -----------------------------------------------------------*/

#endif /* INTERFACE_I2C_H_ */
