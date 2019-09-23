/*******************************************************************************
* File: main.c
*
* Version: 2.00
*
* Description:
*  This is the source code for example project that demonstartes basic
*  functionality of PSoC4 IDAC8.
*
********************************************************************************
* Copyright 2013-2018, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#include <project.h>

#define SWITCH_PRESSED 0u       /* When switch is pressed, it is shorted to Vss */
#define LOOP_DELAY     25uL     /* Loop speed is 35 ms */

/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  Main function performs following functions:
*   1: Start the IDAC and set initial value to 0
*   2: Loop forever until switch (SW2) is pressed.
*   3: Increment IDAC for as long as switch is pressed.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
int main()
{
    uint32 iDacValue = 0;         /* IDAC value and initial value */
 
    IDAC8_Start();                /* Start the IDAC component */

    IDAC8_SetValue(iDacValue);    /* Set IDAC output to 0 uA  */ 
    
    for(;;)
    {
        CyDelay(LOOP_DELAY);                /* Set Loop Delay to slow down iDAC ramp */
        if(SW2_Read() == SWITCH_PRESSED )   /* If button pressed, increase output */
        {
            iDacValue++;
            if( iDacValue > 255 )           /* If max value, reset to 0 */
            {
                iDacValue = 0;
            }
            IDAC8_SetValue(iDacValue);      /* Update the IDAC output  */
        }
    }
}

/* [] END OF FILE */
