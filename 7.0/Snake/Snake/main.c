/*
 * Snake.c
 *
 * Created: 20.05.2026 16:11:00
 * Author : e4Menzi
 */ 

#include "ucBoardDriver.h"
#define SPEED (500)

int main(void){
	initBoard(0);
//time
	uint64_t inZeit_ms = 0;
//joystick
	uint16_t x = 0;
	uint16_t y = 0;
	uint8_t flag_right = 0;
	uint8_t flag_left = 0;
	uint8_t flag_up = 0;
	uint8_t flag_down = 0;
	
//snake Head
	uint8_t y_axis = 5;
	uint8_t x_axis = 9;
	
//movement
	uint8_t right = 0;
	uint8_t left = 0;
	uint8_t up = 0;
	uint8_t down = 0;
	uint64_t movement_timer = 0;	
	uint8_t y_axis_alt = 0;
	uint8_t x_axis_alt = 0;
	
//death
	uint8_t death = 0;
//fruit
	uint8_t fruit_flag = 0;
	uint8_t random_x_axis = 0;
	uint8_t random_y_axis = 1;
//body
	uint8_t length = 0;
	uint8_t position_one_y = 0;
	uint8_t position_one_x = 0;

	
	int *wavelenght_x [] = {&x_axis_alt,&position_one_x};
	int *wavelenght_y [] = {&y_axis_alt,&position_one_y};
    while (1) {
//time
	inZeit_ms =getSystemTimeMs();
	
//joystick	
	x = adcRead(ADC_10_JOYSTICK_X);
	y = adcRead(ADC_11_JOYSTICK_Y);
	if((x<200)&&flag_right&&(!left)){
		flag_right -= 1;
		right = 1;
		left = 0;
		up = 0;
		down = 0;
	}
	if((x>823)&flag_left&&(!right)){
		flag_left -= 1;
		right = 0;
		left = 1;
		up = 0;
		down = 0;
	}
	if((y<200)&&flag_down&&(!up)){
		flag_down -= 1; 
		right = 0;
		left = 0;
		up = 0;
		down = 1;
	}
	if((y>800)&&flag_up&&(!down)){
		flag_up -= 1;
		right = 0;
		left = 0;
		up = 1;
		down = 0;
	}
	if((x>300)&&(x<600)){
		flag_left = 1;
		flag_right = 1;
	}
	if((y>300)&&(y<600)){
		flag_up = 1;
		flag_down = 1;
	}
	
//movement
	if(((inZeit_ms-movement_timer)>SPEED)&&right&&(!death)){
		movement_timer = inZeit_ms;
		y_axis_alt = y_axis;
		x_axis_alt = x_axis;
		x_axis += 1;
	}
	if(((inZeit_ms-movement_timer)>SPEED)&&left&&(!death)){
		movement_timer = inZeit_ms;
		y_axis_alt = y_axis;
		x_axis_alt = x_axis;
		x_axis -= 1;
	}
	if(((inZeit_ms-movement_timer)>SPEED)&&up&&(!death)){
		movement_timer = inZeit_ms;
		y_axis_alt = y_axis;
		x_axis_alt = x_axis;
		y_axis -= 1;
	}
	if(((inZeit_ms-movement_timer)>SPEED)&&down&&(!death)){
		movement_timer = inZeit_ms;
		y_axis_alt = y_axis;
		x_axis_alt = x_axis;
		y_axis += 1;
	}
	
//death
	if(y_axis<4 | y_axis>7|x_axis>19 ){
		death = 1;
	}
	if(death==1){
		lcdWriteText(0,0,"                    ");
		lcdWriteText(1,0,"                    ");
		lcdWriteText(2,0,"                    ");
		lcdWriteText(3,0,"                    ");
		lcdWriteText(1,5,"You Lost");
		death = 2;
	}
//Fruits
	if(!fruit_flag){
		random_x_axis = (rand() *adcRead(ADC_01_X4_PORTF_BIT1) * adcRead(ADC_13_TEMPERATUR) % (19+ 1));
		random_y_axis = (rand() * adcRead(ADC_01_X4_PORTF_BIT1)* adcRead(ADC_13_TEMPERATUR) % (7 - 4 + 1))+4;
		lcdWriteText(random_y_axis,random_x_axis,"O");
		fruit_flag = 1;
	}
	if((random_y_axis==y_axis)&&(random_x_axis==x_axis)){
		fruit_flag = 0;
		length += 1;
	}
//Head
if(!death){
	lcdWriteText(y_axis,x_axis,"X");
	if(length>0){
	lcdWriteText(position_one_y,position_one_x,"x");
	}
//delete
	lcdWriteText(*wavelenght_y[length],*wavelenght_x[length]," ");
//body
position_one_x = x_axis_alt;
position_one_y = y_axis_alt;

//light
	lcdLight(255);
}
PORTA = random_x_axis;
PORTB = random_y_axis;

}
}

