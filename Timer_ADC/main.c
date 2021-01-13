#include "LPC17xx.h"
#include "timer.h"
#include "adc.h"
#include "gpio.h"
#include "lcd.h"
#include "delay.h"
#include "uart.h"
#include "math.h"

#include <string.h> 
#include <ctype.h>  
#include "stdio.h"

uint32_t timer = 0;
_Bool timer_flag = TRUE;

void init_lcd(){
	LCD_SetUp(P0_0,P0_1,P0_2,P_NC,P_NC,P_NC,P_NC,P0_4,P0_5,P0_6,P0_7);
	LCD_Init(2,16);
	LCD_Clear();
}


long SensMap(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint16_t get_V(uint16_t* adc){
	uint16_t val;
	*adc = ADC_GetAdcValue(0);
	val =  SensMap(*adc,0,4095,0,3300);
	return val;
}
uint16_t get_LUX(uint16_t* adc){
	uint16_t val;
	*adc = ADC_GetAdcValue(1);
	val = SensMap(*adc,0,4095,10000,10);
	return val;
}
uint16_t get_TEMPR(uint16_t* adc){
	uint16_t val;
	*adc = ADC_GetAdcValue(2);
	val = SensMap(*adc,0,4095,0,330);
	return val;
}
uint16_t get_HUMD(uint16_t* adc){
	uint16_t val;
	*adc = ADC_GetAdcValue(3);
	val = SensMap(*adc,0,4095,100,0);
	return val;
}
void campatalize(char* string)
{
    const uint8_t OFFSET = 32;
    while (*string)
    {
        *string = (*string >= 'a' && *string <= 'z') ? *string -= OFFSET : *string;
        string++;
    }
}


void setup(void){
	SystemInit();
	
	GPIO_PinDirection(P0_15,OUTPUT);
	GPIO_PinDirection(P0_16,OUTPUT);
	GPIO_PinDirection(P0_17,OUTPUT);
	GPIO_PinDirection(P0_18,OUTPUT);
	GPIO_PinDirection(P0_19,OUTPUT);
	GPIO_PinDirection(P0_20,OUTPUT);
	GPIO_PinDirection(P0_21,OUTPUT);
	GPIO_PinDirection(P0_22,OUTPUT);
	
	GPIO_PinDirection(P0_23,OUTPUT);	
	GPIO_PinDirection(P0_24,OUTPUT);
	GPIO_PinDirection(P0_25,OUTPUT);
	GPIO_PinDirection(P0_26,OUTPUT);
	
	GPIO_PinDirection(P1_0,INPUT);
	GPIO_PinDirection(P1_1,INPUT);
	GPIO_PinDirection(P1_2,INPUT);
	GPIO_PinDirection(P1_3,INPUT);
	GPIO_PinDirection(P1_4,INPUT);
	GPIO_PinDirection(P1_5,INPUT);
	GPIO_PinDirection(P1_6,INPUT);
	GPIO_PinDirection(P1_7,INPUT);
	
	
	
	GPIO_PinDirection(P0_1,OUTPUT);	
	GPIO_PinDirection(P0_2,OUTPUT);
	GPIO_PinDirection(P0_3,OUTPUT);
	GPIO_PinDirection(P0_4,OUTPUT);
	GPIO_PinDirection(P0_5,OUTPUT);
	GPIO_PinDirection(P0_6,OUTPUT);
	GPIO_PinDirection(P0_7,OUTPUT);
	
	GPIO_PinDirection(P2_10,OUTPUT);	
	GPIO_PinDirection(P2_11,OUTPUT);
	GPIO_PinDirection(P2_12,OUTPUT);
	GPIO_PinDirection(P2_13,OUTPUT);
}

void ISR(void){
	timer++;
	if(timer%10 == 0 )
		timer_flag = TRUE;
}

void Transmit_packet(char *str, char *password , uint8_t numtry, uint16_t volt, uint16_t lux, uint16_t temp, uint16_t hum){
	char data[30];
	
	str[0] = '&';
	str[1] = '4';
	str[2] = '-';
	for(uint8_t i=0; i<7; i++)
		str[3+i] = password[i];
	str[10] = '-';
	
	for(uint8_t i=0; i<2; i++)
		str[11+i] = data[i];
	str[13] = '-';
	
	for(uint8_t i=0; i<3; i++)
		str[14+i] = data[i];
	str[17] = '-';
	
	for(uint8_t i=0; i<3; i++)
		str[18+i] = data[i];
	str[21] = '-';
	
	for(uint8_t i=0; i<2; i++)
		str[22+i] = data[i];
	str[24] = '-';
	
	for(uint8_t i=0; i<2; i++)
		str[25+i] = data[i];

	str[27]='\n';
	
}

int main (){	
	SystemInit();
	setup();
	
	TIMER_Init(TIMER_1,1000000);
	TIMER_Start(TIMER_1);
	TIMER_AttachInterrupt(TIMER_1, ISR);
	
	
	init_lcd();
	UART2_Init(9600);
	
	
	//Q1
//	char pass[11] = "%GROUP 09\n";
//	UART2_TxString(pass);
//	LCD_Printf("GROUP 09\n");
//	UART2_RxString(pass);
//	LCD_SetCursor(1, 0);
//	LCD_Printf(pass);
//	
//	

	//Q2
//	uint8_t round = 1;
//	_Bool flag = TRUE;
//	char* password = "5043324";
//	
//		while(1){
//		if(flag == TRUE && timer_flag == TRUE){
//			timer_flag = FALSE;
//			
//			LCD_Clear();
//			
//			char str[28] = "";
//			ADC_Init();
//			uint16_t adc_num;
//			Transmit_packet(str,password,round,get_V(&adc_num),get_LUX(&adc_num),get_TEMPR(&adc_num),get_HUMD(&adc_num));
//			UART2_TxString(str);
//			UART2_RxString(str);
//			LCD_GoToLine(0);
//			LCD_Printf("%d",round);
//			
//			round++;
//			if(round>10)
//				flag = FALSE;
//		}	
//	}
	

	//Q3
	char str[10] = "";
	char sentence[53] = "";
	
	char transmit_gp[5] = "$05\n";
	UART2_TxString(transmit_gp);
	uint8_t char_num = UART2_RxString(sentence);
	
	LCD_Printf("%d", char_num);
	
//	char junk1[15] = "";
//	for(uint8_t i=0; i<15; i++)
//		junk1[i] = sentence[i];
//	
//	LCD_SetCursor(0,0);
//	LCD_Printf(junk1);
	
	campatalize(sentence);
	

//	char junk2[15] = "";
//	for(uint8_t i=0; i<15; i++)
//		junk2[i] = sentence[i];
//		
//	LCD_SetCursor(1,0);
//	LCD_Printf(junk2);
	
	
	char TX_data[58] = "";
	TX_data[0] = '@';
	TX_data[1] = '0';
	TX_data[2] = '9';
	TX_data[3] = ':';
	for(uint8_t i=0;i<53;i++)
		TX_data[4+i] = sentence[i];
	TX_data[57] = '\n';
	
	UART2_TxString(TX_data);
	UART2_RxString(str);
	LCD_GoToLine(1);
	LCD_Printf(str);
	
}
