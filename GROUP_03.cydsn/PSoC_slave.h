#ifndef __PSoC_H
    #define __PSoC_H

    #define PSoC_DEVICE_ADDRESS 0x08
    #define PSoC_WHO_AM_I_REG_ADDR 0x02
    #define PSoC_REG1 0b00111100 //da capire cosa mettere di default 
    #define PSoC_REG2 0b00000000 //da capire cosa mettere di default

    #define OFF_MODE 0b00000000
    #define PHT_MODE 0b00000010
    #define TEMP_MODE 0b00000001
    #define CONTEMP_MODE 0b00000011
    
    #define DEFAULT_SAMPLE 5
    #define DEFAULT_PERIOD 20
    
#endif

/* [] END OF FILE */
