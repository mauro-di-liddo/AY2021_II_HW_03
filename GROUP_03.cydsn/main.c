/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "InterruptRoutines.h"
#include "stdio.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
       ADC_DelSig_Start(); 
       Timer_Start();
       isr_1_StartEx(Custom_ISR_timer);
       ADC_DelSig_StartConvert();
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
