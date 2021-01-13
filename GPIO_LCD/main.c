#include "LPC17xx.h"
#include "gpio.h"
#include "lcd.h"
#include "delay.h"

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
	while(1){
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

void convert_value_to_digits(int value){
	int houndred = (value / 100) % 10;
	int ten = (value / 10) % 10;
	int digit = value % 10;
	
	print_7_segment(digit, ten, houndred, 0);
}


void find_switch_value(){
	uint8_t s[8];
	s[0] = GPIO_PinRead(P1_0);
	s[1] = GPIO_PinRead(P1_1);
	s[2] = GPIO_PinRead(P1_2);
	s[3] = GPIO_PinRead(P1_3);
	s[4] = GPIO_PinRead(P1_4);
	s[5] = GPIO_PinRead(P1_5);
	s[6] = GPIO_PinRead(P1_6);
	s[7] = GPIO_PinRead(P1_7);
	
	int switch_value = s[0] + 2*s[1] + 4*s[2] + 8*s[3] +
		16*s[4] + 32*s[5]  + 64*s[6]  + 128*s[7];
	convert_value_to_digits(switch_value);
}

void display_and_move_names(){
	LCD_SetUp(P0_0,P0_1,P0_2,P_NC,P_NC,P_NC,P_NC,P0_4,P0_5,P0_6,P0_7);
	LCD_Init(2,16);
	char* name1 = "REZA";
	char* name2 = "ANITA";
	int i = 0;
	int j = 0;
	while(TRUE){
		LCD_Clear();
		LCD_SetCursor(0, i);
		LCD_Printf(name1);
		LCD_SetCursor(1, 11-j);
		LCD_Printf(name2);
		
		if(i == 12)
			i=0;
		else i++;
		if(j == 11)
			j=0;
		else j++;
		
		DELAY_ms(1000);
	}
}
void display_time(){
	LCD_SetUp(P0_0,P0_1,P0_2,P_NC,P_NC,P_NC,P_NC,P0_4,P0_5,P0_6,P0_7);
	LCD_Init(2,16);
	LCD_Clear();
	char* name1 = "1399/8/17";
	char* name2 = "7:50";
	LCD_SetCursor(0, 3);
	LCD_Printf(name1);
	LCD_SetCursor(1, 6);
	LCD_Printf(name2);
}

void display_7segment_SID(){
	int sid = 416 + 533 + 596; //1545
	char* sid_char = "1545";
	LCD_SetUp(P0_0,P0_1,P0_2,P_NC,P_NC,P_NC,P_NC,P0_4,P0_5,P0_6,P0_7);
	LCD_Init(2,16);
	LCD_SetCursor(0, 0);
	LCD_Printf(sid_char);
	print_7_segment(5,4,5,1);
}

int main(){
	
	LPC_GPIO1 -> FIODIR = 0x00000000; // 0 for Input - 1 for Output
	LPC_GPIO0 -> FIODIR = 0xFFFFFFFF; // 0 for Input - 1 for Output
	LPC_GPIO0-> FIOCLR = 0xFFFFFFFF;
	

	//Q1
//	print_7_segment(9, 6, 8, 9);
//	find_switch_value();
	
	//Q2
//	display_time();
	
	//Q3
	display_and_move_names();
	
	//Q4
//	display_7segment_SID();
}
