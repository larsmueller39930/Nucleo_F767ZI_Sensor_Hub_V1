/*******************************************************************************
* File Name: AMux_Low.h
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

#if !defined(CY_AMUX_AMux_Low_H)
#define CY_AMUX_AMux_Low_H

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

void AMux_Low_Start(void) ;
#define AMux_Low_Init() AMux_Low_Start()
void AMux_Low_FastSelect(uint8 channel) ;
/* The Stop, Select, Connect, Disconnect and DisconnectAll functions are declared elsewhere */
/* void AMux_Low_Stop(void); */
/* void AMux_Low_Select(uint8 channel); */
/* void AMux_Low_Connect(uint8 channel); */
/* void AMux_Low_Disconnect(uint8 channel); */
/* void AMux_Low_DisconnectAll(void) */


/***************************************
*         Parameter Constants
***************************************/

#define AMux_Low_CHANNELS  2u
#define AMux_Low_MUXTYPE   1
#define AMux_Low_ATMOSTONE 0

/***************************************
*             API Constants
***************************************/

#define AMux_Low_NULL_CHANNEL 0xFFu
#define AMux_Low_MUX_SINGLE   1
#define AMux_Low_MUX_DIFF     2


/***************************************
*        Conditional Functions
***************************************/

#if AMux_Low_MUXTYPE == AMux_Low_MUX_SINGLE
# if !AMux_Low_ATMOSTONE
#  define AMux_Low_Connect(channel) AMux_Low_Set(channel)
# endif
# define AMux_Low_Disconnect(channel) AMux_Low_Unset(channel)
#else
# if !AMux_Low_ATMOSTONE
void AMux_Low_Connect(uint8 channel) ;
# endif
void AMux_Low_Disconnect(uint8 channel) ;
#endif

#if AMux_Low_ATMOSTONE
# define AMux_Low_Stop() AMux_Low_DisconnectAll()
# define AMux_Low_Select(channel) AMux_Low_FastSelect(channel)
void AMux_Low_DisconnectAll(void) ;
#else
# define AMux_Low_Stop() AMux_Low_Start()
void AMux_Low_Select(uint8 channel) ;
# define AMux_Low_DisconnectAll() AMux_Low_Start()
#endif

#endif /* CY_AMUX_AMux_Low_H */


/* [] END OF FILE */