/*
 * driver_test.c
 *
 * Created: 26.01.2026 09:49:03
 * Author : e4Menzi
 */ 
#include "ucBoardDriver.h"


int main(void)
{
	initBoard(1);
	
	#define JOYSTICK_GESCHWINDIGKEIT (200)
	
	uint16_t wertX = 0;
	uint16_t wertY = 0;
	
	uint64_t zeit_ms = 0;
	uint64_t zeit_s = 0;
	
	uint8_t position_zeilen_lcd = 0;
	
	
	uint16_t game_position = 0;
	
	uint8_t cursor_position_y = 0;
	uint8_t cursor_position_x = 0;
	uint8_t cursor_position_y_old = 0;
	uint8_t cursor_position_x_old = 0;
	uint64_t joystick_counter = 101;
	
    while (1) 
    {
	lcdLight(255);
		
	zeit_ms = getSystemTimeMs();
	zeit_s = zeit_ms/1000;
	
	
	
	//joystick----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	wertX = adcRead(ADC_10_JOYSTICK_X);
	wertY = adcRead(ADC_11_JOYSTICK_Y);
	if(((wertX/100)<3)&&(joystick_counter>JOYSTICK_GESCHWINDIGKEIT)){
		cursor_position_x_old = cursor_position_x;
		cursor_position_y_old = cursor_position_y;
		cursor_position_x = cursor_position_x+1;
		joystick_counter = 0;
		lcdWriteText(cursor_position_y_old,cursor_position_x_old," ",7);
	}
	if(((wertX/100)>6)&&(joystick_counter>JOYSTICK_GESCHWINDIGKEIT)){
		cursor_position_x_old = cursor_position_x;
		cursor_position_y_old = cursor_position_y;
		cursor_position_x = cursor_position_x-1;
		joystick_counter = 0;
		lcdWriteText(cursor_position_y_old,cursor_position_x_old," ",7);
	}
	if(((wertY/100)<3)&&(joystick_counter>JOYSTICK_GESCHWINDIGKEIT)){
		cursor_position_x_old = cursor_position_x;
		cursor_position_y_old = cursor_position_y;
		cursor_position_y = cursor_position_y+1;
		joystick_counter = 0;
		lcdWriteText(cursor_position_y_old,cursor_position_x_old," ",7);
	}
	if(((wertY/100)>6)&&(joystick_counter>JOYSTICK_GESCHWINDIGKEIT)){
		cursor_position_x_old = cursor_position_x;
		cursor_position_y_old = cursor_position_y;
		cursor_position_y = cursor_position_y-1;
		joystick_counter = 0;
		lcdWriteText(cursor_position_y_old,cursor_position_x_old," ",7);
	}
	//counter--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	joystick_counter = joystick_counter +1;

	
	//cursor--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	lcdWriteText(cursor_position_y,cursor_position_x,"X",7);
	
	
	switch (game_position){
	case 0: //start Screen--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	lcdWriteText(1,7,"Start",1);
	if(((cursor_position_x==7)|(cursor_position_x==8)|(cursor_position_x==10)|(cursor_position_x==11))&&(cursor_position_y==1)){
		cursor_position_x = 9;	
	}	
	break;
	}

}
}


