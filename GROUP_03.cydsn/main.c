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
#include "math.h"

//inizializza "stato" come variabile globale


#define SLAVE_BUFFER_SIZE 7
#define TRANSMIT_BUFFER_SIZE 6

volatile uint8_t slaveBuffer[SLAVE_BUFFER_SIZE];
uint8_t dataBuffer[TRANSMIT_BUFFER_SIZE];
#define Control_Reg_1 0
#define Control_Reg_2 1
int32 value_digit;
int32 sum_value_photo;
int32 sum_value_temp;
int32 value_final_photo;
int32 value_final_temp;
uint16_t count=0;
uint16_t i=0;
#define CHANNEL_TEMP 0
#define CHANNEL_PHOTO 1
#define ON 1
#define OFF 0
#define CONTEMP_MODE_STATE 0
#define TEMP_MODE_STATE 1
#define PHT_MODE_STATE 2
#define OFF_MODE_STATE 3

long long n;

uint16_t number_sample = DEFAULT_SAMPLE;

char message[20] = {'\0'};

uint8_t stato=0;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
        ADC_DelSig_Start(); 
        UART_Start();
        Timer_Start();
        AMux_Start();
        isr_ADC_StartEx(Custom_ISR_timer);
        EZI2C_Start();
        ADC_DelSig_StartConvert();
    
        slaveBuffer[Control_Reg_1] = PSoC_REG1;
        slaveBuffer[Control_Reg_2] = PSoC_REG2;
        // Set up who am i register
        slaveBuffer[SLAVE_BUFFER_SIZE-5] = 0xBC;
        dataBuffer[0] = 0xA0; 
        dataBuffer[TRANSMIT_BUFFER_SIZE-1] = 0xC0;
 
        avarage_flag=0;
        sum_value_photo=0;
        sum_value_temp=0;
        value_final_photo=0;
        value_final_temp=0;
        
        EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, SLAVE_BUFFER_SIZE-4, slaveBuffer);
       
    for(;;)
    {
        number_sample=(slaveBuffer[Control_Reg_1] & 0b00111100)>>2;
        if(number_sample==0) number_sample=1;
        /*secondo me il pc lo intende già come numero intero se lo utilizzi come numero intero, tipo nel for a riga 97
        se invece non lo intende già come numero intero dobbiamo trovare un modo di convertirlo*/
        uint8_t stato=(slaveBuffer[Control_Reg_1] & 0b00000011);
        
        if(stato==CONTEMP_MODE){
            stato = CONTEMP_MODE_STATE;
        }
        else if(stato==TEMP_MODE){
            stato=TEMP_MODE_STATE;
        }
        else if(stato==PHT_MODE){
            stato=PHT_MODE_STATE;
        }
        else if(stato==OFF_MODE){
            stato=OFF_MODE_STATE;
        }
        
        switch(stato){
            case CONTEMP_MODE_STATE:
            LEDPin_Write(ON);
            for(i=0; i < number_sample; i++){
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
                count++;
            }
            while(!avarage_flag);
            if (avarage_flag==1){
                avarage_flag=0;
                if(count==number_sample){  //double check
                    value_final_photo=sum_value_photo/count;
                    value_final_temp=sum_value_temp/count;
                    sum_value_photo=0;
                    sum_value_temp=0;
                    count=0;
                }
                else break;
                slaveBuffer[3]=value_final_temp >> 8; //put in MSB
                slaveBuffer[4]=value_final_temp & 0xFF; //remain in LSB
                
                dataBuffer[1] = 0;
                dataBuffer[2] = 0;
                dataBuffer[3] = value_final_temp >> 8;
                dataBuffer[4] = value_final_temp & 0xFF;
                        
                UART_PutArray(dataBuffer, TRANSMIT_BUFFER_SIZE);
                
                sprintf(message, "Temp: %ld\r\n", value_final_temp);
                UART_PutString(message);
                    
                slaveBuffer[5]=value_final_photo >> 8; //put in MSB
                slaveBuffer[6]=value_final_photo & 0xFF; //remain in LSB
                
                dataBuffer[1] = value_final_photo >> 8;
                dataBuffer[2] = value_final_photo & 0xFF;
                dataBuffer[3] = 0x00;
                dataBuffer[4] = 0x00;
                        
                UART_PutArray(dataBuffer, TRANSMIT_BUFFER_SIZE);
                
                sprintf(message, "Photo: %ld\r\n", value_final_photo);
                UART_PutString(message);
            }
            break;
            case TEMP_MODE_STATE:
            LEDPin_Write(OFF);
            for(i=0; i < number_sample; i++){
                AMux_Select(CHANNEL_TEMP);
                value_digit=ADC_DelSig_Read32(); //ricorda che per 16 bit single sample dobbiamo usare la funzione read32, ed è per questo che abbiamo inizializzato le variabili a 32 bit
                if (value_digit < 0) value_digit=0;
                if (value_digit > 65535) value_digit=65535;
                sum_value_temp=sum_value_temp+value_digit;
                count++;
            }
            while(!avarage_flag);
            if (avarage_flag==1){
                avarage_flag=0;
                if(count==number_sample){ //double check
                    value_final_temp=sum_value_temp/count;
                    sum_value_temp=0;
                    count=0;
                    //sprintf(message, "photo: %ld\r\n", value_final_photo);
                    //UART_1_PutString(message);
                    //sprintf(message, "temp: %ld\r\n", value_final_temp);
                    //UART_1_PutString(message);
                }
                else break;
                slaveBuffer[3]=value_final_temp >> 8; //put in MSB
                slaveBuffer[4]=value_final_temp & 0xFF; //remain in LSB

                dataBuffer[1] = 0;
                dataBuffer[2] = 0;
                dataBuffer[3] = value_final_temp >> 8;
                dataBuffer[4] = value_final_temp & 0xFF;
                        
                UART_PutArray(dataBuffer, TRANSMIT_BUFFER_SIZE);
                
                sprintf(message, "Temp: %ld\r\n", value_final_temp);
                UART_PutString(message);
            }
            break;
            case PHT_MODE_STATE:
            LEDPin_Write(OFF);
            for(i=0; i < number_sample; i++){
                AMux_Select(CHANNEL_PHOTO);
                value_digit=ADC_DelSig_Read32(); //ricorda che per 16 bit single sample dobbiamo usare la funzione read32, ed è per questo che abbiamo inizializzato le variabili a 32 bit
                if (value_digit < 0) value_digit=0;
                if (value_digit > 65535) value_digit=65535;
                sum_value_photo=sum_value_photo+value_digit;
                count++;
            }
            while(!avarage_flag);
            if (avarage_flag==1){
                avarage_flag=0;
                if(count==number_sample){  //double check
                    value_final_photo=sum_value_photo/count;
                    sum_value_photo=0;
                    count=0;
                    //sprintf(message, "photo: %ld\r\n", value_final_photo);
                    //UART_1_PutString(message);
                    //sprintf(message, "temp: %ld\r\n", value_final_temp);
                    //UART_1_PutString(message);
                }
                else break;
            }
            slaveBuffer[5]=value_final_photo >> 8; //put in MSB
            slaveBuffer[6]=value_final_photo & 0xFF; //remain in LSB
            dataBuffer[1] = value_final_photo >> 8;
            dataBuffer[2] = value_final_photo & 0xFF;
            dataBuffer[3] = 0x00;
            dataBuffer[4] = 0x00;
                    
            UART_PutArray(dataBuffer, TRANSMIT_BUFFER_SIZE);
            
            sprintf(message, "Photo: %ld\r\n", value_final_photo);
            UART_PutString(message);
                    
            break;
            case OFF_MODE_STATE:
                LEDPin_Write(OFF);
                break;
        }
    }
}

/* [] END OF FILE */
