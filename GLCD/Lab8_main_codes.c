#include "LPC17xx.h"
#include "gpio.h"
#include "delay.h"
#include "timer.h"
#include "uart.h"
#include "sdcard.h"
#include "fat32.h"
#include "spi.h"
#include "ks0108.h" 



char picture[1024];
char* frame = "";
int timerIntervalInUs = 100000;

void setup(void){
	SystemInit();
	
	GPIO_PinDirection(P0_1,OUTPUT);	
	GPIO_PinDirection(P0_2,OUTPUT);
	GPIO_PinDirection(P0_3,OUTPUT);
	GPIO_PinDirection(P0_4,OUTPUT);
	GPIO_PinDirection(P0_5,OUTPUT);
	GPIO_PinDirection(P0_6,OUTPUT);
	GPIO_PinDirection(P0_7,OUTPUT);
	
}


int main()
{	
	setup();
	

//	TIMER_Init(TIMER_1,100);
//	TIMER_Start(TIMER_1);
//	TIMER_AttachInterrupt(TIMER_1, Timer1ISR);
	
	UART2_Init(9600);
	
	
	
	GLCD_Initialize();
	GLCD_ClearScreen();
	
//	//q1
//	char name1[]="In the Name of God";
//	char name2[]="Seyed Reza Hosseini";
//	char name3[]="Soheil Shirvani";
//	char name4[]="Anita Ghandehari";
//	
//	GLCD_GoTo(10,0);
//	GLCD_WriteString(name1);
//	GLCD_GoTo(0,2);
//	GLCD_WriteString(name2);
//	GLCD_GoTo(0,3);
//	GLCD_WriteString(name3);
//	GLCD_GoTo(0,4);
//	GLCD_WriteString(name4);
	// x, y is position from top-left to write
//	
//	//q2
//	UART2_TxString("\n\rhi\n");
//	uint8_t y = 0, x = 0;
//	GLCD_GoTo(x,y);
//	
//	while(1){
//		char C = UART2_RxChar();
//		x++;
//		if(x == 20){
//			x	=	0;
//			y++;
//			GLCD_GoTo(0,y);
//		}
//		if(C == '\n'){
//			y++;
//			GLCD_GoTo(0,y);
//		}
//		
//		//GLCD_GoTo(x,y);
//		GLCD_WriteChar(C);
//		
//	}

//	//q3
//	GLCD_Rectangle(2,2,126,62);
//	GLCD_Circle(54,20,7);
//	GLCD_Line(30,55,98,55);
//	GLCD_Line(30,55,45,35);
//	GLCD_Line(45,35,58,43);
//	GLCD_Line(58,43,80,20);
//	GLCD_Line(80,20,98,55);
//			

// //q4
	uint8_t returnStatus, sdcardType;
	fileConfig_st *srcFilePtr;
	returnStatus = SD_Init(&sdcardType);
	srcFilePtr = FILE_Open("mpl.img",READ,&returnStatus);
	
	for(int i=0;i<=1023;i++){
		picture[i] = FILE_GetCh(srcFilePtr);
		if(picture[i] == EOF){
			break;
		}
		FILE_Close(srcFilePtr);
		while(1){
			GLCD_Bitmap(picture,0,0,128,128);
		}
	}
	
}
