/*******************************************************************************
* File Name: Turn_On_Sink.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "Turn_On_Sink.h"

static Turn_On_Sink_BACKUP_STRUCT  Turn_On_Sink_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: Turn_On_Sink_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function applies only to SIO and USBIO pins.
*  It should not be called for GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet Turn_On_Sink_SUT.c usage_Turn_On_Sink_Sleep_Wakeup
*******************************************************************************/
void Turn_On_Sink_Sleep(void)
{
    #if defined(Turn_On_Sink__PC)
        Turn_On_Sink_backup.pcState = Turn_On_Sink_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            Turn_On_Sink_backup.usbState = Turn_On_Sink_CR1_REG;
            Turn_On_Sink_USB_POWER_REG |= Turn_On_Sink_USBIO_ENTER_SLEEP;
            Turn_On_Sink_CR1_REG &= Turn_On_Sink_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Turn_On_Sink__SIO)
        Turn_On_Sink_backup.sioState = Turn_On_Sink_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        Turn_On_Sink_SIO_REG &= (uint32)(~Turn_On_Sink_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: Turn_On_Sink_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep(). This 
* function applies only to SIO and USBIO pins. It should not be called for
* GPIO or GPIO_OVT pins.
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to Turn_On_Sink_Sleep() for an example usage.
*******************************************************************************/
void Turn_On_Sink_Wakeup(void)
{
    #if defined(Turn_On_Sink__PC)
        Turn_On_Sink_PC = Turn_On_Sink_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            Turn_On_Sink_USB_POWER_REG &= Turn_On_Sink_USBIO_EXIT_SLEEP_PH1;
            Turn_On_Sink_CR1_REG = Turn_On_Sink_backup.usbState;
            Turn_On_Sink_USB_POWER_REG &= Turn_On_Sink_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Turn_On_Sink__SIO)
        Turn_On_Sink_SIO_REG = Turn_On_Sink_backup.sioState;
    #endif
}


/* [] END OF FILE */