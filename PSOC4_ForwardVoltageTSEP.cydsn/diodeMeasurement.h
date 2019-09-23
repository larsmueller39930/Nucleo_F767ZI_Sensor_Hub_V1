/*******************************************************************************
* File Name: diodeMeasurement.h
* Version 1.0
*
* Description:
*  This header file contains the definitions and function prototypes for single 
*  diode temperature measurement.
*
* Note: 
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/
#if !defined(DIODEMEASUREMENT_H)
#define DIODEMEASUREMENT_H
    
#include <project.h>
#include "currentSink.h"

/********************************************************************************
*   Macro definitions
********************************************************************************/
/* Temperature in Kelvin for 0 degree celsius. Used for Kelvin to Degree Celcius conversion of temperature */
#define ZERO_DEGREE_CELSIUS_KELVIN      ((float)273.15)

/* This definition is for the ADC resolution in volts, required for the diode temperature calculation.*/
#define ADC_VOLTAGE_RESOLUTION ((float)0.0005)

/***************************** USER ATTENTION REQUIRED ***********************************************/

/* Channel number definitions of the AMux_HIGH. These are based on TopDesign Schematic connections */
#define AMUX_HIGH_DIODE				  1
#define AMUX_HIGH_OFFSET              0

/* Channel number definitions of the AMux_LOW. These are based on TopDesign Schematic connections */
#define AMUX_LOW_DIODE				  1
#define AMUX_LOW_OFFSET               0

/******************************************************************************************/
/* Number of ADC samples required for IDAC calibration, diode voltage measurements */

#define NUM_OF_SAMPLES_CALIB            64  
#define NUM_OF_SAMPLES_VF               64 

/* Temperataure change that requires a IDAC ratio recalibration is +/- 10  */
#define CALIB_TEMP_INTERVAL_POS 10
#define CALIB_TEMP_INTERVAL_NEG -10

/********************************************************************************
*   Function Prototypes 
********************************************************************************/
int32 getSystemOffset (int8 number);
void  initializeDiodeMeasurement(void);
float32 getDiodeTemperature(uint8 chan);
float convertVoltageToTemperature(float voltage);

#endif /* DIODEMEASUREMENT_H */		


//[] END OF FILE
