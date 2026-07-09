/*
 * CoS_name_list.c
 *
 * Created: 05.02.2026 15:32:15
 * Author : e4Menzi
 */ 

#include "ucBoardDriver.h"

int main(void)
{
	initBoard(1);	
	int8_t position = 1;
	
	char *names [] = {"Arrainoa","athulyt","quetzelkoatl","korathos","caldonrettrus","hypothermia"};
			
    while (1) {
		lcdWriteText(0,0,names[position-1]);
		lcdWriteText(1,0,names[position-2]);
		lcdWriteText(2,0,names[position-3]);
		lcdWriteText(3,0,names[position-4]);
	
	}
}


