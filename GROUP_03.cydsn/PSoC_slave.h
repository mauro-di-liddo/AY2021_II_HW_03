#ifndef __PSoC_H
    #define __PSoC_H

    #define PSoC_DEVICE_ADDRESS 0x08
    #define PSoC_WHO_AM_I_REG_ADDR 0x02
    #define PSoC_ADD_REG1 0b00111100 //da capire cosa mettere di default 
    #define PSoC_ADD_REG2 0b00000000 //da capire cosa mettere di default

    #define OFF_MODE 0b00
    #define PHT_MODE 0b0
    #define TEMP_MODE 0b01
    #define CONTEMP_MODE 0b11
    
    #define DEFAULT_SAMPLE 5
    
#endif

/* [] END OF FILE */
