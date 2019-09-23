/*******************************************************************************
* File Name: GPIO_VREF.c  
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
#include "GPIO_VREF.h"

static GPIO_VREF_BACKUP_STRUCT  GPIO_VREF_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: GPIO_VREF_Sleep
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
*  \snippet GPIO_VREF_SUT.c usage_GPIO_VREF_Sleep_Wakeup
*******************************************************************************/
void GPIO_VREF_Sleep(void)
{
    #if defined(GPIO_VREF__PC)
        GPIO_VREF_backup.pcState = GPIO_VREF_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            GPIO_VREF_backup.usbState = GPIO_VREF_CR1_REG;
            GPIO_VREF_USB_POWER_REG |= GPIO_VREF_USBIO_ENTER_SLEEP;
            GPIO_VREF_CR1_REG &= GPIO_VREF_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(GPIO_VREF__SIO)
        GPIO_VREF_backup.sioState = GPIO_VREF_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        GPIO_VREF_SIO_REG &= (uint32)(~GPIO_VREF_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: GPIO_VREF_Wakeup
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
*  Refer to GPIO_VREF_Sleep() for an example usage.
*******************************************************************************/
void GPIO_VREF_Wakeup(void)
{
    #if defined(GPIO_VREF__PC)
        GPIO_VREF_PC = GPIO_VREF_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            GPIO_VREF_USB_POWER_REG &= GPIO_VREF_USBIO_EXIT_SLEEP_PH1;
            GPIO_VREF_CR1_REG = GPIO_VREF_backup.usbState;
            GPIO_VREF_USB_POWER_REG &= GPIO_VREF_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(GPIO_VREF__SIO)
        GPIO_VREF_SIO_REG = GPIO_VREF_backup.sioState;
    #endif
}


/* [] END OF FILE */
