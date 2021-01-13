#include "LPC17xx.h"
#include "gpio.h"
#include "delay.h"
#include "timer.h"
#include "uart.h"
#include "stdio.h"
#include "ks0108.h" 
#include <FreeRTOS.h>
#include <task.h>
#include "lcd.h"
#include "ks0108.h" 
#include "sdcard.h"
#include "fat32.h"
#include "spi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Setup Functions
void init_lcd(){
	LCD_SetUp(P0_0,P0_1,P0_2,P_NC,P_NC,P_NC,P_NC,P0_4,P0_5,P0_6,P0_7);
	LCD_Init(2,16);
	LCD_Clear();
}
void init_glcd(){
	GLCD_Initialize();
	GLCD_ClearScreen();
}
void setup(void){
	SystemInit();
	GPIO_PinDirection(P1_0,OUTPUT);
	GPIO_PinDirection(P1_1,OUTPUT);
	GPIO_PinDirection(P1_4,OUTPUT);
	GPIO_PinDirection(P1_8,OUTPUT);
}

// Starting Game Functions
void print_start_menu(){
	LCD_Clear();
	LCD_SetCursor(0, 3);
	LCD_Printf("Obstacle run");
	LCD_SetCursor(1, 0);
	LCD_Printf("BY : GROUP 09");
}
void print_start_game(){
	LCD_Clear();
	LCD_SetCursor(0, 0);
	LCD_Printf("SEND M to Start");
	LCD_SetCursor(1, 0);
	LCD_Printf("SEND P to Load");
}

// Global Variable Defines

#define UP 0;
#define DOWN 1;
#define RIGHT 2;
#define LEFT 3;

_Bool start_flag = FALSE; // Starting game Flag
int obstacles_number = 2; // Number Of Opstacles per level

// Player Object
struct player_pos {
	int x_pos;
	int y_pos;
} player;


// Obstacles 
int obstacles[50][12][2]= 
										{ { {114, 10}, {64, 48} } };
										
// Obstacles Moving Direction			
int obstacles_direction[12] = {1,-1,1,-1,1,-1,1,-1,1,-1,1,-1};

// Level Number
int level = 0;							

// Load From SD
void load_game(){
	fileConfig_st *srcFilePtr;
    uint8_t returnStatus,sdcardType;

    returnStatus = SD_Init(&sdcardType);

    if(returnStatus)
    {
        if(returnStatus == SDCARD_NOT_DETECTED)
        {
            UART2_TxString("\n\r SD card not detected..");
        }
        else if(returnStatus == SDCARD_INIT_FAILED)
        {
            UART2_TxString("\n\r Card Initialization failed..");
        }
        else if(returnStatus == SDCARD_FAT_INVALID)
        {
            UART2_TxString("\n\r Invalid Fat filesystem");
        }
				LCD_Clear();
				LCD_GoToLine(0);
				LCD_Printf("SD Failed");
				LCD_GoToLine(1);
				LCD_Printf("Try Another One");
        return;
    }
		srcFilePtr = FILE_Open("GROUP-09.txt",READ,&returnStatus);
    if(srcFilePtr == 0){
			UART2_TxString("\n\rFile Opening Failed");
			
			LCD_Clear();
			LCD_GoToLine(0);
			LCD_Printf("File Failed");
			LCD_GoToLine(1);
			LCD_Printf("Try Another One");
			return;
    }
		level = FILE_GetCh(srcFilePtr);
		FILE_Close(srcFilePtr);
			
		LCD_Clear();
		LCD_GoToLine(0);
		LCD_Printf("Game Loaded");
		LCD_GoToLine(1);
		LCD_Printf("Your Level is: %d", level+1);
		return;
}	
									
// Save to SD
void save_game(){
    fileConfig_st *srcFilePtr;
    uint8_t returnStatus,sdcardType;

    returnStatus = SD_Init(&sdcardType);

    if(returnStatus)
    {
        if(returnStatus == SDCARD_NOT_DETECTED)
        {
            UART2_TxString("\n\r SD card not detected..");
        }
        else if(returnStatus == SDCARD_INIT_FAILED)
        {
            UART2_TxString("\n\r Card Initialization failed..");
        }
        else if(returnStatus == SDCARD_FAT_INVALID)
        {
            UART2_TxString("\n\r Invalid Fat filesystem");
        }
				LCD_Clear();
				LCD_GoToLine(0);
				LCD_Printf("SD Failed");
				LCD_GoToLine(1);
				LCD_Printf("Try Another One");
        return;
    }
		srcFilePtr = FILE_Open("GROUP-09.txt",WRITE,&returnStatus);
    if(srcFilePtr == 0){
			UART2_TxString("\n\rFile Opening Failed");
			
			LCD_Clear();
			LCD_GoToLine(0);
			LCD_Printf("File Failed");
			LCD_GoToLine(1);
			LCD_Printf("Try Another One");
			return;
    }
		FILE_PutCh (srcFilePtr, level);
		FILE_Close(srcFilePtr);
			
		LCD_Clear();
		LCD_GoToLine(0);
		LCD_Printf("Game Saved");
		LCD_GoToLine(1);
		LCD_Printf("Try Another One");
		return;
}	
										
// Input Function
void input_uart(void *pvParameters){
	while(1){
		vTaskDelay(5/portTICK_PERIOD_MS);	
		if(start_flag == TRUE){
			LCD_Clear();
			LCD_SetCursor(0,3);
			LCD_Printf("GOOD LUCK");
			LCD_SetCursor(1,4);
			LCD_Printf("LEVEL %d", level+1);
		}
		
		char data = UART2_RxChar();
		switch (data){
			case ('A'):{
				if(player.x_pos - 16 <= 0 || start_flag == FALSE)
					continue;
				player.x_pos = player.x_pos - 10;
				break;
			}
			
			case ('D'):{
				if(player.x_pos + 16 >= 128 || start_flag == FALSE)
					continue;
				player.x_pos = player.x_pos + 10;
				break;
			}
				
			case ('S'):{
				if(player.y_pos + 16 >= 64 || start_flag == FALSE)
					continue;
				player.y_pos = player.y_pos + 10;
				break;
			}
				
			case ('W'):{
				if(player.y_pos - 16 <= 0 || start_flag == FALSE)
					continue;
				player.y_pos = player.y_pos - 10;
				break;
			}
			
			case ('M'):{
				if(start_flag == FALSE){
					level = 0;
					player.x_pos = 10;
					player.y_pos = 32;
					start_flag = TRUE;
				}
				else{
					start_flag = FALSE;
					LCD_Clear();
					LCD_Printf("Game Stopped");
				}
					
				break;
			}
			
			case ('O'):{
				if(start_flag == FALSE)
					save_game();
				break;
			}
			
			case ('P'):{
				if(start_flag == FALSE){
					load_game();
					player.x_pos = 10;
					player.y_pos = 32;
					start_flag = TRUE;
				}
				break;
			}
			default:{
				LCD_Clear();
				LCD_GoToLine(0);
				LCD_Printf("Invalid Command");
				LCD_GoToLine(1);
				LCD_Printf("Try Another One");
				DELAY_ms(2000);
				break;
			}
		}
	}	
}

void draw_player(){
		//Head
		GLCD_Circle(player.x_pos,player.y_pos,4);
		
		// Body
		GLCD_Line(player.x_pos,player.y_pos+4,player.x_pos,player.y_pos+10);
		
		// Hands
		GLCD_Line(player.x_pos,player.y_pos+8,player.x_pos+6,player.y_pos+4);
		GLCD_Line(player.x_pos,player.y_pos+8,player.x_pos-6,player.y_pos+4);
		
		// Legs
		GLCD_Line(player.x_pos,player.y_pos+10,player.x_pos+5,player.y_pos+15);
		GLCD_Line(player.x_pos,player.y_pos+10,player.x_pos-5,player.y_pos+15);
}

void draw_obsticle(){
	// obstacles
	for(int i = 0; i < obstacles_number; i++){
		//Head
		GLCD_Circle(obstacles[level][i][0], obstacles[level][i][1], 4);
		GLCD_Circle(obstacles[level][i][0], obstacles[level][i][1], 3);
		GLCD_Circle(obstacles[level][i][0], obstacles[level][i][1], 2);
		GLCD_Circle(obstacles[level][i][0], obstacles[level][i][1], 1);
		
		// Body
		GLCD_Line(obstacles[level][i][0]-1,obstacles[level][i][1]+4,obstacles[level][i][0],obstacles[level][i][1]+10);
		GLCD_Line(obstacles[level][i][0],obstacles[level][i][1]+4,obstacles[level][i][0],obstacles[level][i][1]+10);
		GLCD_Line(obstacles[level][i][0]+1,obstacles[level][i][1]+4,obstacles[level][i][0],obstacles[level][i][1]+10);
		
		// Hands
		GLCD_Line(obstacles[level][i][0]-1,obstacles[level][i][1]+8,obstacles[level][i][0]+6,obstacles[level][i][1]+4);
		GLCD_Line(obstacles[level][i][0],obstacles[level][i][1]+8,obstacles[level][i][0]+6,obstacles[level][i][1]+4);
		GLCD_Line(obstacles[level][i][0]+1,obstacles[level][i][1]+8,obstacles[level][i][0]+6,obstacles[level][i][1]+4);
		
		GLCD_Line(obstacles[level][i][0]-1,obstacles[level][i][1]+8,obstacles[level][i][0]-6,obstacles[level][i][1]+4);
		GLCD_Line(obstacles[level][i][0],obstacles[level][i][1]+8,obstacles[level][i][0]-6,obstacles[level][i][1]+4);
		GLCD_Line(obstacles[level][i][0]+1,obstacles[level][i][1]+8,obstacles[level][i][0]-6,obstacles[level][i][1]+4);
		
		// Legs
		GLCD_Line(obstacles[level][i][0]-1,obstacles[level][i][1]+10,obstacles[level][i][0]+5,obstacles[level][i][1]+15);
		GLCD_Line(obstacles[level][i][0],obstacles[level][i][1]+10,obstacles[level][i][0]+5,obstacles[level][i][1]+15);
		GLCD_Line(obstacles[level][i][0]+1,obstacles[level][i][1]+10,obstacles[level][i][0]+5,obstacles[level][i][1]+15);
		
		GLCD_Line(obstacles[level][i][0]-1,obstacles[level][i][1]+10,obstacles[level][i][0]-5,obstacles[level][i][1]+15);
		GLCD_Line(obstacles[level][i][0],obstacles[level][i][1]+10,obstacles[level][i][0]-5,obstacles[level][i][1]+15);
		GLCD_Line(obstacles[level][i][0]+1,obstacles[level][i][1]+10,obstacles[level][i][0]-5,obstacles[level][i][1]+15);
	}
}

void check_game_over(){
	for(int i = 0; i < obstacles_number; i++){
		if(obstacles[level][i][0] - 6 <= player.x_pos && player.x_pos <= obstacles[level][i][0] + 6){
			if(obstacles[level][i][1] -2 <= player.y_pos && player.y_pos <= obstacles[level][i][1] +15){
				start_flag = FALSE;
				LCD_Clear();
				LCD_GoToLine(0);
//				LCD_Printf("%d %d", player.x_pos, player.y_pos);
				LCD_Printf("GameOverLevel %d", level+1);
				LCD_GoToLine(1);
//				LCD_Printf("%d %d", obstacles[level][i][0], obstacles[level][i][1]);
				LCD_Printf("Save,Load,Start");
				break;
			}	
		}
	}
}

// Go Next Level
void next_level(){
	if(player.x_pos >= 120){
		level = level + 1;
		player.x_pos = 10;
		player.y_pos = 32;
		
		
		for(int i=0; i<obstacles_number; i++){
			int random_x = rand() % 108 + 14;
			int random_y = rand() % 32 + 16;
			obstacles[level][i][0] = random_x;
			obstacles[level][i][1] = random_y;	
		}
	}
}

// Create Map Function
void create_map(void *pvParameters){
	while(1){
		vTaskDelay(10/portTICK_PERIOD_MS);
		
		if(start_flag == TRUE){
			GLCD_ClearScreen();
			
			//Check Game Over
			check_game_over();
			
			// Number Of Obstacles
			obstacles_number = 2 + level/5;
			
			// Player
			draw_player();
			
			// Go Next Level
			next_level();
			
			// Obstacles
			draw_obsticle();
		}
	}
}

// Move Obstacles
void move_obstacles(void *pvParameters){
	while(1){
		vTaskDelay(1000 - 10*level/portTICK_PERIOD_MS);
		if(start_flag == TRUE){
			for(int i = 0; i < obstacles_number; i++){
				if(obstacles[level][i][1] + obstacles_direction[i] + 15 >= 64 || obstacles[level][i][1] + obstacles_direction[i] - 2 <= 0)
					obstacles_direction[i] = -obstacles_direction[i];
				
				obstacles[level][i][1] = obstacles[level][i][1] + obstacles_direction[i];
			}
		}
	}
}

int main()
{	
//	srand(time(0));  
	
	setup();
	init_lcd();
	init_glcd();
	UART2_Init(9600);
	
	//Start Menu
	print_start_menu();
	DELAY_ms(5000);
	print_start_game();
	
	player.x_pos = 10;
	player.y_pos = 32;
	
	xTaskCreate(create_map, "renderer", 128,NULL, 2, NULL);
	xTaskCreate(input_uart, "get_input", 128,NULL, 1, NULL);
	xTaskCreate(move_obstacles, "move_obstacles", 128,NULL, 3, NULL);
	
	
	
	vTaskStartScheduler();
}
