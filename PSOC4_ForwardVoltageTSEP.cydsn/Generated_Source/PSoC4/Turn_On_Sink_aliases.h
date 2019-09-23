/*******************************************************************************
* File Name: Turn_On_Sink.h  
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

#if !defined(CY_PINS_Turn_On_Sink_ALIASES_H) /* Pins Turn_On_Sink_ALIASES_H */
#define CY_PINS_Turn_On_Sink_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Turn_On_Sink_0			(Turn_On_Sink__0__PC)
#define Turn_On_Sink_0_PS		(Turn_On_Sink__0__PS)
#define Turn_On_Sink_0_PC		(Turn_On_Sink__0__PC)
#define Turn_On_Sink_0_DR		(Turn_On_Sink__0__DR)
#define Turn_On_Sink_0_SHIFT	(Turn_On_Sink__0__SHIFT)
#define Turn_On_Sink_0_INTR	((uint16)((uint16)0x0003u << (Turn_On_Sink__0__SHIFT*2u)))

#define Turn_On_Sink_INTR_ALL	 ((uint16)(Turn_On_Sink_0_INTR))


#endif /* End Pins Turn_On_Sink_ALIASES_H */


/* [] END OF FILE */
