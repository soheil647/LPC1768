#include "gpio.h"
#include "fat32.h"
#include "sdcard.h"
#include "spi.h"
#include "stdutils.h"
#include "delay.h"
#include "uart.h"
#include "timer.h"
#include "ks0108.h"
#include <stdio.h>

char picture[1024];
char* frame = "";
int timerIntervalInUs = 100000;

int main()
{
	SystemInit();
	GLCD_Initialize();
	GLCD_ClearScreen();
	
	uint8_t returnStatus, sdcardType;
	fileConfig_st *srcFilePtr;
	returnStatus = SD_Init(&sdcardType);
	srcFilePtr = FILE_Open("mpl.img",READ,&returnStatus);
	
	for(int i=0;i<=1023;i++){
		picture[i] = FILE_GetCh(srcFilePtr);
		GLCD_Bitmap(picture,0,0,128,128);
		if(picture[i] == EOF){
			break;
		}
		FILE_Close(srcFilePtr);
		while(1){
		}
	
	}


}
