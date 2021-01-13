#include "LPC17xx.h"
#include "gpio.h"
#include "lcd.h"
#include "delay.h"
#include "timer.h"
#include "adc.h"
#include "uart.h"
#include "stdio.h"
#include "pwm.h"
#include "extintr.h"

#define  A  P2_0
#define  B  P2_1
#define  En P2_2

uint32_t time_number = 0;

uint32_t timer = 0 ,bac_timer = 0;
_Bool flag = TRUE;


void init_lcd(){
	LCD_SetUp(P0_0,P0_1,P0_2,P_NC,P_NC,P_NC,P_NC,P0_4,P0_5,P0_6,P0_7);
	LCD_Init(2,16);
	LCD_Clear();
}

void setup(void){
	SystemInit();

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
	
	GPIO_PinDirection(A,OUTPUT);
	GPIO_PinDirection(B,OUTPUT);
	GPIO_PinDirection(En,OUTPUT);
	
	GPIO_PinDirection(P2_11,INPUT);
	GPIO_PinDirection(P2_10,INPUT);
}

void Timer1ISR(void){
	time_number++;
	bac_timer = timer;
	timer = 0; 
	flag = TRUE;
}

void DC_motor(uint8_t speed){
	
	GPIO_PinWrite(B,0);
	GPIO_PinWrite(A,1);
	
	PWM_SetDutyCycle(PWM_3,speed);
	
}


void set_time(void){
	bac_timer = timer;
	timer = 0;	
}

void encoding_dc_motor(void){
	timer++;	
}

int main()
{	
	setup();
	init_lcd();
	
	PWM_Init(255);
	PWM_Start(PWM_3);
	
		
	//q2
	TIMER_Init(TIMER_1,10);
	EINT_AttachInterrupt(EINT1, set_time, RISING);
	EINT_AttachInterrupt(EINT0, encoding_dc_motor, RISING);

	
	
	
	
	while(1){
		//q1
		//	DC_motor(127);
		
		//q2
		if(time_number%100000 == 0){
			LCD_Clear();
			LCD_GoToLine(0);
			LCD_Printf("%f Hz",100000/(float)bac_timer);
			LCD_GoToLine(1);
			LCD_Printf("Group : 02");
		}
		
	}

}

