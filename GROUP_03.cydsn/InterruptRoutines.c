/* ========================================
 * GROUP_03 - HW_03
 * Di Liddo Mauro, Goshen Davide
 * InterruptRoutines.c
 * ========================================
*/
#include "InterruptRoutines.h"
#include "stdio.h"
#include "project.h"

CY_ISR(Custom_ISR_timer)//timer overflow handler
{
    Timer_ReadStatusRegister();//clears interrupt flag
    read_flag=1;//enable of the reading on adc data
}
/* [] END OF FILE */
