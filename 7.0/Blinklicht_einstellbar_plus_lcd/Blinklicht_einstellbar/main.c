/*
 * Blinklicht_einstellbar.c
 *
 * Created: 26.02.2026 16:07:45
 * Author : e4Menzi
 */ 

#include "ucBoardDriver.h"

#define MASEK_LED_EINS (1)
#define MASKE_TASTER_NULL (1<<0)

int main(void)
{
	uint64_t inZeit_ms = 0;
	uint64_t inTimerEinNull = 0;
	uint64_t inTimerAusNull = 0;
	uint64_t inTimerEinEins = 0;
	uint64_t inTimerAusEins = 0;
	uint64_t inDreiSecTimer = 0;
	uint64_t inFuenfSecTimer = 0;
	
	uint8_t inBlinkFlagNull = 0;
	uint8_t inBlinkFlagEins = 0;
	uint8_t schalter = 0;
	uint8_t schalter_zwei_flag = 0;
	
	uint8_t inLED_null = 0;
	uint8_t inLED_eins = 0;
	uint8_t inLED_sieben = 0;
	uint8_t inLED_zwei = 0;
	
	uint8_t inBlinkFlagStartNull = 0;
	uint8_t inBlinkFlagStartEins = 0;
	
	uint16_t inEin_ZeitNull = 600;
	uint16_t  inAusZeitNull = 400;
	uint16_t inEin_ZeitEins = 250;
	uint16_t  inAusZeitEins = 750;
	
	uint8_t inFlanke = 0;
	uint8_t inPosFlanke = 0;
	uint8_t taster = 0;
	uint8_t inTasterNeu = 0;
	uint8_t inTasterAlt = 0;
	uint8_t DreiSecTaster = 0;
	
	initBoard(0);
	
    while (1) {
//Eingabe-----------------------------------------------------------------------------------------
	schalter = PINC;
	taster = PINL;
	inZeit_ms = getSystemTimeMs();
	inBlinkFlagNull = (schalter&(1<<0))>0;
	inBlinkFlagEins = (schalter&(1<<1))>0;
	schalter_zwei_flag = (schalter&(1<<2))>0;
	inTasterAlt = inTasterNeu;
	inTasterNeu = taster;
	inFlanke = inTasterAlt ^ inTasterNeu;
	inPosFlanke = inTasterNeu & inFlanke;
	DreiSecTaster = (inPosFlanke & MASKE_TASTER_NULL)>0;
//Verarbeitung----------------------------------------------------------------------------------
		//AusschaltverzögerungsLED---------------------------
		if(schalter_zwei_flag){
			inLED_zwei = 1;
			inFuenfSecTimer = inZeit_ms;
		}
		if(((inZeit_ms-inFuenfSecTimer)>5000)&&((inZeit_ms-inFuenfSecTimer)<5050)){
			inLED_zwei = 0;
		}
		//3sec LED und Taster------------------------------------
		if(DreiSecTaster){
			inLED_sieben = 1;
			inDreiSecTimer = inZeit_ms;
		}
		if(((inZeit_ms-inDreiSecTimer)>3000)&&((inZeit_ms-inDreiSecTimer)<3050)){
			inLED_sieben = 0;
		}
		//LED 0------------------------------------------------------
		if(inBlinkFlagNull){	
				if((inBlinkFlagStartNull)==0){
					inLED_null = 1;
					inTimerEinNull = inZeit_ms;
					inBlinkFlagStartNull = 1;
			}
				if((inZeit_ms-inTimerEinNull)==inEin_ZeitNull){
					inLED_null = 0;
					inTimerAusNull = inZeit_ms;
				}
				if((inZeit_ms-inTimerAusNull)==inAusZeitNull){
					inLED_null = 1;
					inTimerEinNull = inZeit_ms;
					inLED_null = 1;
				}
		}
		else{
			inLED_null = 0;
			inBlinkFlagStartNull = 0;
		}
		//LED 1----------------------------------------------------
				if(inBlinkFlagEins){
					if((inBlinkFlagStartEins)==0){
						inLED_eins = 1;
						inTimerEinEins = inZeit_ms;
						inBlinkFlagStartEins = 1;
					}
					if((inZeit_ms-inTimerEinEins)==inEin_ZeitEins){
						inLED_eins = 0;
						inTimerAusEins = inZeit_ms;
					}
					if((inZeit_ms-inTimerAusEins)==inAusZeitEins){
						inLED_eins = 1;
						inTimerEinEins = inZeit_ms;
						inLED_eins = 1;
					}
				}
				else{
					inLED_eins = 0;
					inBlinkFlagStartEins = 0;
				}
//Ausgabe----------------------------------------------------------------------------------------
	PORTA = inLED_null | (inLED_eins << MASEK_LED_EINS) | (inLED_sieben << 7) | (inLED_zwei << 2);
    }
}

