/*******************************************************************************
* File Name: currentSink.h
* Version 1.0
*
* Description:
*
* Note: none
*
********************************************************************************/
#ifndef CURRENTSINK_H
#define CURRENTSINK_H
#include <project.h>
#include "myspi.h"
    
/********************************************************************************
*   Defintions 
********************************************************************************/ 
    
 /*********************************************************************************************/
/* IDAC high, low currents used to change the current value in the current sink regulator shield
	For PSoC 4, the IDAC is set to 2.4uA/bit in the 0-612uA range
	Low current corresponds to 50mA in the current sink regulator and to the lowest IDAC value 00
    High current corresponds to 100mA in the current sink regulator and to the highest IDAC value FF     
*********************************************************************************************/
#define IDAC_LOW_CURRENT                0x00
#define IDAC_HIGH_CURRENT               0xFF
#define IDAC_HIGH_100uA                 0x2A
#define IDAC_HIGH_25uA                  0xA
    
#define GPIO_WRITE_LOW                  0u   
#define GPIO_WRITE_HIGH                 1u

#define CURRENT_SINK_STATE_ON           0u
#define CURRENT_SINK_STATE_OFF          1u
    
#define CURRENT_SINK_TURNON_PENDING     1u
#define CURRENT_SINK_TURNOFF_PENDING    2u
    
 /*********************************************************************************************/
/* The current sink shield have a multiplexer to drive current through all 4 body diodes of the DUT 
   CHANNEL_1: SIC-Transistor 1 
   CHANNEL_2: SIC-Transistor 2
   CHANNEL_3: SIC-Transistor 3
   CHANNEL_4: SIC-Transistor 4
*********************************************************************************************/  
#define MULTIPLEXER_CHAN1               0u
#define MULTIPLEXER_CHAN2               1u
#define MULTIPLEXER_CHAN3               2u
#define MULTIPLEXER_CHAN4               3u
      
    
/********************************************************************************
*   Function Prototypes 
********************************************************************************/
void turnOffSink(void);
void turnOnSink(void);
void setLowCurrent(void);
void setHighCurrent(void);
void setMultiplexerChan(uint8 chan);  
void initializeCurrentSink(void);
uint8 getISRState(void);    
void resetISRState(void);
uint8 getCurrentSourceState(void);

#endif
/* [] END OF FILE */
