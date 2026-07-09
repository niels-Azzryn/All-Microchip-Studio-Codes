/*
 * Blinklicht_einstellbar.c
 *
 * Created: 26.02.2026 16:07:45
 * Author : e4Menzi
 */ 

#include "ucBoardDriver.h"


int main(void)
{
	uint64_t inZeit_ms = 0;
	uint64_t inTimerEin = 0;
	uint64_t inTimerAus = 0;
	
	uint8_t inOnOffSchalter = 0;
	uint8_t inBlinkFlag = 0;
	uint8_t schalter = 0;
	
	uint8_t inLED = 0;
	
	uint8_t inBlinkFlagStart = 0;
	uint8_t inOnOffFlag = 0;
	
	uint16_t inEin_Zeit = 300;
	uint16_t  inAusZeit = 400;
	initBoard(0);
	
    while (1) {
//Eingabe----------------------------------------------------------
	schalter = PINC;
	inZeit_ms = getSystemTimeMs();
	inOnOffSchalter = (schalter&(1<<7))>0;
	inBlinkFlag = (schalter&(1<<6))>0;
	
//Verarbeitung----------------------------------------------------
	if(inOnOffSchalter){
			if(inBlinkFlag){	
					if((inOnOffFlag&&inBlinkFlagStart)==0){
						inLED = 1;
						inTimerEin = inZeit_ms;
						inBlinkFlagStart = 1;
						inOnOffFlag = 1;
					}
					if((inZeit_ms-inTimerEin)==inEin_Zeit){
						inLED = 0;
						inTimerAus = inZeit_ms;
					}
					if((inZeit_ms-inTimerAus)==inAusZeit){
						inLED = 1;
						inTimerEin = inZeit_ms;
						inLED = 1;
					}
			}
			else{
				inLED = 1;
				inBlinkFlagStart = 0;
			}
	}
	else{
		inLED = 0;
		inOnOffFlag = 0;
	}
	
//Ausgabe----------------------------------------------------------
	PORTA = inLED;
    }
}

