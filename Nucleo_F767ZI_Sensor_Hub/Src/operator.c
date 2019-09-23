/*
 * operator.c
 *
 *  Created on: Jun 14, 2019
 *      Author: Lars
 */


/* Includes ------------------------------------------------------------------*/
#include "operator.h"


/* Private defines -----------------------------------------------------------*/
#define UART_STANDARD_TIMEOUT 100

#define UART_WRITE_BUF_SIZE		32U
#define UART_READ_BUF_SIZE		32U
#define OPERATOR_COMMAND_SIZE 	18U
#define COMMAND_PRE_LENGTH		8U
#define COMMAND_ID_LENGTH		3U
#define COMMAND_DATA_LENGTH		6U


/* Private variables ---------------------------------------------------------*/
extern Sensor_HandleType sens1;
extern Sensor_HandleType sens2;
extern Sensor_HandleType sens3;
extern Sensor_HandleType sens4;
extern Sensor_HandleType sens5;

UART_HandleTypeDef *huart3p;

/* Global variables for managing UART interrupts */
uint8_t dataAquistionStart = 0;
uint8_t uartDataReceived = 0;
uint8_t uartDataSend = 0;

/* Global Structure to store the latest command for processing */
typedef struct
{
	char command[OPERATOR_COMMAND_SIZE];
	char commandFlag[COMMAND_PRE_LENGTH+1];
	char commandId[COMMAND_ID_LENGTH+1];
	char commandData[COMMAND_DATA_LENGTH+1];
	int8_t commandNr;
	uint32_t commandParameter1;
}Operator_uartReceiveData;

Operator_uartReceiveData Operator_rec;

/* Private function prototypes --------------------------------------------------*/
static void Operator_initBuffer(Operator_UartDataType *uart_data);
static void parseCommand(Operator_UartDataType *uart_data);

/* Private Function Implementation -----------------------------------------------*/

/**
  * @brief	Sets the transmit and receive buffer to 0 before the first communication
  * @param 	uart_data pointer the to user UART-data structure
  * @retval NONE
  */
static void Operator_initBuffer(Operator_UartDataType *uart_data)
{
	//TODO: Asserts
	//Init WriteBuf
	for(uint8_t i = 0; i < UART_WRITE_BUF_SIZE; i++)
	{
		uart_data->transmitBuf[i] = 0;
	}

	//Init ReadBuf
	for(uint8_t i = 0; i < UART_READ_BUF_SIZE; i++)
	{
		//uart_data->receiveBuf[i] = 0;
	}
}

/**
  * @brief	This functions parses the incoming operator message and stores the different parts in the command structure
  * @param 	uart_data pointer the to user UART-data structure
  * @retval NONE
  */
static void parseCommand(Operator_UartDataType *uart_data)
{
	uint8_t from;
	uint8_t until;

	strncpy(Operator_rec.command, (char*)uart_data->receiveBuf, OPERATOR_COMMAND_SIZE);

	from = 0;
	until = COMMAND_PRE_LENGTH;

	for(unsigned i = from; i <= until; i++)
	{
		Operator_rec.commandFlag[i - from] = Operator_rec.command[i];
	}
	Operator_rec.commandFlag[COMMAND_PRE_LENGTH] = '\0';

	from = COMMAND_PRE_LENGTH;
	until = COMMAND_PRE_LENGTH + COMMAND_ID_LENGTH;

	if(strcmp(Operator_rec.commandFlag, "COMMAND:") == 0 )
	{
		for(unsigned i = from; i <= until; i++)
		{
			Operator_rec.commandId[i-from] = Operator_rec.command[i];
		}
		Operator_rec.commandId[COMMAND_ID_LENGTH] = '\0';

		Operator_rec.commandNr = atoi(Operator_rec.commandId);
	}
	else
	{
		Operator_rec.commandNr = -1;
	}

	from = COMMAND_PRE_LENGTH + COMMAND_ID_LENGTH + 1;
	until = COMMAND_PRE_LENGTH + COMMAND_ID_LENGTH + 1 + COMMAND_DATA_LENGTH;

	if(Operator_rec.commandNr == OPERATOR_DATASIZE_COMMAND)
	{
		for(unsigned i = from; i <= until; i++)
		{
			Operator_rec.commandData[i - from] = Operator_rec.command[i];
		}
		Operator_rec.commandData[COMMAND_DATA_LENGTH] = '\0';

		Operator_rec.commandParameter1 = atoi(Operator_rec.commandData);
	}

}

/* Public Function Implementation -----------------------------------------------*/

/**
  * @brief	This functions sets the UART interface for a given operator, configures the peripheral and call the HAL init functions.
  * 		The varying UART parameters will already be set from the user in the main application file.
  	@param	huart pointer to the UART HAL peripheral handler
  * @param 	uart_data pointer the to user UART-data structure
  * @retval NONE
  */
void Operator_uartInitOperatorInterface(UART_HandleTypeDef *huart,Operator_UartDataType *uart_data )
{
	//TODO: Asserts
/*General settings which are not intended to be changed by operator*/
  huart->Init.Mode = UART_MODE_TX_RX;
  huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart->Init.OverSampling = UART_OVERSAMPLING_16;
  huart->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(huart) != HAL_OK)
  {
    //TODO: Error
  }
  if(huart->Instance == USART3)
  {
	  huart3p = huart;
  }
  Operator_initBuffer(uart_data);
}

/**
  * @brief	Sets the transmit buffer for sending the temperature data collected in the latest cycle and starts the DMA controller for sending out data
  	@param	huart pointer to the UART HAL peripheral handler
  * @param 	uart_data pointer the to user UART-data structure
  * @retval NONE
  */
void Operator_uartSendTempData(UART_HandleTypeDef *huart, Operator_UartDataType *uart_data)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET); /*Indication for time to serve operator*/

	uint8_t temperature_byte1;
	uint8_t temperature_byte2;
	uint8_t temperature_byte3;
	uint8_t temperature_byte4;

	RTC_TimeTypeDef timestampValue;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint8_t subseconds1;
	uint8_t subseconds2;

	/*Sensor 1 Thermocouple*/

	temperature_byte1 = ( (Sensor_getRegData(&sens1, 0x00) & 0xFF00) >> 8);
	temperature_byte2 = ( (Sensor_getRegData(&sens1, 0x00) & 0x00FF) >> 0);

	timestampValue=  Sensor_getTimeData(&sens1);
	hours = timestampValue.Hours;
	minutes = timestampValue.Minutes;
	seconds = timestampValue.Seconds;
	subseconds1 = ( (timestampValue.SubSeconds & 0xFF << 0) >> 0);
	subseconds2 = ( (timestampValue.SubSeconds & 0xFF << 8) >> 8);

	uart_data->transmitBuf[0] = hours;
	uart_data->transmitBuf[1] = minutes;
	uart_data->transmitBuf[2] = seconds;
	uart_data->transmitBuf[3] = subseconds1;
	uart_data->transmitBuf[4] = subseconds2;
	uart_data->transmitBuf[5] = temperature_byte1;
	uart_data->transmitBuf[6] = temperature_byte2;

	/*Sensor 2 Thermopile*/

	temperature_byte1 = ( (Sensor_getRegData(&sens2, 0xA1) & 0xFF << 24) >> 24);
	temperature_byte2 = ( (Sensor_getRegData(&sens2, 0xA1) & 0xFF << 16) >> 16);
	temperature_byte3 = ( (Sensor_getRegData(&sens2, 0xA1) & 0xFF << 8) >> 8);
	temperature_byte4 = ( (Sensor_getRegData(&sens2, 0xA1) & 0xFF << 0) >> 0);

	uart_data->transmitBuf[7] = temperature_byte1;
	uart_data->transmitBuf[8] = temperature_byte2;
	uart_data->transmitBuf[9] = temperature_byte3;
	uart_data->transmitBuf[10] = temperature_byte4;

	/*Sensor 3 Thermistor*/

	temperature_byte1 = ( (Sensor_getRegData(&sens3, 0xA1) & 0xFF << 24) >> 24);
	temperature_byte2 = ( (Sensor_getRegData(&sens3, 0xA1) & 0xFF << 16) >> 16);
	temperature_byte3 = ( (Sensor_getRegData(&sens3, 0xA1) & 0xFF << 8) >> 8);
	temperature_byte4 = ( (Sensor_getRegData(&sens3, 0xA1) & 0xFF << 0) >> 0);

	uart_data->transmitBuf[11] = temperature_byte1;
	uart_data->transmitBuf[12] = temperature_byte2;
	uart_data->transmitBuf[13] = temperature_byte3;
	uart_data->transmitBuf[14] = temperature_byte4;

	/*Sensor 4 TSEP*/

	temperature_byte1 = ( (Sensor_getRegData(&sens4, 0xA1) & 0xFF << 24) >> 24);
	temperature_byte2 = ( (Sensor_getRegData(&sens4, 0xA1) & 0xFF << 16) >> 16);
	temperature_byte3 = ( (Sensor_getRegData(&sens4, 0xA1) & 0xFF << 8) >> 8);
	temperature_byte4 = ( (Sensor_getRegData(&sens4, 0xA1) & 0xFF << 0) >> 0);

	uart_data->transmitBuf[15] = temperature_byte1;
	uart_data->transmitBuf[16] = temperature_byte2;
	uart_data->transmitBuf[17] = temperature_byte3;
	uart_data->transmitBuf[18] = temperature_byte4;

	/*Sensor 5 Thermocamera*/

	temperature_byte1 = Sensor_getRegData(&sens5, 0xA1);

	uart_data->transmitBuf[19] = temperature_byte1;

	uart_data->nBytes = 20;

	HAL_UART_Transmit_DMA(huart, uart_data->transmitBuf,uart_data->nBytes);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
}

/**
  * @brief	Sets the operator back to listening mode if the last command ist processed
  	@param	huart pointer to the UART HAL peripheral handler
  * @param 	uart_data pointer the to user UART-data structure
  * @retval NONE
  */
void Operator_uartListenCommand(UART_HandleTypeDef *huart, Operator_UartDataType *uart_data)
{
	if(uartDataReceived == 0)
	{
		HAL_UART_Receive_DMA(huart, uart_data->receiveBuf, OPERATOR_COMMAND_SIZE+1);
	}
}

/**
  * @brief	Processes the command given by the operator
  	@param	huart pointer to the UART HAL peripheral handler
  * @param 	uart_data pointer the to user UART-data structure
  * @retval NONE
  */
uint8_t Operator_processCommand(UART_HandleTypeDef *huart, Operator_UartDataType *uart_data)
{
	if(uartDataReceived)
	{

		uartDataReceived = 0;
		parseCommand(uart_data);
		memset(uart_data->receiveBuf,'\0',32);

		switch(Operator_rec.commandNr)
		{
		case 1:
			return OPERATOR_START_COMMAND;
			break;
		case 2:
			uart_data->dataSize = Operator_rec.commandParameter1;
			return OPERATOR_DATASIZE_COMMAND;
			break;
		case -1:
			return OPERATOR_NO_COMMAND;
			break;
		default:
			return OPERATOR_NO_COMMAND;
			break;
		}
	}

	return 0;
}

/**
  * @brief	Returns the dataSize for the next data acquisition which is set by the operator
  * @param 	uart_data pointer the to user UART-data structure
  * @retval NONE
  */
uint32_t Operator_getDataSize(Operator_UartDataType *uart_data)
{
	return uart_data->dataSize;
}

/* Callback Function Implementation -----------------------------------------------*/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uartDataReceived = 1;
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	uartDataSend = 1;
}

