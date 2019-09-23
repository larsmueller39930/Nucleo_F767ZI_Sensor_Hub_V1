/*******************************************************************************
* File Name: myspi.c
* Version 1.0
*
* Description:
*
* Note: none
*
********************************************************************************/

#include "myspi.h"

volatile int32 SPI_Temp_Reg;
volatile int8 SPI_Status_Reg;

uint8 SPI_RXflag;
uint8 SPI_TXflag;
uint8 spiRxBuffer[128];
uint8 spiTxBuffer[128];

/********************************************************************************
*   Function Definitions
********************************************************************************/

/********************************************************************************** 
* Function Name: initializeSPIInterface
***********************************************************************************
*
* Summary:
*  Initialize the SPI interface for communication with master 

* Parameters: none 
*
* Return: none
* 
*************************************************************************************/
void initializeSPIInterface(void)
{    
    uint8 status;
    uint8 nBytes;
    
    SPIS_Start(); //Starts the SPI peripheral and enables the RX and TX Interrupts
    SPI_RXflag = 0;
    SPI_TXflag = 0;
    SPIS_EnableInt();
    SPIS_SetRxInterruptMode(SPIS_INTR_RX_NOT_EMPTY);
    SPIS_SpiUartClearTxBuffer(); 
    SPIS_SpiUartClearRxBuffer(); 
 
}

/********************************************************************************** 
* Function Name: setSPITempReg
***********************************************************************************
*
* Summary:
*  Puts the conversion result of the adc in the temperature register

* Parameters:  
*  intrHigh: high byte before radix point
*  intrLow:  low byte before radix point
*  fracHigh: high byte after radix point
*  fracLow:  low byte before radix point
*
* Return: none
* 
*************************************************************************************/
void setSPITempReg(uint8 intrHigh, uint8 intrLow, uint8 fracHigh, uint8 fracLow)
{
    SPI_Temp_Reg = (intrHigh << 24u | intrLow << 16u | fracHigh << 8u | fracLow << 0u);
}

/********************************************************************************** 
* Function Name: SPIS_SPI_UART_ISR_ExitCallback
***********************************************************************************
*
* Summary:
*  Callback function when a byte was received or transmitted over the SPI interface

* Parameters: none 
*
* Return: none
* 
*************************************************************************************/
void SPIS_SPI_UART_ISR_ExitCallback(void)
{
    uint8 status;
    uint8 nBytes;
    
    //RX ISR
    status = SPIS_GetInterruptCause();
    if(status == SPIS_INTR_CAUSE_RX)
    {
        spiRxBuffer[0] = SPIS_SpiUartReadRxData();
       
        nBytes = SPIS_SpiUartGetRxBufferSize();
        status = SPIS_GetSlaveInterruptSourceMasked();
        status = SPIS_GetRxInterruptMode();
        
        if(spiRxBuffer[0] == REG_ADDRESS_TEMP )
        {       
            SPIS_SpiUartClearTxBuffer(); 

            spiTxBuffer[0]   = (SPI_Temp_Reg & (0xFF<<24u)) >> 24u; 
            spiTxBuffer[1]   = (SPI_Temp_Reg & (0xFF<<16u)) >> 16u; 
            spiTxBuffer[2]   = (SPI_Temp_Reg & (0xFF<<8u)) >> 8u; 
            spiTxBuffer[3]   = (SPI_Temp_Reg & (0xFF<<0u));

            SPIS_SpiUartPutArray((uint8*)spiTxBuffer, 4);     
           __SPI_CLEAR_STATUS_BIT(STATUS_CONVERSION_DONE);
        }   
        else if(spiRxBuffer[0] == REG_ADDRESS_STATUS )
        {
            SPIS_SpiUartClearTxBuffer(); 
            
          
            spiTxBuffer[0] = SPI_Status_Reg;
            
            SPIS_SpiUartPutArray((uint8*)spiTxBuffer, 1);     
        }
        
        SPIS_SpiUartClearRxBuffer(); 
        SPIS_ClearRxInterruptSource(SPIS_INTR_RX_NOT_EMPTY); 
    }
  
    
    status = 0;
}

/* [] END OF FILE */
