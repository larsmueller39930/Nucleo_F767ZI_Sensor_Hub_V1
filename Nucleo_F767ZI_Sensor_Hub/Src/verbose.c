/*
 * verbose.c
 *
 *  Created on: 17.06.2019
 *      Author: Lars
 */


/* Includes ------------------------------------------------------------------*/

#include "verbose.h"

/* Private variables ---------------------------------------------------------*/

/*Private HAL Handler*/
extern I2C_HandleTypeDef *hi2c1p;
extern I2C_HandleTypeDef *hi2c2p;

void I2C1_EV_IRQVerbose(void)
{
	if(__HAL_I2C_GET_FLAG(hi2c1p,I2C_FLAG_ADDR) == SET)
	{
		printf("I2C_EVENT_IRQ FLAG: ADDRESS SET SET\r\n");
	}
	if(__HAL_I2C_GET_FLAG(hi2c1p,I2C_FLAG_TXE) == SET)
	{
		printf("I2C_EVENT_IRQ FLAG: TRANSMIT BUFFER EMPTY SET\r\n");
	}
	if(__HAL_I2C_GET_FLAG(hi2c1p,I2C_FLAG_RXNE) == SET)
	{
		printf("I2C_EVENT_IRQ FLAG: RECEIVE BUFFER FULL SET\r\n");
	}
	if(__HAL_I2C_GET_FLAG(hi2c1p,I2C_FLAG_STOPF) == SET)
	{
		printf("I2C_EVENT_IRQ FLAG: TRANSMIT BUFFER SET SET\r\n");
	}
}
 void I2C1_ER_IRQVerbose(void)

{
	if(__HAL_I2C_GET_FLAG(hi2c1p,I2C_FLAG_AF) == SET)
	{
		printf("I2C_ERROR_IRQ FLAG: NOCK FULL SET\r\n");
	}
	if(__HAL_I2C_GET_FLAG(hi2c1p,I2C_FLAG_BERR) == SET)
	{
		printf("I2C_ERROR_IRQ FLAG: FULL SET\r\n");
	}
	if(__HAL_I2C_GET_FLAG(hi2c1p,I2C_FLAG_AF) == SET)
	{
		printf("I2C_ERROR_IRQ FLAG: FULL SET\r\n");
	}
	if(__HAL_I2C_GET_FLAG(hi2c1p,I2C_FLAG_AF) == SET)
	{
		printf("I2C_ERROR_IRQ FLAG: FULL SET\r\n");
	}
}

void I2C2_EV_IRQVerbose(void)
{

	if(__HAL_I2C_GET_FLAG(hi2c1p,I2C_FLAG_ADDR) == SET)
	{
		printf("I2C_EVENT_IRQ FLAG: ADDRESS SET SET\r\n");
	}
	if(__HAL_I2C_GET_FLAG(hi2c1p,I2C_FLAG_TXE) == SET)
	{
		printf("I2C_EVENT_IRQ FLAG: TRANSMIT BUFFER EMPTY SET\r\n");
	}
	if(__HAL_I2C_GET_FLAG(hi2c1p,I2C_FLAG_RXNE) == SET)
	{
		printf("I2C_EVENT_IRQ FLAG: RECEIVE BUFFER FULL SET\r\n");
	}
	if(__HAL_I2C_GET_FLAG(hi2c1p,I2C_FLAG_STOPF) == SET)
	{
		printf("I2C_EVENT_IRQ FLAG: TRANSMIT BUFFER SET SET\r\n");
	}

}

void I2C2_ER_IRQVerbose(void)
{
	if(__HAL_I2C_GET_FLAG(hi2c1p,I2C_FLAG_AF) == SET)
	{
		printf("I2C_ERROR_IRQ FLAG: NOCK FULL SET\r\n");
	}
	if(__HAL_I2C_GET_FLAG(hi2c1p,I2C_FLAG_BERR) == SET)
	{
		printf("I2C_ERROR_IRQ FLAG: FULL SET\r\n");
	}
	if(__HAL_I2C_GET_FLAG(hi2c1p,I2C_FLAG_AF) == SET)
	{
		printf("I2C_ERROR_IRQ FLAG: FULL SET\r\n");
	}
	if(__HAL_I2C_GET_FLAG(hi2c1p,I2C_FLAG_AF) == SET)
	{
		printf("I2C_ERROR_IRQ FLAG: FULL SET\r\n");
	}
}


void SPI2_IRQVerbose(void)
{


}

void SPI3_IRQVerbose(void)
{


}
