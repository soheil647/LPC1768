#include "LPC17xx.h"
#include "gpio.h"
#include "lcd.h"
#include "delay.h"
#include <math.h>

uint32_t number_of_steps = 50;
uint32_t delay;

uint32_t T1 = 9 * 50;
uint32_t T2 = 9 * 80;
uint32_t time1 = 0;
uint32_t time2 = 0;

int counter1 = 0;
int counter2 = 0;

int clock = 1;
uint32_t clock_time = 0;

Boolean_et steeper_status = TRUE;

uint8_t lookup_table[4][4] = {
    {1,1,0,0},
    {0,1,1,0},
    {0,0,1,1},
    {1,0,0,1}};

void set_outputs( int index)
{
    // drive the four main outputs that control the stepper;
    // use the values from the look up table;
    
    if ( lookup_table[index][0] == 0) {
        LPC_GPIO2->FIOCLR |= 1 << 10;
    } else {
        LPC_GPIO2->FIOSET |= 1 << 10;        
    }
    if ( lookup_table[index][1] == 0) {
        LPC_GPIO2->FIOCLR |= 1 << 11;
    } else {
        LPC_GPIO2->FIOSET |= 1 << 11;        
    }    
    if ( lookup_table[index][2] == 0) {
        LPC_GPIO2->FIOCLR |= 1 << 12;
    } else {
        LPC_GPIO2->FIOSET |= 1 << 12;        
    }
    if ( lookup_table[index][3] == 0) {
        LPC_GPIO2->FIOCLR |= 1 << 13;
    } else {
        LPC_GPIO2->FIOSET |= 1 << 13;        
    }
}


void speed(uint32_t speed)
{
	delay = 60 * 1000 * 1000 / number_of_steps / speed;
}

Boolean_et check_time_1(){
		return time1 >= T1;
}

Boolean_et check_time_2(){
		return time2 >= T2;
}
	
void display_time(){
	if(check_time_1()){
		time1 = time1 - T1;
		counter1 = counter1 + 1;
	}
	if(check_time_2()){
		time2 = time2 - T2;
		counter2 = counter2 + 1;
	}
	int count = 0;
	LCD_Clear();
	LCD_SetCursor(0, 0);
	count = (counter1 == 0) ? 1  : (log10(counter1) + 1);
	LCD_DisplayNumber(10, counter1, count);
	LCD_SetCursor(1, 0);
	count = (counter2 == 0) ? 1  : (log10(counter2) + 1);
	LCD_DisplayNumber(10, counter2, count);
}
void find_digit(int number){
	if(number == 0){
		LPC_GPIO0-> FIOCLR =0x007F8000;
		DELAY_ms(1);
		LPC_GPIO0-> FIOSET =0x001F8000;
	}
	else if(number == 1){
		LPC_GPIO0-> FIOCLR =0x007F8000;
		LPC_GPIO0-> FIOSET =0x00030000;
	}
	else if(number == 2){
		LPC_GPIO0-> FIOCLR =0x007F8000;
		LPC_GPIO0-> FIOSET =0x002D8000;
	}
	else if(number == 3){
		LPC_GPIO0-> FIOCLR =0x007F8000;
		LPC_GPIO0-> FIOSET =0x00278000;
	}
	else if(number == 4){
		LPC_GPIO0-> FIOCLR =0x007F8000;
		LPC_GPIO0-> FIOSET =0x00330000;
	}
	else if(number == 5){
		LPC_GPIO0-> FIOCLR =0x007F8000;
		LPC_GPIO0-> FIOSET =0x00368000;
	}
	else if(number == 6){
		LPC_GPIO0-> FIOCLR =0x007F8000;
		LPC_GPIO0-> FIOSET =0x003E8000;
	}
	else if(number == 7){
		LPC_GPIO0-> FIOCLR =0x007F8000;
		LPC_GPIO0-> FIOSET =0x00038000;
	}
	else if(number == 8){
		LPC_GPIO0-> FIOCLR =0x007F8000;
		LPC_GPIO0-> FIOSET =0x003F8000;
	}
	else if(number == 9){
		LPC_GPIO0-> FIOCLR =0x007F8000;
		LPC_GPIO0-> FIOSET =0x00378000;
	}
}


void print_7_segment(int num1, int num2, int num3, int num4){
	int i = 0;
	while(i <= 5){
		LPC_GPIO0-> FIOCLR =0x07800000;
		find_digit(num1);
		LPC_GPIO0->FIOSET = 0x00800000;
		DELAY_ms(1);
		
		LPC_GPIO0-> FIOCLR =0x07800000;
		find_digit(num2);
		LPC_GPIO0->FIOSET = 0x01000000;
		DELAY_ms(1);
					
		LPC_GPIO0-> FIOCLR =0x07800000;
		find_digit(num3);
		LPC_GPIO0->FIOSET = 0x02000000;
		DELAY_ms(1);
					
		LPC_GPIO0-> FIOCLR =0x07800000;
		find_digit(num4);
		LPC_GPIO0->FIOSET = 0x04000000;
		DELAY_ms(1);
		i = i + 1;
	}
	LPC_GPIO0->FIOSET = 0x07800000;
}

void print_clock_7_segment(){
	int ten = (clock / 10) % 10;
	int digit = clock % 10;
	
	print_7_segment(digit,ten,0,0);
}

void print_clock_lcd(){
	int count = 0;
	LCD_Clear();
	LCD_SetCursor(0, 0);
	count = (clock == 0) ? 1  : (log10(clock) + 1);
	LCD_DisplayNumber(10, clock, count);
	if(clock % 2 == 0)
		LCD_Printf(":");
	else 
		LCD_Printf(" ");
	LCD_DisplayNumber(10, 0, 2);
}

void print_clock_steeper(){
	if(steeper_status == TRUE){
		steeper_status = FALSE;
		int i;
		if(clock % 2 == 0){
			for ( i = 2; i <= 3; i++ ) {
				set_outputs(i); 
				DELAY_ms(10);
				clock_time = clock_time + 10;
			}
		}
		else{
			for ( i = 0; i <= 1; i++ ) {
				set_outputs(i); 
				DELAY_ms(10);
				clock_time = clock_time + 10;
		 }
	 }
	}
}


void show_clock(){
	if(clock_time >= 1000){
		clock_time = 0;
		steeper_status = TRUE;
		if(clock >= 12){
			clock = 1;
		}
		else{
			clock = clock + 1;
		}
	}
	
	print_clock_7_segment();
	print_clock_lcd();
	print_clock_steeper();
}

int main( void) 
{
	LPC_GPIO2->FIODIR |= (1 << 10) | (1 << 11) | (1 << 12) | (1 << 13);
	LPC_GPIO1 -> FIODIR = 0x00000000; // 0 for Input - 1 for Output
	LPC_GPIO0 -> FIODIR = 0xFFFFFFFF; // 0 for Input - 1 for Output
	LPC_GPIO0-> FIOCLR = 0xFFFFFFFF;

	LPC_GPIO2->FIOCLR |= 1 << 10;
	LPC_GPIO2->FIOCLR |= 1 << 11;
	LPC_GPIO2->FIOCLR |= 1 << 12;
	LPC_GPIO2->FIOCLR |= 1 << 13;  
//	int i = 0;
//	int j = 0;
//	while(j <= 4){
//		for ( i = 0; i <= 3; i++ ) {
//					set_outputs(i); 
//					DELAY_ms(500);
//		 }
//			j++;		
//	 }
	
//	LCD_SetUp(P0_0,P0_1,P0_2,P_NC,P_NC,P_NC,P_NC,P0_4,P0_5,P0_6,P0_7);
//	LCD_Init(2,16);
//	LCD_Clear();
//	while(TRUE){
//		display_time();
//		DELAY_ms(100);
//		time1 = time1 + 100;
//		time2 = time2 + 100;
//	}

	LCD_SetUp(P0_0,P0_1,P0_2,P_NC,P_NC,P_NC,P_NC,P0_4,P0_5,P0_6,P0_7);
	LCD_Init(2,16);
	while(TRUE){
		show_clock();
		clock_time = clock_time + 100;
		DELAY_ms(100);
	}

}
