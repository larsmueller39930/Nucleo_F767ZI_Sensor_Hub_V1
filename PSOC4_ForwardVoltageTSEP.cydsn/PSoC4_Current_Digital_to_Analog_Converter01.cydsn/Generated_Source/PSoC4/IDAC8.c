/*******************************************************************************
* File Name: IDAC8.c
* Version 1.10
*
* Description:
*  This file provides the source code of APIs for the IDAC_P4 component.
*
*******************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "IDAC8.h"

uint32 IDAC8_initVar = 0u;


/*******************************************************************************
* Function Name: IDAC8_Init
********************************************************************************
*
* Summary:
*  Initializes IDAC registers with initial values provided from customizer.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  None
*
*******************************************************************************/
void IDAC8_Init(void)
{
    uint32 regVal;
    uint8 enableInterrupts;

    /* Set initial configuration */
    enableInterrupts = CyEnterCriticalSection();
    
    #if(IDAC8_MODE_SOURCE == IDAC8_IDAC_POLARITY)
        regVal  = IDAC8_CSD_TRIM1_REG & ~(IDAC8_CSD_IDAC_TRIM1_MASK);
        regVal |=  (IDAC8_SFLASH_TRIM1_REG & IDAC8_CSD_IDAC_TRIM1_MASK);
        IDAC8_CSD_TRIM1_REG = regVal;
    #else
        regVal  = IDAC8_CSD_TRIM2_REG & ~(IDAC8_CSD_IDAC_TRIM2_MASK);
        regVal |=  (IDAC8_SFLASH_TRIM2_REG & IDAC8_CSD_IDAC_TRIM2_MASK);
        IDAC8_CSD_TRIM2_REG = regVal;
    #endif /* (IDAC8_MODE_SOURCE == IDAC8_IDAC_POLARITY) */

    /* clear previous values */
    IDAC8_IDAC_CONTROL_REG &= ((uint32)~((uint32)IDAC8_IDAC_VALUE_MASK <<
        IDAC8_IDAC_VALUE_POSITION)) | ((uint32)~((uint32)IDAC8_IDAC_MODE_MASK <<
        IDAC8_IDAC_MODE_POSITION))  | ((uint32)~((uint32)IDAC8_IDAC_RANGE_MASK  <<
        IDAC8_IDAC_RANGE_POSITION));

    IDAC8_IDAC_POLARITY_CONTROL_REG &= (~(uint32)((uint32)IDAC8_IDAC_POLARITY_MASK <<
        IDAC8_IDAC_POLARITY_POSITION));

    /* set new configuration */
    IDAC8_IDAC_CONTROL_REG |= (((uint32)IDAC8_IDAC_INIT_VALUE <<
        IDAC8_IDAC_VALUE_POSITION) | ((uint32)IDAC8_IDAC_RANGE <<
        IDAC8_IDAC_RANGE_POSITION));

    IDAC8_IDAC_POLARITY_CONTROL_REG |= ((uint32)IDAC8_IDAC_POLARITY <<
                                                           IDAC8_IDAC_POLARITY_POSITION);

    CyExitCriticalSection(enableInterrupts);

}


/*******************************************************************************
* Function Name: IDAC8_Enable
********************************************************************************
*
* Summary:
*  Enables IDAC operations.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  None
*
*******************************************************************************/
void IDAC8_Enable(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    /* Enable the IDAC */
    IDAC8_IDAC_CONTROL_REG |= ((uint32)IDAC8_IDAC_EN_MODE <<
                                                  IDAC8_IDAC_MODE_POSITION);
    IDAC8_IDAC_POLARITY_CONTROL_REG |= ((uint32)IDAC8_IDAC_CSD_EN <<
                                                           IDAC8_IDAC_CSD_EN_POSITION);
    CyExitCriticalSection(enableInterrupts);

}


/*******************************************************************************
* Function Name: IDAC8_Start
********************************************************************************
*
* Summary:
*  Starts the IDAC hardware.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  IDAC8_initVar
*
*******************************************************************************/
void IDAC8_Start(void)
{
    if(0u == IDAC8_initVar)
    {
        IDAC8_Init();
        IDAC8_initVar = 1u;
    }

    IDAC8_Enable();

}


/*******************************************************************************
* Function Name: IDAC8_Stop
********************************************************************************
*
* Summary:
*  Stops the IDAC hardware.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  None
*
*******************************************************************************/
void IDAC8_Stop(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    /* Disable the IDAC */
    IDAC8_IDAC_CONTROL_REG &= ((uint32)~((uint32)IDAC8_IDAC_MODE_MASK <<
        IDAC8_IDAC_MODE_POSITION));
    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: IDAC8_SetValue
********************************************************************************
*
* Summary:
*  Sets the IDAC value.
*
* Parameters:
*  uint32 value
*
* Return:
*  None
*
* Global variables:
*  None
*
*******************************************************************************/
void IDAC8_SetValue(uint32 value)
{
    uint8 enableInterrupts;
    uint32 newRegisterValue;

    enableInterrupts = CyEnterCriticalSection();

    #if(IDAC8_IDAC_VALUE_POSITION != 0u)
        newRegisterValue = ((IDAC8_IDAC_CONTROL_REG & (~(uint32)((uint32)IDAC8_IDAC_VALUE_MASK <<
        IDAC8_IDAC_VALUE_POSITION))) | (value << IDAC8_IDAC_VALUE_POSITION));
    #else
        newRegisterValue = ((IDAC8_IDAC_CONTROL_REG & (~(uint32)IDAC8_IDAC_VALUE_MASK)) | value);
    #endif /* IDAC8_IDAC_VALUE_POSITION != 0u */

    IDAC8_IDAC_CONTROL_REG = newRegisterValue;

    CyExitCriticalSection(enableInterrupts);
}

/* [] END OF FILE */
