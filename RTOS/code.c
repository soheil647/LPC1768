#include <FreeRTOS.h>
#include <task.h>
#include "LPC17xx.h"
#include "gpio.h"
#include "delay.h"
#include "timer.h"
#include "uart.h"
#include "ks0108.h" 
#include "LCD.h"


#define LED1 P1_0
#define LED2 P1_1
#define LED3 P1_4
#define LED4 P1_8

void setup(){
	SystemInit();
	
	GPIO_PinDirection(LED1,OUTPUT);	
	GPIO_PinDirection(LED2,OUTPUT);
	GPIO_PinDirection(LED3,OUTPUT);
	GPIO_PinDirection(LED4,OUTPUT);
	
	
	
	GPIO_PinDirection(P0_0,OUTPUT);	
	GPIO_PinDirection(P0_1,OUTPUT);	
	GPIO_PinDirection(P0_2,OUTPUT);
	GPIO_PinDirection(P0_3,OUTPUT);
	GPIO_PinDirection(P0_4,OUTPUT);
	GPIO_PinDirection(P0_5,OUTPUT);
	GPIO_PinDirection(P0_6,OUTPUT);
	GPIO_PinDirection(P0_7,OUTPUT);
}
void turn_on_led1(void *pvParameters){
	while(1){
		//1 HZ
		GPIO_PinToggle(LED1);
		vTaskDelay(1000/portTICK_PERIOD_MS); 
	}
}
void turn_on_led2(void *pvParameters){
	while(1){
		//2 HZ
		GPIO_PinToggle(LED2);
		vTaskDelay(2000/portTICK_PERIOD_MS); 
	}
}
void turn_on_led3(void *pvParameters){
	while(1){
		//5 HZ
		GPIO_PinToggle(LED3);
		vTaskDelay(5000/portTICK_PERIOD_MS); 
	}
}
void turn_on_led4(void *pvParameters){
	while(1){
		//10 HZ
		GPIO_PinToggle(LED4);
		vTaskDelay(10000/portTICK_PERIOD_MS); 
	}
}

void setup_lcd(){
	LCD_SetUp(P0_0,P0_1,P0_2,P_NC,P_NC,P_NC,P_NC,P0_4,P0_5,P0_6,P0_7);
	LCD_Init(2,16);
}



uint32_t A_counter,B_counter;
void A_freq(void *pvParameters){
	while(1){
		A_counter++;
		uint32_t delay = 222; //9*50 Khz
		vTaskDelay(delay/portTICK_PERIOD_MS);
	}
}

void B_freq(void *pvParameters){
	while(1){
		B_counter++;
		uint32_t delay = 130;  //9*80 Khz
		vTaskDelay(delay/portTICK_PERIOD_MS);
	}
}


void print_counter_lcd(void *pvParameters){
	while(1){
		LCD_Clear();
		LCD_SetCursor(0,9);
		LCD_Printf("A : %d",A_counter);
		LCD_SetCursor(1,9);
		LCD_Printf("B : %d",B_counter);
		vTaskDelay(100/portTICK_PERIOD_MS);
	}
}



int main() {
	setup();
	setup_lcd();
	LCD_Clear();
	LCD_Printf("H111111111");
	//Q1
	xTaskCreate( turn_on_led1, ( char * )"Task1", 128, NULL, 1, NULL );
	xTaskCreate( turn_on_led2, ( char * )"Task2", 128, NULL, 2, NULL );
	xTaskCreate( turn_on_led3, ( char * )"Task3", 128, NULL, 3, NULL );
	xTaskCreate( turn_on_led4, ( char * )"Task4", 128, NULL, 4, NULL );
	
	
	//Q2
//	xTaskCreate(A_freq, "a_count", 128,NULL, 1, NULL);
//	xTaskCreate(B_freq, "b_count", 128,NULL, 1, NULL);
//	xTaskCreate(print_counter_lcd, "lcd_count", 128,NULL, 1, NULL);
	
	vTaskStartScheduler(); 
	
	while(1){
	}
}
