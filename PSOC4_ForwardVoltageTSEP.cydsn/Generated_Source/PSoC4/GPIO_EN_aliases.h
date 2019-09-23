/*******************************************************************************
* File Name: GPIO_EN.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_GPIO_EN_ALIASES_H) /* Pins GPIO_EN_ALIASES_H */
#define CY_PINS_GPIO_EN_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define GPIO_EN_0			(GPIO_EN__0__PC)
#define GPIO_EN_0_PS		(GPIO_EN__0__PS)
#define GPIO_EN_0_PC		(GPIO_EN__0__PC)
#define GPIO_EN_0_DR		(GPIO_EN__0__DR)
#define GPIO_EN_0_SHIFT	(GPIO_EN__0__SHIFT)
#define GPIO_EN_0_INTR	((uint16)((uint16)0x0003u << (GPIO_EN__0__SHIFT*2u)))

#define GPIO_EN_INTR_ALL	 ((uint16)(GPIO_EN_0_INTR))


#endif /* End Pins GPIO_EN_ALIASES_H */


/* [] END OF FILE */
