/*******************************************************************************
* File Name: AMux_High.h
* Version 1.80
*
*  Description:
*    This file contains the constants and function prototypes for the Analog
*    Multiplexer User Module AMux.
*
*   Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_AMUX_AMux_High_H)
#define CY_AMUX_AMux_High_H

#include "cyfitter.h"
#include "cyfitter_cfg.h"

#if ((CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) || \
         (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC4) || \
         (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5))    
    #include "cytypes.h"
#else
    #include "syslib/cy_syslib.h"
#endif /* ((CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) */


/***************************************
*        Function Prototypes
***************************************/

void AMux_High_Start(void) ;
#define AMux_High_Init() AMux_High_Start()
void AMux_High_FastSelect(uint8 channel) ;
/* The Stop, Select, Connect, Disconnect and DisconnectAll functions are declared elsewhere */
/* void AMux_High_Stop(void); */
/* void AMux_High_Select(uint8 channel); */
/* void AMux_High_Connect(uint8 channel); */
/* void AMux_High_Disconnect(uint8 channel); */
/* void AMux_High_DisconnectAll(void) */


/***************************************
*         Parameter Constants
***************************************/

#define AMux_High_CHANNELS  2u
#define AMux_High_MUXTYPE   1
#define AMux_High_ATMOSTONE 0

/***************************************
*             API Constants
***************************************/

#define AMux_High_NULL_CHANNEL 0xFFu
#define AMux_High_MUX_SINGLE   1
#define AMux_High_MUX_DIFF     2


/***************************************
*        Conditional Functions
***************************************/

#if AMux_High_MUXTYPE == AMux_High_MUX_SINGLE
# if !AMux_High_ATMOSTONE
#  define AMux_High_Connect(channel) AMux_High_Set(channel)
# endif
# define AMux_High_Disconnect(channel) AMux_High_Unset(channel)
#else
# if !AMux_High_ATMOSTONE
void AMux_High_Connect(uint8 channel) ;
# endif
void AMux_High_Disconnect(uint8 channel) ;
#endif

#if AMux_High_ATMOSTONE
# define AMux_High_Stop() AMux_High_DisconnectAll()
# define AMux_High_Select(channel) AMux_High_FastSelect(channel)
void AMux_High_DisconnectAll(void) ;
#else
# define AMux_High_Stop() AMux_High_Start()
void AMux_High_Select(uint8 channel) ;
# define AMux_High_DisconnectAll() AMux_High_Start()
#endif

#endif /* CY_AMUX_AMux_High_H */


/* [] END OF FILE */
