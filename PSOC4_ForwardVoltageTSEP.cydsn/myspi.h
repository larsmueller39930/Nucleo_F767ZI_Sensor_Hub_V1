/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#ifndef MYSPI_H
#define MYSPI_H
#include <project.h>

/********************************************************************************
*   Defintions 
********************************************************************************/ 

/*Register address for status byte and temperature data*/
#define REG_ADDRESS_STATUS          0xA0
#define REG_ADDRESS_TEMP            0xA1
#define DUMMY_ADDRESS_RESPONSE      0xFF
      
/*Definition of the bit values in the status byte*/
#define STATUS_CONVERSION_DONE      0u  
#define STATUS_CURRENT_SOURCE_ON    1u
#define STATUS_CALIBRATION_DONE     2u
#define STATUS_ADC_OVERFLOW         3u
 
/*Makros for setting and clearing the status bit*/
#define __SPI_GET_STATUS_BIT(_SPI_STATUS_BIT_) ((SPI_STATUS_REG & (1u <<_SPI_STATUS_BIT_) >> _SPI_STATUS_BIT_)
#define __SPI_SET_STATUS_BIT(_SPI_STATUS_BIT_) (SPI_Status_Reg |= (1u <<_SPI_STATUS_BIT_))
#define __SPI_CLEAR_STATUS_BIT(_SPI_STATUS_BIT_) (SPI_Status_Reg &= ~(1u <<_SPI_STATUS_BIT_))
/********************************************************************************
*   Function Prototypes 
********************************************************************************/

void SPIS_TX_ISR_ExitCallback(void);
void SPIS_RX_ISR_ExitCallback(void);  
    
void setSPITempReg(uint8 intrHigh, uint8 intrLow, uint8 fracHigh, uint8 fracLow);
void initializeSPIInterface(void);
void sendTempData(void);
#endif
/* [] END OF FILE */
