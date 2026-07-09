/*
 * Toaster.c
 *
 * Created: 26.02.2026 09:55:34
 * Author : e4Menzi
 */ 

#include "ucBoardDriver.h"

#define MASKE_ON_OFF_SCHALTER (1<<7)
#define MASKE_DREHSCHALTER (0b1111)
#define MASKE_START_HEBEL (1<<1)
#define MASKE_STOPP_HEBEL (1<<0)

#define MASKE_POWER_LED (7)
#define MASKE_HEBEL (6)
#define MASKE_HEIZELEMENT (5)
#define MASKE_KLAPPE (4)
#define MASKE_HEISS_LED (3)

int main(void)
{
	initBoard(0);
	
	uint8_t schalter = 0;
	uint8_t inOnOffSchalter = 0;
	uint32_t inDrehschalter = 0;
	
	uint8_t taster = 0;
	
	uint8_t inPowerLED = 0;
	
	uint32_t inBackzeit = 0;
	
	uint8_t inFlanke = 0;
	uint8_t inPosFlanke = 0;
	uint8_t inTasterNeu = 0;
	uint8_t inTasterAlt = 0;
	uint8_t inFlankeStart = 0;
	uint8_t inFlankStopp = 0;
	
	uint64_t inZeit_ms = 0;
	uint64_t inTimerBacken = 0;
	uint64_t inTimerKlappe = 0;
	uint64_t intimerHitze = 3000;
	
	uint8_t inHebelEingerastet = 0;
	uint8_t inHeizelement = 0;
	uint8_t inKlappe = 0;
	uint8_t inBackEndeFlag = 1;
	uint8_t inHeiss = 0;
	

	
    while (1) {
//Eingabe-----------------------------------------------------------------------------------------------
	schalter = PINC;
	taster = PINL;
	inOnOffSchalter = (schalter& MASKE_ON_OFF_SCHALTER)>0;
	inDrehschalter = schalter&MASKE_DREHSCHALTER;
	inBackzeit = (inDrehschalter*15+15)*1000;
	inTasterAlt = inTasterNeu;
	inTasterNeu = taster;
	inFlanke = inTasterAlt ^ inTasterNeu;
	inPosFlanke = inFlanke & inTasterNeu;
	inFlankeStart = (inPosFlanke&MASKE_START_HEBEL)>0;
	inFlankStopp = (inPosFlanke&MASKE_STOPP_HEBEL)>0;
	inZeit_ms = getSystemTimeMs();
//Verarbeitung-----------------------------------------------------------------------------------------
	
	if(inOnOffSchalter){
		inPowerLED = 1;
	}
	else{
		inPowerLED = 0;
		inHeizelement = 0;
		inHebelEingerastet = 0;
	}
	if(inFlankeStart&&inOnOffSchalter&&(!inHebelEingerastet)){
		inTimerBacken = inZeit_ms;
		inHebelEingerastet = 1;
		inHeizelement = 1;
		inBackEndeFlag = 0;
		inHeiss = 1;
	}
	if(((inZeit_ms-inTimerBacken)>=inBackzeit&&(!inBackEndeFlag))|(inFlankStopp&&inHeizelement)){//ODER
		inHebelEingerastet = 0;
		inHeizelement = 0;
		inKlappe = 1;
		inTimerKlappe = inZeit_ms;
		inBackEndeFlag = 1;
		intimerHitze = inZeit_ms;
	}
	if(((inZeit_ms-inTimerKlappe)<550)&&((inZeit_ms-inTimerKlappe)>500)){
		inKlappe = 0;
	}
	if(((inZeit_ms-intimerHitze)<180050)&&((inZeit_ms-intimerHitze)>180000)){
		inHeiss = 0;
	}
	
//Ausgabe-----------------------------------------------------------------------------------------------
	PORTA = (inPowerLED << MASKE_POWER_LED) | (inHebelEingerastet << MASKE_HEBEL) | (inHeizelement << MASKE_HEIZELEMENT) | (inKlappe << MASKE_KLAPPE) | (inHeiss << MASKE_HEISS_LED);
	
//zusatz---------------------------------------------
	lcdWriteText(1,0,"t:");
	lcdWriteZahl(1,3,inBackzeit/1000,3,0);
	lcdWriteText(1,6,"s");
	lcdWriteText(0,0,"Powwa %u", inPowerLED);
	lcdWriteText(2,0,"T: %u",inHeizelement);
	lcdWriteText(3,0,"Hebel %u",inHebelEingerastet);
	lcdWriteText(0,9,"Klappe %u",inKlappe);
	lcdLight(255);
	lcdWriteText(1,9,"delta");
	lcdWriteZahl(1,15,(inZeit_ms-inTimerBacken)/1000,2,0);
	lcdWriteText(3,9,"heat %u",inHeiss);
    }
}

