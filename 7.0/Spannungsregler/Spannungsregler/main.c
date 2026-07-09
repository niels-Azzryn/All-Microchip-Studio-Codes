/*
 * Spannungsregler.c
 *
 * Created: 18.05.2026 13:38:04
 * Author : e4Menzi
 */ 

#include "ucBoardDriver.h"


int main(void)
{
	initBoard(0);
	uint32_t spannung = 0;
	uint32_t poti = 0;
	uint8_t gate = 0;
	uint8_t schalter = 0;
	uint64_t inZeit_ms = 0;
	uint64_t timer = 0;
	DDRL  |= (1<<4);
	lcdWriteText(0,0, "Spannung");
	lcdWriteText(0,13,"mv");
    while (1) {
	spannung =	adcRead(ADC_03_X4_PORTF_BIT3);
	poti = adcRead(ADC_08_POTI_1);
	poti *= 4.89;
	spannung = spannung * 4.89;
	inZeit_ms = getSystemTimeMs();
	if((inZeit_ms-timer)>500){
		timer = inZeit_ms;
		lcdWriteZahl(0,9,spannung,4,0);
	}
	if(spannung<poti){
		gate = 0;
	}
	if(spannung>poti){
		gate = 1;
	}
	PORTL = gate<<4;
    }
}

