/*******************************************************************************
* File Name: cyapicallbacks.h
* Version 1.0
*
* Description:
*
* Note: none
*
********************************************************************************/
#ifndef CYAPICALLBACKS_H
#define CYAPICALLBACKS_H
    
#include "myspi.h"

    /*Define your macro callbacks here */
    /*For more information, refer to the Writing Code topic in the PSoC Creator Help.*/

  //  #define SPIS_TX_ISR_EXIT_CALLBACK 
    #define SPIS_SPI_UART_ISR_EXIT_CALLBACK 
    
    #define ADC_ISR_INTERRUPT_CALLBACK
   
    #define ISR_TurnOn_Sink_INTERRUPT_INTERRUPT_CALLBACK
    #define ISR_TurnOff_Sink_INTERRUPT_INTERRUPT_CALLBACK
  
void SPIS_SPI_UART_ISR_ExitCallback(void);
void ISR_TurnOn_Sink_Interrupt_InterruptCallback(void);
void ISR_TurnOff_Sink_Interrupt_InterruptCallback(void);
   
#endif /* CYAPICALLBACKS_H */   
/* [] */
