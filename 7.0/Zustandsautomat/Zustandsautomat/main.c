/*
 * Zustandsautomat.c
 *
 * Created: 05.03.2026 11:31:56
 * Author : e4Menzi
 */ 

#include "ucBoardDriver.h"

#define MASKE_EINAUS_TASTER (1<<0)
#define MASKE_FARBWECHSEL (1<<6)

#define RGB (PORTH)

#define LIGHT_ON (255)
#define  LIGHT_OFF (0)

int main(void)
{
	uint8_t inTasterAlt = 0;
	uint8_t inTasterNeu = 0;
	uint8_t inFlanke = 0;
	uint8_t inPosFlanke = 0;
	uint8_t taster = 0;
	uint8_t inEinAus = 0;
	uint8_t inFarbwechsel = 0;
	
	uint8_t inRed = 0;
	uint8_t inGreen = 0;
	uint8_t inBlue = 0;
	
	uint8_t flag = 0;
	typedef enum  {AUS,RED, GREEN, BLUE,} zustand_t;
		
	zustand_t  zustand = AUS;
	
	initBoard(0);
    while (1){
//Eingabe
	taster = PINL;
	inTasterAlt = inTasterNeu;
	inTasterNeu = taster;
	inFlanke = inTasterAlt ^ inTasterNeu;
	inPosFlanke = inFlanke & inTasterNeu;
	inEinAus = (inPosFlanke & MASKE_EINAUS_TASTER);
	inFarbwechsel = (inPosFlanke & MASKE_FARBWECHSEL)>0;
	
//Verabeitung		
		switch (zustand){
			case AUS:
			if(inEinAus){
				inRed = LIGHT_ON;
				zustand = RED;
			}
			break;
			
			case RED:
			if(inEinAus){
				zustand = AUS;
				inRed = LIGHT_OFF;
			}
			if(inFarbwechsel){
				inRed = LIGHT_OFF;
				inGreen = LIGHT_ON;
				zustand = GREEN;
			}
			break;
			
			case GREEN:
			if(inEinAus){
				zustand = AUS;
				inGreen = LIGHT_OFF;
			}
			if(inFarbwechsel){
				inGreen = LIGHT_OFF;
				inBlue = LIGHT_ON;
				zustand = BLUE;
			}
			break;
			
			case BLUE:
			if(inEinAus){
				zustand = AUS;
				inBlue = LIGHT_OFF;
			}
			if(inFarbwechsel){
				inBlue = LIGHT_OFF;
				inRed = LIGHT_ON;
				zustand = RED;
			}
			break;
		}
//Ausgabe
	rgbWrite(inRed,inGreen,inBlue);
	}
}


