/*******************************************************************************
* File Name: main.c
* Version 1.0
*
* Description:
*
* Note: none
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include <device.h>
#include "diodeMeasurement.h"
#include "currentSink.h"
#include "myspi.h"

#define CURRENT_SINK_MODE   0U /*The PSOC only serves as a current sink. ADC forward voltage
                                 measurement is done in the STM32 Nucleo board*/
#define SENSOR_MODE         1U /*The PSOC serves as a current sink and also do the ADC forward 
                                 voltage is conducted by the PSOC and sended out per SPI*/

uint8 mode = CURRENT_SINK_MODE;

extern volatile uint8 SPI_RXflag;
extern volatile uint8 SPI_TXflag;
extern volatile int16 ADCOffset;

int main()
{
    /* Enable global interrupts. */
	CyGlobalIntEnable; 

    /*The PSOC works in Sensor-Mode and using the internal adc for measure the TSEP voltage*/
    if (mode == SENSOR_MODE) 
    {     
        int32 calib_temperature = 0;
        float32 diode_temperature;
	    int16 psoc_tempCounts;
	    int16 psoc_temperature;
	    int16 temperature_diff;
        
        /* Initialize the components for diode temperature measurement mode*/
         initializeDiodeMeasurement();
    
        /* Initialize the components for SPI communication*/
        initializeSPIInterface();
        
        /* Start ADC conversion and wait for the injection channel conversion completion */
        ADC_SAR_StartConvert();
    	
    	/* Enables injection channel for next scan */
        ADC_SAR_EnableInjection();
        ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT); 
        psoc_temperature = ADC_SAR_GetResult16(0);

    	/*Get die temperature temperature from the injection channel which is at Channel 1*/
        calib_temperature = DieTemp_CountsTo_Celsius(psoc_tempCounts);
        ADC_SAR_StopConvert();
        
        for(;;)
        {        
            /* Get diode temperature and print on LCD */
            diode_temperature = getDiodeTemperature(MULTIPLEXER_CHAN1);	
        }    
    }
    
    /*The PSOSC only works as a controlled current sink and the measurement is done with a external adc*/
    if (mode == CURRENT_SINK_MODE)
    {
        uint8 ISRStatus = 0;
    
        /* Initialize the components for current sink mode*/
        initializeCurrentSink();
        
        for(;;)
        {
        ISRStatus = getISRState();
        
            if (ISRStatus != 0)
            {             
                if( ISRStatus == CURRENT_SINK_TURNON_PENDING)
                {
                    turnOnSink();
                    resetISRState();
                    ISRStatus = 0;
                }
                if( ISRStatus == CURRENT_SINK_TURNOFF_PENDING)
                {   
                    turnOffSink();
                    resetISRState();
                    ISRStatus = 0;
                }
            }        
        }
    }
	return 1;
}

/* [] END OF FILE */
