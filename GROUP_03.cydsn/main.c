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

#define SLAVE_BUFFER_SIZE 7

uint8_t slaveBuffer[SLAVE_BUFFER_SIZE];
uint8_t Control_Reg_1;
uint8_t Control_Reg_2;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
        ADC_DelSig_Start(); 
        Timer_Start();
        isr_1_StartEx(Custom_ISR_timer);
        EZI2C_1_Start();
        ADC_DelSig_StartConvert();
    
        slaveBuffer[0] = Control_Reg_1;
        slaveBuffer[1] = Control_Reg_2;
        // Set up who am i register
        slaveBuffer[SLAVE_BUFFER_SIZE-5] = 0xBC;
    
       
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
