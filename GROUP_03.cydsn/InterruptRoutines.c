/* ========================================
 * GROUP_03 - HW_03
 * Di Liddo Mauro, Goshen Davide
 * InterruptRoutines.c
 *
 * ========================================
*/
#include "InterruptRoutines.h"
#include "stdio.h"
#include "project.h"

CY_ISR(Custom_ISR_timer)  //timer overflow handler (useful for RX timeouts)
{
    Timer_ReadStatusRegister();//clears interrupt flag
    read_flag=1;
}
/* [] END OF FILE */
