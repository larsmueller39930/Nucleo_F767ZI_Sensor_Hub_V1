/*
 * sensor.h
 *
 *  Created on: May 25, 2019
 *      Author: Lars
 */

#ifndef SENSOR_H_
#define SENSOR_H_

/* Includes ------------------------------------------------------------------*/
#include "interface_i2c.h"
#include "interface_spi.h"
#include "interface_adc.h"
#include "utilFunctions.h"
/* Standard includes ----------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>


/* Exported types ------------------------------------------------------------*/
typedef struct
{
	uint8_t 	sensorType;		/*!< Type of temperature sensor, can be a given value of SENSOR_TYPE_x */
	uint8_t 	sensorCnt; 		/*!< Sensor identifier */
	uint8_t 	interfaceType;	/*!< Interface type/protocol, can be a given value of INTERFACE_TYPE_x */
	uint16_t 	identifier;
}Sensor_InitType;

typedef struct
{
	uint16_t regAddress;		/*!< Address of the register of a external sensor */
	uint16_t regSize;			/*!< Size of the Register */
	uint32_t regValue;			/*!< Value of the Register */
}Sensor_RegType;

typedef enum
{
	SENSOR_STATE_RESET      = 0x00U,    /*!< sensor is not initialized */
	SENSOR_STATE_READY      = 0x01U,    /*!< sensor is initialized and ready to use */
	SENSOR_STATE_BUSY       = 0x02U,    /*!< sensor is transmitting/receivign or processing data */
 	SENSOR_STATE_ERROR      = 0x06U,    /*!< sensor is in error state */
} Sensor_StateTypeDef;

typedef struct
{

	Sensor_InitType Init;					/*!< Parameters to init the sensor handler */
	double tempDataBuf[128];				/*!< Temperature data after conversion */
	RTC_TimeTypeDef timeStampBuf[128];		/*!< Data buffer for RTC timestamps for each cycle */
	RTC_DateTypeDef dateStampBuf[128];		/*!< Data buffer for RTC datestamps for each cycle */
	uint32_t cycleCnt;						/*!< Cycle-counter for each Sensor */

	uint8_t status;							/*!< Current status of the sensor */
	Sensor_RegType regMap[20];				/*!< Structure for all possible registers of the sensor */
	uint16_t hardwareID;					/*!< ID of the external sensor hardware */

	I2C_HandleTypeDef hi2c_peripheral;		/*!< Handler for the I2C interface when this interface is chosen */
	Sensor_I2CDataType i2c_data;			/*!< Structure to transfer data, state and error of the ADC peripheral to the sensor handler, also holds
												specific slave information for the I2C interface  */

	SPI_HandleTypeDef spi_peripheral;		/*!< Handler for the SPI interface when this interface is chosen */
	Sensor_SPIDataType spi_data;			/*!< Structure to transfer data, state and error of the SPI peripheral to the sensor handler, also holds
												 specific slave information for the SPI interface  */

	ADC_HandleTypeDef adc_peripheral;		/*!< Handler for the ADC interface when this interface is chosen */
	Sensor_ADCDataType adc_data;			/*!< Structure to transfer data, state and error of the ADC peripheral to the sensor handler  */


	uint8_t errorcode[10]; /* Buffer of occurred error events */
	char errormessage[128]; /* buffer for error message string of current active error */

}Sensor_HandleType;


/* Exported functions prototypes ---------------------------------------------*/

Sensor_StateTypeDef Sensor_initSensor(Sensor_HandleType *sens);

Sensor_StateTypeDef Sensor_readTempData(Sensor_HandleType *sens);

Sensor_StateTypeDef Sensor_writeControlData(Sensor_HandleType *sens);
Sensor_StateTypeDef Sensor_readControlData(Sensor_HandleType *sens);

Sensor_StateTypeDef Sensor_readSensorState(Sensor_HandleType *sens);
Sensor_StateTypeDef Sensor_readSensorID(Sensor_HandleType *sens);
Sensor_StateTypeDef Sensor_setSensorCalibration(Sensor_HandleType *sens);
Sensor_StateTypeDef Sensor_tempConversion(Sensor_HandleType *sens);
Sensor_StateTypeDef Sensor_setTimeStamp(Sensor_HandleType *sens);
Sensor_StateTypeDef Sensor_putDummyData(Sensor_HandleType *sens);
void Sensor_incrementCycleCnt(Sensor_HandleType *sens);

double Sensor_getTempData(Sensor_HandleType *sens);
uint32_t Sensor_getRegData(Sensor_HandleType *sens, uint16_t regAddress);
RTC_TimeTypeDef Sensor_getTimeData(Sensor_HandleType *sens);

/* Exported constants -----------------------------------------------------------*/

/* Possible values for the sensor interfaceType */
#define SENSOR_TYPE_THERMOCOUPLE		0x01
#define SENSOR_TYPE_THERMOPILE			0x02
#define SENSOR_TYPE_THERMISTOR_PTC		0x03
#define SENSOR_TYPE_THERMISTOR_NTC		0x04
#define SENSOR_TYPE_TSEP				0x05
#define SENSOR_TYPE_THERMOCAM			0x06

/* Possible values for the sensor interfaceType */
#define SENSOR_INTERFACE_TYPE_I2C		0x01
#define SENSOR_INTERFACE_TYPE_SPI		0x02
#define SENSOR_INTERFACE_TYPE_ANALOG	0x03
#define SENSOR_INTERFACE_TYPE_TRIGGER	0x04

#define SENSOR_REG_SIZE_8 				1U
#define SENSOR_REG_SIZE_16 				2U
#define SENSOR_REG_SIZE_32 				4U

/*Active Sensors
 *I2C Sensor: A0, A1...
 *SPI Sensor: B0, B1...
 *ADC Sensor: C0, C1...
 *Camera: D0...
 */
#define MCP9600_ACTIVE					0xA0
#define GTPMO023_ACTIVE					0xB0
#define PSOC4_ACTIVE					0xB1
#define VFADC_ACTIVE					0xC0
#define VTHADC_ACTIVE					0xC1
#define VARIOCAM_ACIVE					0xD0

/*Bytes indices in the receive and transmit buffer*/
#define Byte1 							0U
#define Byte2 							1U
#define Byte3 							2U
#define Byte4 							3U
#define Byte5 							4U
#define Byte6 							5U
#define Byte7 							6U
#define Byte8 							7U

#endif /* SENSOR_H_ */
