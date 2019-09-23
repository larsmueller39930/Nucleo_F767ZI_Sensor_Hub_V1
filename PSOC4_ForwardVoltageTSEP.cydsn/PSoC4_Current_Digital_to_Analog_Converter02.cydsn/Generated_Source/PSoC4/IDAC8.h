/*******************************************************************************
* File Name: IDAC8.h
* Version 1.10
*
* Description:
*  This file provides constants and parameter values for the IDAC_P4
*  component.
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_IDAC_IDAC8_H)
#define CY_IDAC_IDAC8_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h"


/***************************************
* Internal Type defines
***************************************/

/* Structure to save state before go to sleep */
typedef struct
{
    uint8  enableState;
} IDAC8_BACKUP_STRUCT;


extern uint32 IDAC8_initVar;

/**************************************
*    Enumerated Types and Parameters
**************************************/

/* IDAC polarity setting constants */
#define IDAC8__SOURCE 0
#define IDAC8__SINK 1


/* IDAC range setting constants */
#define IDAC8__LOWRANGE 0
#define IDAC8__HIGHRANGE 1


/* IDAC polarity setting definitions */
#define IDAC8_MODE_SOURCE    (IDAC8__SOURCE)
#define IDAC8_MODE_SINK      (IDAC8__SINK)

/* IDAC RANGE setting definitions */
#define IDAC8_MODE_LOWRANGE  (IDAC8__LOWRANGE)
#define IDAC8_MODE_HIGHRANGE (IDAC8__HIGHRANGE)

/***************************************
*   Conditional Compilation Parameters
****************************************/

#define IDAC8_IDAC_RESOLUTION    (8u)
#define IDAC8_IDAC_RANGE         (1u)
#define IDAC8_IDAC_POLARITY      (0u)


/***************************************
*    Initial Parameter Constants
***************************************/
#define IDAC8_IDAC_INIT_VALUE    (0u)




/***************************************
*        Function Prototypes
***************************************/

void IDAC8_Init(void);
void IDAC8_Enable(void);
void IDAC8_Start(void);
void IDAC8_Stop(void);
void IDAC8_SetValue(uint32  value);
void IDAC8_SaveConfig(void);
void IDAC8_Sleep(void);
void IDAC8_RestoreConfig(void);
void IDAC8_Wakeup(void);


/***************************************
*            API Constants
***************************************/

#define IDAC8_IDAC_EN_MODE           (3u)
#define IDAC8_IDAC_CSD_EN            (1u)
#define IDAC8_IDAC_CSD_EN_POSITION   (31u)

#define IDAC8_IDAC_VALUE_POSITION    (IDAC8_cy_psoc4_idac__CSD_IDAC_SHIFT)

#define IDAC8_IDAC_MODE_SHIFT        (8u)
#define IDAC8_IDAC_MODE_POSITION     ((uint32)IDAC8_cy_psoc4_idac__CSD_IDAC_SHIFT +\
                                                 IDAC8_IDAC_MODE_SHIFT)

#define IDAC8_IDAC_RANGE_SHIFT       (10u)
#define IDAC8_IDAC_RANGE_POSITION    ((uint32)IDAC8_cy_psoc4_idac__CSD_IDAC_SHIFT +\
                                                 IDAC8_IDAC_RANGE_SHIFT)

#define IDAC8_IDAC_POLARITY_POSITION ((uint32)IDAC8_cy_psoc4_idac__POLARITY_SHIFT)

#define IDAC8_IDAC_TRIM1_POSITION    ((uint32)IDAC8_cy_psoc4_idac__CSD_TRIM1_SHIFT)
#define IDAC8_IDAC_TRIM2_POSITION    ((uint32)IDAC8_cy_psoc4_idac__CSD_TRIM2_SHIFT)

#define IDAC8_IDAC_CDS_EN_MASK       (0x80000000u)

#if(IDAC8_IDAC_RESOLUTION == 8u)
  #define IDAC8_IDAC_VALUE_MASK      (0xFFu)
#else
  #define IDAC8_IDAC_VALUE_MASK      (0x7Fu)
#endif /* (IDAC8_IDAC_RESOLUTION == 8u) */

#define IDAC8_IDAC_MODE_MASK         (3u)
#define IDAC8_IDAC_RANGE_MASK        (1u)
#define IDAC8_IDAC_POLARITY_MASK     (1u)

#define IDAC8_CSD_IDAC_TRIM1_MASK    (0x0000000FuL << IDAC8_IDAC_TRIM1_POSITION)
#define IDAC8_CSD_IDAC_TRIM2_MASK    (0x0000000FuL << IDAC8_IDAC_TRIM2_POSITION)


/***************************************
*        Registers
***************************************/

#define IDAC8_IDAC_CONTROL_REG    (*(reg32 *)IDAC8_cy_psoc4_idac__CSD_IDAC)
#define IDAC8_IDAC_CONTROL_PTR    ( (reg32 *)IDAC8_cy_psoc4_idac__CSD_IDAC)

#define IDAC8_IDAC_POLARITY_CONTROL_REG    (*(reg32 *)IDAC8_cy_psoc4_idac__CONTROL)
#define IDAC8_IDAC_POLARITY_CONTROL_PTR    ( (reg32 *)IDAC8_cy_psoc4_idac__CONTROL)

#define IDAC8_CSD_TRIM1_REG       (*(reg32 *)IDAC8_cy_psoc4_idac__CSD_TRIM1)
#define IDAC8_CSD_TRIM1_PTR       ( (reg32 *)IDAC8_cy_psoc4_idac__CSD_TRIM1)

#define IDAC8_CSD_TRIM2_REG       (*(reg32 *)IDAC8_cy_psoc4_idac__CSD_TRIM2)
#define IDAC8_CSD_TRIM2_PTR       ( (reg32 *)IDAC8_cy_psoc4_idac__CSD_TRIM2)

#if (CY_PSOC4_4100M || CY_PSOC4_4200M)
    #if(IDAC8_cy_psoc4_idac__IDAC_NUMBER > 2u)
        #define IDAC8_SFLASH_TRIM1_REG       (*(reg8 *)CYREG_SFLASH_CSD1_TRIM1_HVIDAC)
        #define IDAC8_SFLASH_TRIM1_PTR       ( (reg8 *)CYREG_SFLASH_CSD1_TRIM1_HVIDAC)
        
        #define IDAC8_SFLASH_TRIM2_REG       (*(reg8 *)CYREG_SFLASH_CSD1_TRIM2_HVIDAC)
        #define IDAC8_SFLASH_TRIM2_PTR       ( (reg8 *)CYREG_SFLASH_CSD1_TRIM2_HVIDAC)
    #else
        #define IDAC8_SFLASH_TRIM1_REG       (*(reg8 *)CYREG_SFLASH_CSD_TRIM1_HVIDAC)
        #define IDAC8_SFLASH_TRIM1_PTR       ( (reg8 *)CYREG_SFLASH_CSD_TRIM1_HVIDAC)
        
        #define IDAC8_SFLASH_TRIM2_REG       (*(reg8 *)CYREG_SFLASH_CSD_TRIM2_HVIDAC)
        #define IDAC8_SFLASH_TRIM2_PTR       ( (reg8 *)CYREG_SFLASH_CSD_TRIM2_HVIDAC)
    #endif /* (IDAC8_cy_psoc4_idac__IDAC_NUMBER > 2u) */
#else
    #define IDAC8_SFLASH_TRIM1_REG       (*(reg8 *)CYREG_SFLASH_CSD_TRIM1_HVIDAC)
    #define IDAC8_SFLASH_TRIM1_PTR       ( (reg8 *)CYREG_SFLASH_CSD_TRIM1_HVIDAC)
    
    #define IDAC8_SFLASH_TRIM2_REG       (*(reg8 *)CYREG_SFLASH_CSD_TRIM2_HVIDAC)
    #define IDAC8_SFLASH_TRIM2_PTR       ( (reg8 *)CYREG_SFLASH_CSD_TRIM2_HVIDAC)
#endif /* (CY_PSOC4_4100M || CY_PSOC4_4200M) */

#endif /* CY_IDAC_IDAC8_H */

/* [] END OF FILE */