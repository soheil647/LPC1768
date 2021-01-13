#include "LPC17xx.h"
#include "gpio.h"
#include "uart.h"

int main()
{
	SystemInit();
	GPIO_PinDirection(P2_11,INPUT);
	GPIO_PinDirection(P2_10,INPUT);
	
	
	UART2_Init(9600);
	UART2_TxString("Program Started\n");
	
	
	uint8_t y = 0, x = 0;
	GLCD_GoTo(x,y);
	while(1){
		char C = UART2_RxChar();
		x++;
		if(x == 20){
			x	=	0;
			y++;
			GLCD_GoTo(0,y);
		}
		if(C == '\n'){
			y++;
			GLCD_GoTo(0,y);
		}
		
		GLCD_WriteChar(C);
	}	
}
