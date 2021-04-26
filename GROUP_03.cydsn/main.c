/* ========================================
 * GROUP_03 - HW_03
 * Di Liddo Mauro, Goshen Davide
 * main.c
 * ========================================
*/
#include "project.h"
#include "InterruptRoutines.h"
#include "stdio.h"
#include "PSoC_slave.h"
#include "math.h"


#define SLAVE_BUFFER_SIZE 7

volatile uint8_t slaveBuffer[SLAVE_BUFFER_SIZE];

#define Control_Reg_1 0
#define Control_Reg_2 1

//define for the avg computation
int32 value_digit;
int32 sum_value_photo;
int32 sum_value_temp;
int32 value_final_photo;
int32 value_final_temp;
uint16_t count=0;
uint16_t i=0;

//define for the switch-case
#define CHANNEL_TEMP 0
#define CHANNEL_PHOTO 1
#define ON 1
#define OFF 0
#define CONTEMP_MODE_STATE 0
#define TEMP_MODE_STATE 1
#define PHT_MODE_STATE 2
#define OFF_MODE_STATE 3


uint8_t number_sample = DEFAULT_SAMPLE;
uint8_t period = DEFAULT_PERIOD;

uint8_t stato=0;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
        ADC_DelSig_Start();
        Timer_Start();
        AMux_Start();
        isr_ADC_StartEx(Custom_ISR_timer);
        EZI2C_Start();
        ADC_DelSig_StartConvert();
        //set up control register_1&&2
        slaveBuffer[Control_Reg_1] = PSoC_REG1;
        slaveBuffer[Control_Reg_2] = PSoC_REG2;
        //set up who am i register
        slaveBuffer[SLAVE_BUFFER_SIZE-5] = 0xBC;
        //flag and values init
        read_flag=0;
        sum_value_photo=0;
        sum_value_temp=0;
        value_final_photo=0;
        value_final_temp=0;
        //setting slaveBuffer as the actual SLAVE BUFFER
        EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, SLAVE_BUFFER_SIZE-4, slaveBuffer);
       
    for(;;)
    {
        //save the desired number of samples
        number_sample=(slaveBuffer[Control_Reg_1] & 0b00111100)>>2;
        if(number_sample==0) number_sample=1;
        //save which channels we want to read
        uint8_t stato=(slaveBuffer[Control_Reg_1] & 0b00000011);
        //save the period value
        period=slaveBuffer[Control_Reg_2];
        //setting the timer with the desired period value
        Timer_WritePeriod(period);
        //defining the states depending on the channel that we want to read
        if(stato==CONTEMP_MODE)
        {
            //both sensors on
            stato = CONTEMP_MODE_STATE;
        }
        else if(stato==TEMP_MODE)
        {
            //only temp sensor on
            stato=TEMP_MODE_STATE;
        }
        else if(stato==PHT_MODE)
        {
            //only light sensor on
            stato=PHT_MODE_STATE;
        }
        else if(stato==OFF_MODE)
        {
            //shut down of the communication
            stato=OFF_MODE_STATE;
        }
        
        switch(stato){
            case CONTEMP_MODE_STATE:
            LEDPin_Write(ON);
            if(read_flag==1){
                read_flag=0;
                AMux_Select(CHANNEL_PHOTO);
                value_digit=ADC_DelSig_Read32(); //ricorda che per 16 bit single sample dobbiamo usare la funzione read32, ed è per questo che abbiamo inizializzato le variabili a 32 bit
                if (value_digit < 0) value_digit=0;
                if (value_digit > 65535) value_digit=65535;
                
                AMux_Select(CHANNEL_TEMP);
                value_digit=ADC_DelSig_Read32(); //ricorda che per 16 bit single sample dobbiamo usare la funzione read32, ed è per questo che abbiamo inizializzato le variabili a 32 bit
                if (value_digit < 0) value_digit=0;
                if (value_digit > 65535) value_digit=65535;
                
                sum_value_photo=sum_value_photo+value_digit;
                sum_value_temp=sum_value_temp+value_digit;
                count++;
                if(count==number_sample){
                    value_final_photo=sum_value_photo/count;
                    value_final_temp=sum_value_temp/count;
                    sum_value_photo=0;
                    sum_value_temp=0;
                    count=0;
                    
                    slaveBuffer[3]=value_final_temp >> 8; //put in MSB
                    slaveBuffer[4]=value_final_temp & 0xFF; //remain in LSB
                    
                    slaveBuffer[5]=value_final_photo >> 8; //put in MSB
                    slaveBuffer[6]=value_final_photo & 0xFF; //remain in LSB
                }
            }
            break;
            case TEMP_MODE_STATE:
            LEDPin_Write(OFF);
            if(read_flag==1){
                read_flag=0;
                AMux_Select(CHANNEL_TEMP);
                value_digit=ADC_DelSig_Read32(); //ricorda che per 16 bit single sample dobbiamo usare la funzione read32, ed è per questo che abbiamo inizializzato le variabili a 32 bit
                if (value_digit < 0) value_digit=0;
                if (value_digit > 65535) value_digit=65535;
                
                sum_value_temp=sum_value_temp+value_digit;
                count++;
                if(count==number_sample){
                    value_final_temp=sum_value_temp/count;
                    sum_value_temp=0;
                    count=0;
                    
                    slaveBuffer[3]=value_final_temp >> 8; //put in MSB
                    slaveBuffer[4]=value_final_temp & 0xFF; //remain in LSB
                }
            }
            break;
            case PHT_MODE_STATE:
            LEDPin_Write(OFF);
            if(read_flag){
                read_flag=0;
                AMux_Select(CHANNEL_PHOTO);
                value_digit=ADC_DelSig_Read32(); //ricorda che per 16 bit single sample dobbiamo usare la funzione read32, ed è per questo che abbiamo inizializzato le variabili a 32 bit
                if (value_digit < 0) value_digit=0;
                if (value_digit > 65535) value_digit=65535;
                
                sum_value_photo=sum_value_photo+value_digit;
                count++;
                if(count==number_sample){
                    value_final_photo=sum_value_photo/count;
                    sum_value_photo=0;
                    count=0;
                    
                    slaveBuffer[5]=value_final_photo >> 8; //put in MSB
                    slaveBuffer[6]=value_final_photo & 0xFF; //remain in LSB
                }
            }
            break;
            case OFF_MODE_STATE:
                LEDPin_Write(OFF);
                break;
        }
    }
}
/* [] END OF FILE */
