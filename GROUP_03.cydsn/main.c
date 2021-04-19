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
#include "PSoC_slave.h"

//inizializza "stato" come variabile globale


#define SLAVE_BUFFER_SIZE 7

volatile uint8_t slaveBuffer[SLAVE_BUFFER_SIZE];
uint8_t Control_Reg_1;
uint8_t Control_Reg_2;
int32 value_digit;
int32 sum_value_photo;
int32 sum_value_temp;
int32 value_final_photo;
int32 value_final_temp;
int count=0;
#define CHANNEL_TEMP 0
#define CHANNEL_PHOTO 1
#define ON 1
#define OFF 0
char message[20] = {'\0'};

uint8_t stato=0;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
        ADC_DelSig_Start(); 
        //UART_1_Start();
        Timer_Start();
        AMux_Start();
        isr_ADC_StartEx(Custom_ISR_timer);
        EZI2C_Start();
        ADC_DelSig_StartConvert();
    
        slaveBuffer[0] = Control_Reg_1;
        slaveBuffer[1] = Control_Reg_2;
        // Set up who am i register
        slaveBuffer[SLAVE_BUFFER_SIZE-5] = 0xBC;
        read_flag=0;
        sum_value_photo=0;
        sum_value_temp=0;
        value_final_photo=0;
        value_final_temp=0;
       
    for(;;)
    {
        switch(stato){
        case CONTEMP_MODE:
            if (read_flag==1){
                LEDPin_Write(OFF);
                AMux_Select(CHANNEL_PHOTO);
                value_digit=ADC_DelSig_Read32(); //ricorda che per 16 bit single sample dobbiamo usare la funzione read32, ed è per questo che abbiamo inizializzato le variabili a 32 bit
                if (value_digit < 0) value_digit=0;
                if (value_digit > 65535) value_digit=65535;
                sum_value_photo=sum_value_photo+value_digit;
                    
                AMux_Select(CHANNEL_TEMP);
                value_digit=ADC_DelSig_Read32(); //ricorda che per 16 bit single sample dobbiamo usare la funzione read32, ed è per questo che abbiamo inizializzato le variabili a 32 bit
                if (value_digit < 0) value_digit=0;
                if (value_digit > 65535) value_digit=65535;
                sum_value_temp=sum_value_temp+value_digit;
                read_flag=0;
                count++;
                if(count == 5){
                    count=0;
                    value_final_photo=sum_value_photo/5;
                    value_final_temp=sum_value_temp/5;
                    sum_value_photo=0;
                    sum_value_temp=0;
                    //sprintf(message, "photo: %ld\r\n", value_final_photo);
                    //UART_1_PutString(message);
                    //sprintf(message, "temp: %ld\r\n", value_final_temp);
                    //UART_1_PutString(message);
                }
                slaveBuffer[3]=value_final_temp >> 8; //put in MSB
                slaveBuffer[4]=value_final_temp & 0xFF; //remain in LSB
                    
                slaveBuffer[5]=value_final_photo >> 8; //put in MSB
                slaveBuffer[6]=value_final_photo & 0xFF; //remain in LSB
                LEDPin_Write(ON);
            }
            break;
            case TEMP_MODE:
                if (read_flag==1){
                    LEDPin_Write(OFF);
                    AMux_Select(CHANNEL_TEMP);
                    value_digit=ADC_DelSig_Read32(); //ricorda che per 16 bit single sample dobbiamo usare la funzione read32, ed è per questo che abbiamo inizializzato le variabili a 32 bit
                    if (value_digit < 0) value_digit=0;
                    if (value_digit > 65535) value_digit=65535;
                    sum_value_temp=sum_value_temp+value_digit;
                    read_flag=0;
                    count++;
                if(count == 5){
                    count=0;
                    value_final_temp=sum_value_temp/5;
                    sum_value_temp=0;
                    //sprintf(message, "photo: %ld\r\n", value_final_photo);
                    //UART_1_PutString(message);
                    //sprintf(message, "temp: %ld\r\n", value_final_temp);
                    //UART_1_PutString(message);
                }
                slaveBuffer[3]=value_final_temp >> 8; //put in MSB
                slaveBuffer[4]=value_final_temp & 0xFF; //remain in LSB
            
                }
                break;
            case PHT_MODE:
                if (read_flag==1){
                    LEDPin_Write(OFF);
                    AMux_Select(CHANNEL_PHOTO);
                    value_digit=ADC_DelSig_Read32(); //ricorda che per 16 bit single sample dobbiamo usare la funzione read32, ed è per questo che abbiamo inizializzato le variabili a 32 bit
                    if (value_digit < 0) value_digit=0;
                    if (value_digit > 65535) value_digit=65535;
                    sum_value_photo=sum_value_photo+value_digit;
                    read_flag=0;
                    count++;
                    if(count == 5){
                        count=0;
                        value_final_photo=sum_value_photo/5;
                        sum_value_photo=0;
                        //sprintf(message, "photo: %ld\r\n", value_final_photo);
                        //UART_1_PutString(message);
                        //sprintf(message, "temp: %ld\r\n", value_final_temp);
                        //UART_1_PutString(message);
                    }
                slaveBuffer[5]=value_final_photo >> 8; //put in MSB
                slaveBuffer[6]=value_final_photo & 0xFF; //remain in LSB
                }
                break;
            case OFF_MODE:
                LEDPin_Write(OFF);
                break;
        }
    }
}

/* [] END OF FILE */
