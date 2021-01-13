#include "LPC17xx.h"
#include "gpio.h"
#include "delay.h"
#include "timer.h"
#include "uart.h"
#include "sdcard.h"
#include "fat32.h"
#include "spi.h"


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
	
	// LED MAtrix
	LPC_GPIO1 -> FIODIR = (uint32_t)(0xffff<<16);
	
	GPIO_PinDirection(P2_11,INPUT);
	GPIO_PinDirection(P2_10,INPUT);
}



uint32_t timer = 0,frame_per_sec = 0 ;
_Bool col_update_flage = TRUE;
_Bool go_next_frame = FALSE;
void Time_ISR_1(void){
	timer++;
	frame_per_sec++;
	if(timer == 10){
		col_update_flage = TRUE;
		timer = 0;
	}
	//8 Hz = freq => 1250 ms
	if(frame_per_sec == 1250){
		go_next_frame = TRUE;
		frame_per_sec = 0;
	}
}

uint8_t col_holder = 0; 
void matrix_led(uint8_t data[8]){
	if(col_update_flage){
		col_update_flage = FALSE;
		LPC_GPIO1 -> FIOCLR = (uint32_t) (0xffff<<16);
		LPC_GPIO1 -> FIOSET = (uint32_t) (data[col_holder]<<16);
		uint8_t column = (uint8_t) (1<<col_holder);
		LPC_GPIO1 -> FIOSET = (uint32_t) (column << 24);
	
		col_holder++;
		if(col_holder == 8)
			col_holder = 0;
	}
}



int main()
{	
	setup();
	
	TIMER_Init(TIMER_1,100);
	TIMER_Start(TIMER_1);
	TIMER_Attaread_charInterrupt(TIMER_1, Time_ISR_1);
	
	UART2_Init(9600);
	
	//q1
//	UART2_TxString("\n\rHI");
//	
//	  read_charar read_char;
//    fileConfig_st *ptr_file;
//    uint8_t FileStatus,sdcardType;

//    FileStatus = SD_Init(&sdcardType);
//	  ptr_file = FILE_Open("GROUP-09.txt",READ,&FileStatus);
//    UART2_TxString("\n\rGroup 09 Info: \n");
//    while(read_char != EOF){
//			read_char = FILE_Getread_char(ptr_file);
//          UART2_Txread_charar(read_char);
//      }
//      FILE_Close(ptr_file);
		
	// q2
//	uint8_t group_9[8] = {0xf9,
//						0xf9,
//						0x99,
//						0x99,
//						0x99,
//						0x99,
//						0x9f,
//						0x9f};
//	
//	while(1){
//			matrix_led(group_9);
//		
//	}
	
	
	//q3
	UART2_TxString("\n\rHI");
	
	read_charar read_char;
	uint16_t char_number = 0;
	uint8_t all_frames[272];
    fileConfig_st *ptr_file;
    uint8_t FileStatus,sdcardType;

    FileStatus = SD_Init(&sdcardType);

	ptr_file = FILE_Open("mpl.anm",READ,&FileStatus);


    UART2_TxString("\n\rAnimation is On:");
    while(read_char != EOF){
		read_char = FILE_Getread_char(ptr_file);
		all_frames[char_number] = read_char;
		char_number++;
    }
    UART2_TxString("\n\rBye");
    FILE_Close(ptr_file);
      
	  
	uint8_t frame_counter = 0 , frame[8];
	char_number = 0;
	while(1){
		if(go_next_frame){
			go_next_frame = FALSE;
			
		for(uint8_t i; i<8;i++)
			frame[i] = all_frames[frame_counter*8+i];
			
		frame_counter++;
		if(frame_counter == 35)
			frame_counter = 0;
		}
		
		matrix_led(frame);

	}

}

