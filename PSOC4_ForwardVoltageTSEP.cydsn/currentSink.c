/*******************************************************************************
* File Name: currenSink.c
* Version 1.0
*
* Description:
*
* Note: none
*
********************************************************************************/
#include "currentSink.h"

extern volatile int8 SPI_Status_Reg;
volatile uint8 currentSinkToggleRequest;
uint8 currentSourceState;

/********************************************************************************
*   Function Definitions
********************************************************************************/


/********************************************************************************** 
* Function Name: initializeCurrentSink
***********************************************************************************
*
* Summary:
*  Initialize the current sink IDAC and external interrupts

* Parameters: none 
*
* Return: none
* 
*************************************************************************************/
void initializeCurrentSink(void)
{
    IDAC_Start();
   // Pin_Toggle_Sink_SetInterruptMode(Pin_Toggle_Sink_0_INTR,Pin_Toggle_Sink_INTR_RISING);
    ISR_TurnOn_Sink_Start(); 
   //     Pin_Toggle_Sink_SetInterruptMode(Pin_Toggle_Sink_0_INTR,Pin_Toggle_Sink_INTR_RISING);
    ISR_TurnOff_Sink_Start();     
}

/********************************************************************************** 
* Function Name: setHighCurrent
***********************************************************************************
*
* Summary: Set the IDAC value to drive a high current (IDAC value = 255)
* 

* Parameters: none 
*
* Return: none
* 
*************************************************************************************/
void setHighCurrent(void)
{ 
    if(GPIO_EN_Read() == GPIO_WRITE_LOW && GPIO_VREF_Read() == GPIO_WRITE_HIGH) //Check if the Multiplexer is enabled to prevent driving in high z
    {
    IDAC_SetValue(IDAC_HIGH_CURRENT);   //Set the current to 100mA 
    }   
}

/********************************************************************************** 
* Function Name: setHighCurrent
***********************************************************************************
*
* Summary: Set the IDAC value to drive a low current (IDAC value = 0)
* 

* Parameters: none 
*
* Return: none
* 
*************************************************************************************/
void setLowCurrent(void)
{ 
    if(GPIO_EN_Read() == GPIO_WRITE_LOW && GPIO_VREF_Read() == GPIO_WRITE_HIGH) //Check if the Multiplexer is enabled to prevent driving in high z
    {   
    IDAC_SetValue(IDAC_LOW_CURRENT);   //Set the current to 50mA   
    } 
}

/********************************************************************************** 
* Function Name: turnOnSink
***********************************************************************************
*
* Summary: Turn on the current by setting the corresponding GPIO and IDAC value
* 

* Parameters: none 
*
* Return: none
* 
*************************************************************************************/
void turnOnSink(void)
{   
    GPIO_EN_Write(GPIO_WRITE_LOW);
    CyDelayUs(30); //Wait 30us for switching time
    GPIO_VREF_Write(GPIO_WRITE_HIGH);  
    IDAC_SetValue(IDAC_HIGH_CURRENT);
    __SPI_SET_STATUS_BIT(STATUS_CURRENT_SOURCE_ON);
    currentSourceState = 1;
}

/********************************************************************************** 
* Function Name: turnOffSink
***********************************************************************************
*
* Summary: Turn pff the current by resetting the corresponding GPIO and IDAC value
* 

* Parameters: none 
*
* Return: none
* 
*************************************************************************************/
void turnOffSink(void)
{ 
   GPIO_VREF_Write(GPIO_WRITE_LOW); 
   IDAC_SetValue(IDAC_LOW_CURRENT);   
   CyDelayUs(30); //Wait 30us for switching time
   GPIO_EN_Write(GPIO_WRITE_HIGH); 
   __SPI_CLEAR_STATUS_BIT(STATUS_CURRENT_SOURCE_ON);
   currentSourceState = 0;
}

/********************************************************************************** 
* Function Name: setMultiplexerChan
***********************************************************************************
*
* Summary: Set the GPIOs to set the multiplexer to drive a specific transistor for TSEP measurement
* 
* Parameters: chan channel that will be set active
*           CHANNEL_1: SIC-Transistor 1 
*           CHANNEL_2: SIC-Transistor 2
*           CHANNEL_3: SIC-Transistor 3
*           CHANNEL_4: SIC-Transistor 4
*
* Return: none
* 
*************************************************************************************/
void setMultiplexerChan(uint8 chan)
{
    
    switch(chan)
    {
    case MULTIPLEXER_CHAN1: 
        GPIO_S0_Write(GPIO_WRITE_LOW);
        GPIO_S1_Write(GPIO_WRITE_LOW);
        break;
    case MULTIPLEXER_CHAN2: 
        GPIO_S0_Write(GPIO_WRITE_HIGH);
        GPIO_S1_Write(GPIO_WRITE_LOW);
        break;
    case MULTIPLEXER_CHAN3: 
        GPIO_S0_Write(GPIO_WRITE_LOW);
        GPIO_S1_Write(GPIO_WRITE_HIGH);
        break;
    case MULTIPLEXER_CHAN4: 
        GPIO_S0_Write(GPIO_WRITE_HIGH);
        GPIO_S1_Write(GPIO_WRITE_HIGH);
        break;
    default: 
        //No valid Channel
        break;
    }
}

/********************************************************************************** 
* Function Name: getCurrentSourceState
***********************************************************************************
*
* Summary: Returns if the current source is turned on or of
* 
* Parameters: None
*
* Return: The state of the current source
*           CURRENT_SINK_STATE_ON      current sink on
*           CURRENT_SINK_STATE_OFF     current sink off
* 
*************************************************************************************/
uint8 getCurrentSourceState(void)
{
    return currentSourceState;
}

/********************************************************************************
* Handling of the Interrupts to turn on and off the current sink for measurement 
* of the forward voltage using an external adc
********************************************************************************/


/********************************************************************************** 
* Function Name: getISRState
***********************************************************************************
*
* Summary: Returns if a turn on or turn off request is currently active
* 
* Parameters: None
*
* Return: The ISR request type 
*           0U                              no request
*           CURRENT_SINK_TURNON_PENDING     turn on request
*           CURRENT_SINK_TURNOFF_PENDING    turn off request

*************************************************************************************/
uint8 getISRState(void)
{
    return currentSinkToggleRequest;
}

/********************************************************************************** 
* Function Name: resetISRState
***********************************************************************************
*
* Summary: Resets the request state after processing of interrupt
* 
* Parameters: None
*
* Return: None
*
*************************************************************************************/
void resetISRState(void)
{
    currentSinkToggleRequest = 0;
}

/********************************************************************************** 
* Function Name: ISR_TurnOff_Sink_Interrupt_InterruptCallback
***********************************************************************************
*
* Summary: Callback that sets the request to pending turned on
* 
* Parameters: None
*
* Return: None
*
*************************************************************************************/
void ISR_TurnOn_Sink_Interrupt_InterruptCallback(void)
{
    currentSinkToggleRequest = CURRENT_SINK_TURNON_PENDING;
    ISR_TurnOn_Sink_ClearPending();
}

/********************************************************************************** 
* Function Name: ISR_TurnOff_Sink_Interrupt_InterruptCallback
***********************************************************************************
*
* Summary: Callback that sets the request to pending turned off
* 
* Parameters: None
*
* Return: None
*
*************************************************************************************/
void ISR_TurnOff_Sink_Interrupt_InterruptCallback(void)
{
    currentSinkToggleRequest = CURRENT_SINK_TURNOFF_PENDING;
    ISR_TurnOff_Sink_ClearPending();    
}

/* [] END OF FILE */

