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
#include "InterruptRoutines.h"
#include "stdio.h"
#include "project.h"

CY_ISR(Custom_ISR_timer)  //timer overflow handler (useful for RX timeouts)
{
    Timer_ReadStatusRegister();//clears interrupt flag
    avarage_flag=1;
}
/* [] END OF FILE */
