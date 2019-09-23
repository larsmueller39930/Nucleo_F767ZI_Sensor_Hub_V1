/*******************************************************************************
* File Name: diodeMeasurement.c
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

/* Include all generated header files  */
#include <device.h>  
 /* Included for using logarithm function */
#include <math.h>   
 /* Included the single diode header file */
#include <diodeMeasurement.h>

/* Opamps and ADC offset at delta Vbe measurement */
volatile int16 ADCOffset = 0; 
extern volatile int32 SPI_Temp_Reg;
extern volatile int8 SPI_Status_Reg;

/********************************************************************************** 
* Function Name: getSystemOffset
***********************************************************************************
*
* Summary:
*  Get the total offset caused by opamps and ADC 
* Parameters:  
* number: the averaging number for noise reduction
*
* Return: 
* offset caused by both ADC and opamps are returned in this function as: "Adc_offset"
*************************************************************************************/

int32 getSystemOffset(int8 number)
{
	uint8 loop = 0;	
	int32 Offset = 0;
	 /*Set the ADC multiplexer to short OAMPS to get ADC + Amplifier Offset */
	AMux_High_Select(AMUX_HIGH_OFFSET);	
	AMux_Low_Select (AMUX_LOW_OFFSET); 
    
	/*add some delay to makes sure the last conversion is complete*/
	CyDelayUs(30); 
	/*Clear the bit for that last reading that was started before you turned off the ADC*/
	ADC_SAR_SAR_INTR_REG = ADC_SAR_EOS_MASK; 

	for(loop = 0; loop < number; loop++)
    {    
        ADC_SAR_StartConvert();
        ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT);
        Offset = Offset +  ADC_SAR_GetResult16(0); 
		ADC_SAR_StopConvert();
    } 
	
	return (Offset);
}

/*************************************************************************************** 
* Function Name: initializeDiodeMeasurement
****************************************************************************************
*
* Summary:
*  Initializes the components required for temperature measurement and also
*  and finds the system offset  
*
* Parameters:  
*  None
*
* Return: 
*  None
*
*
******************************************************************************************/
void  initializeDiodeMeasurement()
{
    /* Start the components */
    ADC_SAR_Start();
    IDAC_Start();
	Opamp_1_Start();
	Opamp_2_Start();
	AMux_High_Start();
    AMux_Low_Start(); 
	
	/* Initial IDAC Calibration */
    ADCOffset = getSystemOffset(NUM_OF_SAMPLES_CALIB);
   __SPI_SET_STATUS_BIT(STATUS_CALIBRATION_DONE);
    
}


/******************************************************************************************* 
* Function Name: convertVoltageToTemperature
********************************************************************************************
*
* Summary:
*  Converts the voltage measured with the ADC to a corresponding temperature with a lookuptable
*
* Parameters:  
*  float - voltage 
*
* Return: 
*  float - diode temperature
*
*********************************************************************************************/
float convertVoltageToTemperature(float32 voltage)
{
  
    /*store the integer part of the diode temperature*/
	uint16 intr; 
    uint8 intrHigh;
    uint8 intrLow;
	/*store the fractional part of the diode temperature*/
	uint16 frac; 
    uint8 fracHigh;
    uint8 fracLow;
    
    intr = (int16)voltage;
	frac = (int16)(voltage*1000)-intr*1000;
    
    intrHigh = ((intr & (0xFF << 8)) >> 8);
    intrLow = (intr & 0xFF);
    fracHigh = ((frac & (0xFF << 8)) >> 8);
    fracLow = (frac & 0xFF);
    
    setSPITempReg(intrHigh, intrLow, fracHigh, fracLow);
    
    return voltage; //No Conversion so far
}




/******************************************************************************************* 
* Function Name: getDiodeTemperature
********************************************************************************************
*
* Summary:
*  Returns the temperature of the transistor diode and the Vbe voltage measured
*
* Parameters:  
*  None
*
* Return: 
*  float - diode temperature
*
*********************************************************************************************/
float getDiodeTemperature(uint8 chan)
{
    uint8 loop;
    int32 Vf_SUM = 0;
    float32 Vf_Single;
    float32 diode_temperature;

  
    /*Set the ADC multiplexer to measure Voltage over Body Diode*/
    AMux_High_Select (AMUX_HIGH_DIODE);
    AMux_Low_Select (AMUX_LOW_DIODE);
    
    /*Set the current multiplexer to measure Mosfet 1 of DUT*/
	setMultiplexerChan(chan);
    turnOnSink();
    __SPI_SET_STATUS_BIT(STATUS_CURRENT_SOURCE_ON);
	/*add some delay to makes sure the diode voltage is stable*/
	CyDelayUs(100); 
	/*Clear the bit for that last reading that was started before you turned off the ADC*/
	ADC_SAR_SAR_INTR_REG = ADC_SAR_EOS_MASK; 

	/* Measure delta Vbe at I2. Software loop to reduce noise in measurement */
    for(loop = 0; loop < NUM_OF_SAMPLES_VF; loop++)
    {
        ADC_SAR_StartConvert();
        ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT); 
        Vf_SUM = Vf_SUM +  ADC_SAR_GetResult16(0);
		ADC_SAR_StopConvert();
	}
	/*Get the delta Vbe after offset calibration*/
	Vf_SUM = Vf_SUM  - ADCOffset;

	turnOffSink();
    __SPI_CLEAR_STATUS_BIT(STATUS_CURRENT_SOURCE_ON);
    
	/* Take the different in Vbe and average it per the software loop cycles above */
    Vf_Single = ((float32)(Vf_SUM)) / ((float32)(NUM_OF_SAMPLES_VF));
	
	/* Temperature in degree Centigrade */
	diode_temperature = convertVoltageToTemperature(Vf_Single);
    __SPI_SET_STATUS_BIT(STATUS_CONVERSION_DONE);
	return(diode_temperature);
}

/* [] END OF FILE */
