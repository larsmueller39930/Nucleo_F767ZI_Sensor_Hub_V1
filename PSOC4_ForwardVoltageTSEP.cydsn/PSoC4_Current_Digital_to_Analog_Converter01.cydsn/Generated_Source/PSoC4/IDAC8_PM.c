/*******************************************************************************
* File Name: IDAC8_PM.c
* Version 1.10
*
* Description:
*  This file provides Low power mode APIs for IDAC_P4 component.
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "IDAC8.h"


static IDAC8_BACKUP_STRUCT IDAC8_backup;


/*******************************************************************************
* Function Name: IDAC8_SaveConfig
********************************************************************************
*
* Summary:
*  Saves component state before sleep
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  None
*
*******************************************************************************/
void IDAC8_SaveConfig(void)
{
    /* All registers are retention - nothing to save */
}


/*******************************************************************************
* Function Name: IDAC8_Sleep
********************************************************************************
*
* Summary:
*  Calls _SaveConfig() function
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void IDAC8_Sleep(void)
{
        if(0u != (IDAC8_IDAC_CONTROL_REG & ((uint32)IDAC8_IDAC_MODE_MASK <<
        IDAC8_IDAC_MODE_POSITION)))
        {
            IDAC8_backup.enableState = 1u;
        }
        else
        {
            IDAC8_backup.enableState = 0u;
        }

    IDAC8_Stop();
    IDAC8_SaveConfig();
}


/*******************************************************************************
* Function Name: IDAC8_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores component state after wakeup
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  None
*
*******************************************************************************/
void IDAC8_RestoreConfig(void)
{
    /* All registers are retention - nothing to save */
}


/*******************************************************************************
* Function Name: IDAC8_Wakeup
********************************************************************************
*
* Summary:
*  Calls _RestoreConfig() function
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void IDAC8_Wakeup(void)
{
    /* Restore IDAC register settings */
    IDAC8_RestoreConfig();
    if(IDAC8_backup.enableState == 1u)
    {
        /* Enable operation */
        IDAC8_Enable();
    } /* Do nothing if the component was disabled before */

}


/* [] END OF FILE */
