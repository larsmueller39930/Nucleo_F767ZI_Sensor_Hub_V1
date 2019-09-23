/*
 * operator.h
 *
 *  Created on: Jun 14, 2019
 *      Author: Lars
 */

#ifndef UART_OPERATOR_H_
#define UART_OPERATOR_H_


/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "sensor.h"

/* Standard includes ----------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

/* Exported types ------------------------------------------------------------*/
typedef struct
{
	uint8_t		nBytes;
	uint8_t		transmitBuf[32];
	uint8_t		receiveBuf[32];
	uint8_t 	error;
	uint32_t 	dataSize;

}Operator_UartDataType;


/* Exported functions prototypes ---------------------------------------------*/
void Operator_uartInitOperatorInterface(UART_HandleTypeDef *huart, Operator_UartDataType *uart_data);
void Operator_uartReadSensorConfiguration(UART_HandleTypeDef *huart, Operator_UartDataType *uart_data);
void Operator_uartListenCommand(UART_HandleTypeDef *huart, Operator_UartDataType *uart_data);
void Operator_uartSendTempData(UART_HandleTypeDef *huart, Operator_UartDataType *uart_data);
uint8_t Operator_processCommand(UART_HandleTypeDef *huart, Operator_UartDataType *uart_data);
uint32_t Operator_getDataSize(Operator_UartDataType *uart_data);


void HAL_UART_RXCptlCallback(UART_HandleTypeDef *huart);
/* Exported constants -----------------------------------------------------------*/
#define OPERATOR_NO_COMMAND		0U
#define OPERATOR_START_COMMAND 	1U
#define OPERATOR_DATASIZE_COMMAND	2U

#endif /* UART_OPERATOR_H_ */
