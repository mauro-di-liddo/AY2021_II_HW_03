#ifndef __PSoC_H
    #define __PSoC_H

    #define PSoC_DEVICE_ADDRESS 0x08
    #define PSoC_WHO_AM_I_REG_ADDR 0x02
    #define PSoC_REG1 0b00010111 //di default metto 5 sample(i 4 bit centrali codificano 5 in binario), e il campionamento di entrambi i sensori (i due bit finali entrambi a 1) 
    #define PSoC_REG2 0b00010100 //di default metto 20 in binario per creare un isr di 4 ms

    #define OFF_MODE 0b00000000
    #define PHT_MODE 0b00000010
    #define TEMP_MODE 0b00000001
    #define CONTEMP_MODE 0b00000011
    
    #define DEFAULT_SAMPLE 5
    #define DEFAULT_PERIOD 20
    
#endif

/* [] END OF FILE */
