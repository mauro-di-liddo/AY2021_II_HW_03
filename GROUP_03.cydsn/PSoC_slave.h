/* ========================================
 * GROUP_03 - HW_03
 * Di Liddo Mauro, Goshen Davide
 * PSoC_slave.h
 * ========================================
*/

#ifndef __PSoC_H
    #define __PSoC_H

    #define PSoC_DEVICE_ADDRESS 0x08
    #define PSoC_WHO_AM_I_REG_ADDR 0x02
    #define PSoC_REG1 0b00000000 //as default 0, it will be configured through BCP
    #define PSoC_REG2 0b00000000 //as default 0, it will be configured through BCP
    
    //mask used to properly save data and set the function modality
    #define OFF_MODE 0b00000000
    #define PHT_MODE 0b00000010
    #define TEMP_MODE 0b00000001
    #define CONTEMP_MODE 0b00000011
    
    //defalut values of samples and period
    #define DEFAULT_SAMPLE 0
    #define DEFAULT_PERIOD 0
    
#endif

/* [] END OF FILE */
