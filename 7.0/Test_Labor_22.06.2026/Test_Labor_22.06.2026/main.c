/*
 * Test_Labor_22.06.2026.c
 *
 * Created: 22.06.2026 16:05:35
 * Author : e4Menzi
 */ 

#include "ucBoardDriver.h"


int main(void)
{
	initBoard(0);
	DDRL = 0b00001000;
	uint16_t kondi_spannung = 0;
    while (1) {
		kondi_spannung =	adcRead(ADC_00_X4_PORTF_BIT0);
		lcdWriteZahl(0,0,kondi_spannung,19,0);
    }
}

