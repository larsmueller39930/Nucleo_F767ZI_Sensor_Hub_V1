/*
 * sensor.c
 *
 *  Created on: May 25, 2019
 *      Author: Lars
 */

/* Includes ------------------------------------------------------------------*/

#include <sensor.h>

/* Private defines -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void Sensor_initRegList(Sensor_HandleType *sens);

/* Private variables ---------------------------------------------------------*/
extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim6;
uint8_t cameraFrameReady = 0;

/* Private defines -----------------------------------------------------------*/


/*Calibration data for digital sensors*/
#define THERMOCOUPLE_TYPE			(0U << 4)
#define FILTER_COEFFCIENTS			(4U << 0)
#define MCP9600_OPTIONS_BYTE1 		(THERMOCOUPLE_TYPE | FILTER_COEFFCIENTS )

#define COLD_JUNCTION_RESOLUTION			(1U << 7)
#define ADC_MEASUREMENT_RESOLUTION			(2U << 5)
#define BURST_MODE_TEMPERATURE_SAMPLES		(0U << 2)
#define SHUTDOWN_MODES						(0U << 0)
#define MCP9600_OPTIONS_BYTE2 		(COLD_JUNCTION_RESOLUTION | ADC_MEASUREMENT_RESOLUTION | BURST_MODE_TEMPERATURE_SAMPLES | SHUTDOWN_MODES)

/*Bit Values in the Status Registers for updated Data*/
#define STATUS_TH_UPDATE_FLAG				(1U << 6)
#define STATUS_TVF_UPDATE_FLAG				(1U << 0)

/* Private Function Implementation -----------------------------------------------*/

/**
  * @brief	Initialize all addresses and sizes of the sensor registers
  *	@param	sens pointer to the sensor handler
  * @retval NONE
  */
static void Sensor_initRegList(Sensor_HandleType *sens)
{

	switch(sens->Init.identifier)
	{
	case MCP9600_ACTIVE:
		//TODO: Read register-mapping over file

		//Temperature T_H
		sens->regMap[0].regAddress = 0x00;
		sens->regMap[0].regSize = SENSOR_REG_SIZE_16;
		//Temperature T_Delta
		sens->regMap[1].regAddress = 0x01;
		sens->regMap[1].regSize = SENSOR_REG_SIZE_16;
		//Temperature T_C
		sens->regMap[2].regAddress = 0x02;
		sens->regMap[2].regSize = SENSOR_REG_SIZE_16;
		//Status
		sens->regMap[3].regAddress = 0x04;
		sens->regMap[3].regSize = SENSOR_REG_SIZE_8;
		//Config thermocouple
		sens->regMap[4].regAddress = 0x05;
		sens->regMap[4].regSize = SENSOR_REG_SIZE_8;
		//Config device
		sens->regMap[5].regAddress = 0x06;
		sens->regMap[5].regSize = SENSOR_REG_SIZE_8;
		//Device Id
		sens->regMap[6].regAddress = 0x20;
		sens->regMap[6].regSize = SENSOR_REG_SIZE_16;
		break;
	case GTPMO023_ACTIVE:

		//Sensor Temp
		sens->regMap[0].regAddress = 0xA0;
		sens->regMap[0].regSize = SENSOR_REG_SIZE_16;
		//Objekt Temp
		sens->regMap[1].regAddress = 0xA1;
		sens->regMap[1].regSize = SENSOR_REG_SIZE_16;
		break;

	case PSOC4_ACTIVE:
		//Status Register
		sens->regMap[0].regAddress = 0xA0;
		sens->regMap[0].regSize = SENSOR_REG_SIZE_8;
		//Temp Register
		sens->regMap[1].regAddress = 0xA1;
		sens->regMap[1].regSize = SENSOR_REG_SIZE_32;
		break;
	case VFADC_ACTIVE:
		//Status Register
		sens->regMap[0].regAddress = 0xA0;
		sens->regMap[0].regSize = SENSOR_REG_SIZE_8;
		//Voltage Register
		sens->regMap[1].regAddress = 0xA1;
		sens->regMap[1].regSize = SENSOR_REG_SIZE_32;
	case VTHADC_ACTIVE:
		//Status Register
		sens->regMap[0].regAddress = 0xA0;
		sens->regMap[0].regSize = SENSOR_REG_SIZE_8;
		//Voltage Register
		sens->regMap[1].regAddress = 0xA1;
		sens->regMap[1].regSize = SENSOR_REG_SIZE_32;
	case VARIOCAM_ACIVE:
		//Status Register
		sens->regMap[0].regAddress = 0xA0;
		sens->regMap[0].regSize = SENSOR_REG_SIZE_8;
		//VTrigger Register
		sens->regMap[1].regAddress = 0xA1;
		sens->regMap[1].regSize = SENSOR_REG_SIZE_8;

	default:
		//No Sensor chosen
		break;
	}
}


/* Public Function Implementation -----------------------------------------------*/
/**
  * @brief	Initialize the sensor interface handlers, the buffer and the registers. The initialization process depends on the chosen interface peripheral
  *	@param	sens pointer to the sensor handler
  * @retval returns the state of the given sensor handler after processing the function
  * 		SENSOR_STATE_READY
  *			SENSOR_STATE_BUSY
  *			SENSOR_STATE_ERROR
  */
Sensor_StateTypeDef Sensor_initSensor(Sensor_HandleType *sens){

	Sensor_StateTypeDef sensError = SENSOR_STATE_READY;

	if (sens == NULL)
	{
		sensError = SENSOR_STATE_ERROR;
		return sensError;
	}

	switch(sens->Init.interfaceType)
	{
	case SENSOR_INTERFACE_TYPE_I2C:
		if(sens->hi2c_peripheral.Instance == 0x00)
		{
			//No interface handler active
			sensError = SENSOR_STATE_ERROR;
			return sensError;
		}
		I2C_initInterface(&sens->hi2c_peripheral,&sens->i2c_data);
		Sensor_initRegList(sens);
		Sensor_readSensorID(sens);
		Sensor_readSensorState(sens);
		sens->cycleCnt = 0;
		break;
	case SENSOR_INTERFACE_TYPE_SPI:
		if(sens->spi_peripheral.Instance == 0x00)
		{
			sensError = SENSOR_STATE_ERROR;
			return sensError;
		}
		SPI_initInterface(&sens->spi_peripheral,&sens->spi_data);
		Sensor_initRegList(sens);
		sens->cycleCnt = 0;
		break;
	case SENSOR_INTERFACE_TYPE_ANALOG:
		if(sens->adc_peripheral.Instance == 0x00)
		{
			sensError = SENSOR_STATE_ERROR;
			return sensError;
		}
		ADC_initInterface(&sens->adc_peripheral,&sens->adc_data);
		Sensor_initRegList(sens);
		sens->cycleCnt = 0;
		break;
	case SENSOR_INTERFACE_TYPE_TRIGGER:
		//Only GPIO necessary
		Sensor_initRegList(sens);
		sens->cycleCnt = 0;
		break;
	default:
		//No interface was chosen
		sensError = SENSOR_STATE_ERROR;
		return sensError;
		break;
	}

	return sensError;
}

/**
  * @brief	Read out the temperature data registers, the data location is evaluated depending on the given the sensor identifier and the chosen interface peripheral
  *	@param	sens pointer to the sensor handler
  * @retval returns the state of the given sensor handler after processing the function
  * 		SENSOR_STATE_READY
  *			SENSOR_STATE_BUSY
  *			SENSOR_STATE_ERROR
  */
Sensor_StateTypeDef Sensor_readTempData(Sensor_HandleType *sens)
{
	Sensor_StateTypeDef sensError = SENSOR_STATE_READY;
	//TODO: Asserts
	if (sens == NULL)
	{
		sensError = SENSOR_STATE_ERROR;
		return sensError;
	}

	if(sens->Init.identifier == MCP9600_ACTIVE && sens->Init.interfaceType == SENSOR_INTERFACE_TYPE_I2C)
	{
		sens->i2c_data.transmitBuf[Byte1] = sens->regMap[0].regAddress;
		sens->i2c_data.nBytes = 1;
		I2C_sendData(&sens->hi2c_peripheral,&sens->i2c_data);

		sens->i2c_data.nBytes = sens->regMap[0].regSize + sens->regMap[1].regSize + sens->regMap[2].regSize;
		I2C_receiveData(&sens->hi2c_peripheral,&sens->i2c_data);
		sens->regMap[0].regValue = (sens->i2c_data.receiveBuf[Byte1] << 8) | (sens->i2c_data.receiveBuf[Byte2] << 0);
		sens->regMap[1].regValue = (sens->i2c_data.receiveBuf[Byte3] << 8) | (sens->i2c_data.receiveBuf[Byte4] << 0);
		sens->regMap[2].regValue = (sens->i2c_data.receiveBuf[Byte5] << 8) | (sens->i2c_data.receiveBuf[Byte6] << 0);
		I2C_flushData(&sens->i2c_data);
	}

	if(sens->Init.identifier == GTPMO023_ACTIVE && sens->Init.interfaceType == SENSOR_INTERFACE_TYPE_SPI)
	{
		//First Com A0
		sens->spi_data.transmitBuf[Byte1] = sens->regMap[0].regAddress;
		//Dummy Data
		sens->spi_data.transmitBuf[Byte2] = 0x00;
		sens->spi_data.transmitBuf[Byte3] = 0x00;
		sens->spi_data.nBytes = 3;
		SPI_receiveSendData(&sens->spi_peripheral, &sens->spi_data);
		sens->regMap[0].regValue = (sens->spi_data.receiveBuf[Byte2] << 8) | (sens->spi_data.receiveBuf[2] << 0);

		sens->regMap[0].regValue = (sens->spi_data.receiveBuf[Byte2] << 8) | (sens->spi_data.receiveBuf[2] << 0);
		SPI_flushData(&sens->spi_data);

		//First Com A1
		sens->spi_data.transmitBuf[Byte1] = sens->regMap[1].regAddress;
		//Dummy Data
		sens->spi_data.transmitBuf[Byte2] = 0x00;
		sens->spi_data.transmitBuf[Byte3] = 0x00;
		sens->spi_data.nBytes = 3;
		SPI_receiveSendData(&sens->spi_peripheral, &sens->spi_data);
		sens->regMap[1].regValue = (sens->spi_data.receiveBuf[Byte2] << 8) | (sens->spi_data.receiveBuf[2] << 0);
		SPI_flushData(&sens->spi_data);
	}

	if(sens->Init.identifier == PSOC4_ACTIVE && sens->Init.interfaceType == SENSOR_INTERFACE_TYPE_SPI)
	{
		sens->spi_data.transmitBuf[Byte1] = sens->regMap[1].regAddress;
		sens->spi_data.nBytes = 1;
		SPI_receiveSendData(&sens->spi_peripheral, &sens->spi_data);

		//Dummy Data
		sens->spi_data.transmitBuf[Byte1] = 0x00;
		sens->spi_data.transmitBuf[Byte2] = 0x00;
		sens->spi_data.transmitBuf[Byte3] = 0x00;
		sens->spi_data.transmitBuf[Byte4] = 0x00;

		sens->spi_data.nBytes = 4;
		SPI_receiveSendData(&sens->spi_peripheral, &sens->spi_data);
		sens->regMap[1].regValue =  (sens->spi_data.receiveBuf[Byte1] << 24 | sens->spi_data.receiveBuf[1] << 16 | sens->spi_data.receiveBuf[2] << 8 | sens->spi_data.receiveBuf[0] << 0);
		SPI_flushData(&sens->spi_data);
	}

	if(sens->Init.identifier == VTHADC_ACTIVE && sens->Init.interfaceType == SENSOR_INTERFACE_TYPE_ANALOG)
	{
		ADC_singleConversion(&sens->adc_peripheral, &sens->adc_data);
		sens->regMap[1].regValue = sens->adc_data.adcBuf[Byte1];
	}

	if(sens->Init.identifier == VFADC_ACTIVE && sens->Init.interfaceType == SENSOR_INTERFACE_TYPE_ANALOG)
	{
		ADC_singleConversion(&sens->adc_peripheral, &sens->adc_data);
		sens->regMap[1].regValue = sens->adc_data.adcBuf[Byte1];
	}

	if(sens->Init.identifier == VARIOCAM_ACIVE && sens->Init.interfaceType == SENSOR_INTERFACE_TYPE_TRIGGER)
	{
		/*Send a 120 us Trigger puls for the ir-camera*/
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_SET);
		sens->regMap[1].regValue = 0xFF; //Ir camera was triggered
		delayUSDWT(500);
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_RESET);
	}

	return sensError;
}

/**
  * @brief	Calculates the temperature as a celsius floating point value from the temperature register with a mapping that depends on the sensor identifier
  *	@param	sens pointer to the sensor handler
  * @retval returns the state of the given sensor handler after processing the function
  * 		SENSOR_STATE_READY
  *			SENSOR_STATE_BUSY
  *			SENSOR_STATE_ERROR
  */
Sensor_StateTypeDef Sensor_tempConversion(Sensor_HandleType *sens)
{
	Sensor_StateTypeDef sensError = SENSOR_STATE_READY;
	double temp = 0;

	//TODO: Asserts
	if (sens == NULL)
	{
		sensError = SENSOR_STATE_ERROR;
		return sensError;
	}

	switch (sens->Init.identifier)
	{
	case MCP9600_ACTIVE:
		/*Check for sign bit of temperture register */
		switch(sens->regMap[0].regValue & (1 << 16))
		{
		case 0 : /*Positive temperature MCP9600*/
			sens->tempDataBuf[sens->cycleCnt] = (((sens->regMap[0].regValue & 0xFF00) >> 8) * 16.0) + (((sens->regMap[0].regValue & 0x00FF) >> 0) / 16.0);
			break;

		case 1 : /*Negative temperature MCP9600*/
			sens->tempDataBuf[sens->cycleCnt] = (((sens->regMap[0].regValue & 0xFF00) >> 8) * 16.0) + (((sens->regMap[0].regValue & 0x00FF) >> 0) / 16.0)-4048.0;
			break;
		}
		break;

	case GTPMO023_ACTIVE:
		sens->tempDataBuf[sens->cycleCnt] = ( (((sens->regMap[1].regValue & 0xFF00) >> 8) * 256.0) + ((sens->regMap[1].regValue & 0x00FF) >> 0) )/100.0;
		break;

	case PSOC4_ACTIVE:
		sens->tempDataBuf[sens->cycleCnt] = ( ((sens->regMap[1].regValue & 0xFF << 24) >> 24) + ((sens->regMap[1].regValue & 0xFF << 16) >> 16) + ((sens->regMap[1].regValue & 0xFF << 8) >> 8)/1000 + ((sens->regMap[1].regValue & 0xFF << 0) >> 0)/1000 );

		break;
	case VTHADC_ACTIVE:
		temp = 10000.0 * ( ( 1 - (sens->regMap[1].regValue / 4096.0) ) / (sens->regMap[1].regValue / 4096.0) );
		sens->tempDataBuf[sens->cycleCnt] =  ( 1 / ( (1/298.15) - (log(10000) - log(temp)) / 3450 ) ) - 273.15;
		break;
	case VFADC_ACTIVE:
		//TODO: Calibration of forward voltage to temperature
		sens->tempDataBuf[sens->cycleCnt] = ( sens->regMap[1].regValue /4096.0) * 3.31;
		break;
	default:
		//TODO: Default Error
		break;

	}
	return sensError;
}

/**
  * @brief	Reads out the hardware identification number when an external sensor is used
  *	@param	sens pointer to the sensor handler
  * @retval returns the state of the given sensor handler after processing the function
  * 		SENSOR_STATE_READY
  *			SENSOR_STATE_BUSY
  *			SENSOR_STATE_ERROR
  */
Sensor_StateTypeDef Sensor_readSensorID(Sensor_HandleType *sens)
{
	Sensor_StateTypeDef sensError = SENSOR_STATE_READY;

	if (sens == NULL)
	{
		sensError = SENSOR_STATE_ERROR;
		return sensError;
	}
	if(sens->Init.identifier == MCP9600_ACTIVE && sens->Init.interfaceType == SENSOR_INTERFACE_TYPE_I2C)
	{
		sens->i2c_data.transmitBuf[Byte1] = sens->regMap[6].regAddress;
		sens->i2c_data.nBytes = 1;

		I2C_sendData(&sens->hi2c_peripheral,&sens->i2c_data);

		sens->i2c_data.nBytes = sens->regMap[6].regSize;
		I2C_receiveData(&sens->hi2c_peripheral,&sens->i2c_data);

		sens->regMap[6].regValue = (sens->i2c_data.receiveBuf[Byte1] << 8) | (sens->i2c_data.receiveBuf[Byte2] << 0);
		sens->hardwareID = (sens->i2c_data.receiveBuf[Byte1] << 8) | (sens->i2c_data.receiveBuf[Byte2] << 0);

		I2C_flushData(&sens->i2c_data);
	}

	return sensError;
}

/**
  * @brief	Determine the current state of the given sensor handler by checking if the temperature data already is updated
  *	@param	sens pointer to the sensor handler
  * @retval returns the state of the given sensor handler after processing the function
  * 		SENSOR_STATE_READY
  *			SENSOR_STATE_BUSY
  *			SENSOR_STATE_ERROR
  */
Sensor_StateTypeDef Sensor_readSensorState(Sensor_HandleType *sens)
{
	if (sens == NULL)
	{
		return SENSOR_STATE_ERROR; /* sensor is not initialized */
	}

	if(sens->Init.identifier == MCP9600_ACTIVE && sens->Init.interfaceType == SENSOR_INTERFACE_TYPE_I2C)
	{
		sens->i2c_data.transmitBuf[Byte1] = sens->regMap[3].regAddress;

		sens->i2c_data.nBytes = 1;
		I2C_sendData(&sens->hi2c_peripheral,&sens->i2c_data);

		sens->i2c_data.nBytes = sens->regMap[3].regSize;
		I2C_receiveData(&sens->hi2c_peripheral,&sens->i2c_data);

		sens->regMap[3].regValue = sens->i2c_data.receiveBuf[Byte1];
		sens->status = sens->i2c_data.receiveBuf[Byte1];

		I2C_flushData(&sens->i2c_data);

		if(sens->regMap[3].regValue & STATUS_TH_UPDATE_FLAG)
		{
			return SENSOR_STATE_READY;
		}

		return SENSOR_STATE_BUSY;
	}

	if(sens->Init.identifier == PSOC4_ACTIVE && sens->Init.interfaceType == SENSOR_INTERFACE_TYPE_SPI)
	{
		sens->spi_data.transmitBuf[0] = sens->regMap[0].regAddress;
		sens->spi_data.nBytes = 1;
		SPI_receiveSendData(&sens->spi_peripheral, &sens->spi_data);

		//Dummy Data
		sens->spi_data.transmitBuf[0] = 0x00;
		sens->spi_data.nBytes = 1;
		SPI_receiveSendData(&sens->spi_peripheral, &sens->spi_data);
		sens->regMap[0].regValue = sens->spi_data.receiveBuf[Byte1];

		if(sens->regMap[0].regValue & STATUS_TVF_UPDATE_FLAG)
		{
			return SENSOR_STATE_READY;
		}

		return SENSOR_STATE_BUSY;
	}

	if(sens->Init.identifier == VFADC_ACTIVE && sens->Init.interfaceType == SENSOR_INTERFACE_TYPE_ANALOG)
	{

		if(ADC_getState(&sens->adc_data) == ADC_STATE_READY)
		{
			return SENSOR_STATE_READY;
		}

		return SENSOR_STATE_BUSY;
	}


	if(sens->Init.identifier == VTHADC_ACTIVE && sens->Init.interfaceType == SENSOR_INTERFACE_TYPE_ANALOG)
	{

		if(ADC_getState(&sens->adc_data) == ADC_STATE_READY)
		{
			return SENSOR_STATE_READY;
		}

		return SENSOR_STATE_BUSY;
	}

	if(sens->Init.identifier == VARIOCAM_ACIVE && sens->Init.interfaceType == SENSOR_INTERFACE_TYPE_TRIGGER)
	{
		if(cameraFrameReady == 1)
		{
			cameraFrameReady = 0;
			return SENSOR_STATE_READY;
		}

		return SENSOR_STATE_BUSY;

	}

	return SENSOR_STATE_ERROR; /* sensor is not existing*/
}

/**
  * @brief	Sends the calibration information if a external sensor is used
  *	@param	sens pointer to the sensor handler
  * @retval returns the state of the given sensor handler after processing the function
  * 		SENSOR_STATE_READY
  *			SENSOR_STATE_BUSY
  *			SENSOR_STATE_ERROR
  */
Sensor_StateTypeDef Sensor_setSensorCalibration(Sensor_HandleType *sens)
{
	Sensor_StateTypeDef sensError = SENSOR_STATE_READY;

	if (sens == NULL)
	{
		sensError = SENSOR_STATE_ERROR;
		return sensError;
	}
	if(sens->Init.identifier == MCP9600_ACTIVE && sens->Init.interfaceType == SENSOR_INTERFACE_TYPE_I2C )
	{

		sens->i2c_data.transmitBuf[Byte1] = sens->regMap[4].regAddress;
		sens->i2c_data.transmitBuf[Byte2] = MCP9600_OPTIONS_BYTE1;
		sens->i2c_data.nBytes = 1 + sens->regMap[4].regSize;
		I2C_sendData(&sens->hi2c_peripheral,&sens->i2c_data);

		sens->i2c_data.nBytes = sens->regMap[4].regSize;
		I2C_receiveData(&sens->hi2c_peripheral,&sens->i2c_data);

		if(sens->i2c_data.receiveBuf[Byte1] != sens->regMap[4].regValue)
		{
			sensError = SENSOR_STATE_ERROR;
			return sensError;
		}

		sens->i2c_data.transmitBuf[Byte1] = sens->regMap[5].regAddress;
		sens->i2c_data.transmitBuf[Byte2] = MCP9600_OPTIONS_BYTE2;
		sens->i2c_data.nBytes = 1 + sens->regMap[5].regSize;
		I2C_sendData(&sens->hi2c_peripheral,&sens->i2c_data);

		sens->i2c_data.nBytes = sens->regMap[5].regSize;
		I2C_receiveData(&sens->hi2c_peripheral,&sens->i2c_data);

		if(sens->i2c_data.receiveBuf[Byte1] != sens->regMap[5].regValue)
		{
			sensError = SENSOR_STATE_ERROR;
			return sensError;
		}
		I2C_flushData(&sens->i2c_data);
	}

	return sensError;
}


/**
  * @brief	Reads out the current time from the RTC peripheral
  *	@param	sens pointer to the sensor handler
  * @retval returns the state of the given sensor handler after processing the function
  * 		SENSOR_STATE_READY
  *			SENSOR_STATE_BUSY
  *			SENSOR_STATE_ERROR
  */
Sensor_StateTypeDef Sensor_setTimeStamp(Sensor_HandleType *sens)
{
	HAL_StatusTypeDef hal_state;

	hal_state = HAL_RTC_GetTime(&hrtc, &sens->timeStampBuf[sens->cycleCnt], RTC_FORMAT_BIN);
	if (hal_state != HAL_OK)
	{
		return SENSOR_STATE_BUSY;
	}
	hal_state = HAL_RTC_GetDate(&hrtc, &sens->dateStampBuf[sens->cycleCnt], RTC_FORMAT_BIN);
	if (hal_state != HAL_OK)
	{
		return SENSOR_STATE_BUSY;
	}
	return SENSOR_STATE_READY;
}


/**
  * @brief	Returns the time for the latest cycle
  *	@param	sens pointer to the sensor handler
  * @retval time value of the in the RTC format
  */
RTC_TimeTypeDef Sensor_getTimeData(Sensor_HandleType *sens)
{
	RTC_TimeTypeDef temp  = sens->timeStampBuf[sens->cycleCnt-1];
	return temp;
}

/**
  * @brief	Reads out the floating point celsius temperature of the latest cycle
  *	@param	sens pointer to the sensor handler
  * @retval celsius temperature value
  */
double Sensor_getTempData(Sensor_HandleType *sens)
{
	return sens->tempDataBuf[sens->cycleCnt-1];
}


/**
  * @brief	Reads out the latest stored value for a given register address
  *	@param	sens pointer to the sensor handler
  *	@param regAddress address to the desired register value
  * @retval register value that is stored
  */
uint32_t Sensor_getRegData(Sensor_HandleType *sens, uint16_t regAddress)
{
	uint32_t regMapSize;
	int i;

	regMapSize = sizeof(sens->regMap);
	for(i=0; i<regMapSize; i++)
	{
		if(sens->regMap[i].regAddress == regAddress)
			return sens->regMap[i].regValue;
	}

	return 0;
}

/**
  * @brief	Increments the cycle counter for given the sensor handler
  *	@param	sens pointer to the sensor handler
  * @retval register value that is stored
  */
void Sensor_incrementCycleCnt(Sensor_HandleType *sens)
{
	if(sens->cycleCnt >= 127)
	{
		sens->cycleCnt = 0;
	}
	else
	{
		sens->cycleCnt++;
	}
}

/**
  * @brief	Sets dummy data to the sensor registers when the temperature data is not yet updated or the peripheral is busy in a cycle
  *	@param	sens pointer to the sensor handler
  * @retval register value that is stored
  */
Sensor_StateTypeDef Sensor_putDummyData(Sensor_HandleType *sens)
{

	if(sens->Init.identifier == VARIOCAM_ACIVE && sens->Init.interfaceType == SENSOR_INTERFACE_TYPE_TRIGGER)
	{

	}
	Sensor_StateTypeDef sensError = SENSOR_STATE_READY;

	if (sens == NULL)
	{
		sensError = SENSOR_STATE_ERROR;
		return sensError;
	}
	switch(sens->Init.identifier)
	{
	case MCP9600_ACTIVE:
		sens->regMap[0].regValue = 0x00; //MCP9600 Data not updatet
		break;
	case GTPMO023_ACTIVE:
		sens->regMap[1].regValue = 0x00; //GTPM023 Data not updatet
		break;
	case PSOC4_ACTIVE:
		sens->regMap[1].regValue = 0x00; //PSOC Data not updatet
		break;
	case VFADC_ACTIVE:
		sens->regMap[1].regValue = 0x00; //ADC conversion not complete
		break;
	case VTHADC_ACTIVE:
		sens->regMap[1].regValue = 0x00; //ADC conversion not complete
		break;
	case VARIOCAM_ACIVE:
		sens->regMap[1].regValue = 0x00; //Ir-Camera was not triggered
	default: //TODO: ERROR
		break;
	}
	return sensError;
}

/* Callback functions -----------------------------------------------------------*/

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	if(htim->Instance == TIM6)
	{
		cameraFrameReady = 1;
	}
}
