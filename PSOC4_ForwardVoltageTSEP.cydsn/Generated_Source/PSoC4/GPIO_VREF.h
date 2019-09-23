/*******************************************************************************
* File Name: GPIO_VREF.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_GPIO_VREF_H) /* Pins GPIO_VREF_H */
#define CY_PINS_GPIO_VREF_H

#include "cytypes.h"
#include "cyfitter.h"
#include "GPIO_VREF_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} GPIO_VREF_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   GPIO_VREF_Read(void);
void    GPIO_VREF_Write(uint8 value);
uint8   GPIO_VREF_ReadDataReg(void);
#if defined(GPIO_VREF__PC) || (CY_PSOC4_4200L) 
    void    GPIO_VREF_SetDriveMode(uint8 mode);
#endif
void    GPIO_VREF_SetInterruptMode(uint16 position, uint16 mode);
uint8   GPIO_VREF_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void GPIO_VREF_Sleep(void); 
void GPIO_VREF_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(GPIO_VREF__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define GPIO_VREF_DRIVE_MODE_BITS        (3)
    #define GPIO_VREF_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - GPIO_VREF_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the GPIO_VREF_SetDriveMode() function.
         *  @{
         */
        #define GPIO_VREF_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define GPIO_VREF_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define GPIO_VREF_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define GPIO_VREF_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define GPIO_VREF_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define GPIO_VREF_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define GPIO_VREF_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define GPIO_VREF_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define GPIO_VREF_MASK               GPIO_VREF__MASK
#define GPIO_VREF_SHIFT              GPIO_VREF__SHIFT
#define GPIO_VREF_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in GPIO_VREF_SetInterruptMode() function.
     *  @{
     */
        #define GPIO_VREF_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define GPIO_VREF_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define GPIO_VREF_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define GPIO_VREF_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(GPIO_VREF__SIO)
    #define GPIO_VREF_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(GPIO_VREF__PC) && (CY_PSOC4_4200L)
    #define GPIO_VREF_USBIO_ENABLE               ((uint32)0x80000000u)
    #define GPIO_VREF_USBIO_DISABLE              ((uint32)(~GPIO_VREF_USBIO_ENABLE))
    #define GPIO_VREF_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define GPIO_VREF_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define GPIO_VREF_USBIO_ENTER_SLEEP          ((uint32)((1u << GPIO_VREF_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << GPIO_VREF_USBIO_SUSPEND_DEL_SHIFT)))
    #define GPIO_VREF_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << GPIO_VREF_USBIO_SUSPEND_SHIFT)))
    #define GPIO_VREF_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << GPIO_VREF_USBIO_SUSPEND_DEL_SHIFT)))
    #define GPIO_VREF_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(GPIO_VREF__PC)
    /* Port Configuration */
    #define GPIO_VREF_PC                 (* (reg32 *) GPIO_VREF__PC)
#endif
/* Pin State */
#define GPIO_VREF_PS                     (* (reg32 *) GPIO_VREF__PS)
/* Data Register */
#define GPIO_VREF_DR                     (* (reg32 *) GPIO_VREF__DR)
/* Input Buffer Disable Override */
#define GPIO_VREF_INP_DIS                (* (reg32 *) GPIO_VREF__PC2)

/* Interrupt configuration Registers */
#define GPIO_VREF_INTCFG                 (* (reg32 *) GPIO_VREF__INTCFG)
#define GPIO_VREF_INTSTAT                (* (reg32 *) GPIO_VREF__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define GPIO_VREF_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(GPIO_VREF__SIO)
    #define GPIO_VREF_SIO_REG            (* (reg32 *) GPIO_VREF__SIO)
#endif /* (GPIO_VREF__SIO_CFG) */

/* USBIO registers */
#if !defined(GPIO_VREF__PC) && (CY_PSOC4_4200L)
    #define GPIO_VREF_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define GPIO_VREF_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define GPIO_VREF_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define GPIO_VREF_DRIVE_MODE_SHIFT       (0x00u)
#define GPIO_VREF_DRIVE_MODE_MASK        (0x07u << GPIO_VREF_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins GPIO_VREF_H */


/* [] END OF FILE */
