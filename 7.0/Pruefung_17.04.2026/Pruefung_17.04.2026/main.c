/*
 * Pruefung_17.04.2026.c
 *
 * Created: 17.04.2026 07:45:21
 * Author : e4Menzi
 */ 

#include "ucBoardDriver.h"

#define LED_LOW (PORTA)
#define TASTER (PINL)
#define SCHALTER (PINC)
#define LICHTSCHRANKEN_SCHALTER (0b00000001)
#define TASTER_NULL_UND_EINS (0b00000011)
#define  TASTER_EINFAHRT (0b00000001)
#define TASTER_AUSFAHRT (0b00000010)

#define TOR_GESCHLOSSEN (0b00010000)
#define TOR_OFFEN (0)
#define EINFAHRT_ROT (1)
#define AUSFAHRT_ROT (3)
#define AUSFAHRT_GRUEN (2)
#define AUSGESCHALTET (0)

#define TIMER_DURCHFAHRT (5000)
#define LICHTSCHRANKENTIMER (10000)

int main(void)
{
	uint8_t inLichtschranke = 0;
	uint8_t inEinfahrtGruen = 0;
	uint8_t inEinfahrtRot = 0;
	uint8_t inAusfahrtGruen = 0;
	uint8_t inAusfahrtRot = 0;
	
	uint8_t taster = 0;
	uint8_t taster_neu = 0;
	uint8_t taster_alt = 0;
	uint8_t taster_flanke = 0;
	uint8_t taster_posflanke = 0;
	uint8_t inEinfahrt = 0;
	uint8_t inAusfahrt = 0;
	
	uint8_t tor = 0;
	uint8_t entry = 1;
	
	uint64_t durchfahrt = 0;
	uint64_t inZeit_ms = 0;
	uint64_t lichtschranken_timer = 0;
	uint64_t blinktimer = 0;
	
	typedef enum {Normalbetrieb,Einfahrt,Ausfart,Lichtschranke_unterbrochen,Fehler}zustand_t;
	zustand_t zustand = Normalbetrieb;
	
	initBoard(0);

    while (1){
//Eingabe----------------------------------------------------------------------------------------------------------------
	inLichtschranke = (SCHALTER & LICHTSCHRANKEN_SCHALTER)>0;
	taster = TASTER & TASTER_NULL_UND_EINS;
	taster_alt = taster_neu;
	taster_neu = taster;
	taster_flanke = taster_alt ^ taster_neu;
	taster_posflanke = taster_flanke & taster_neu;
	inEinfahrt = (taster_posflanke & TASTER_EINFAHRT) >0;
	inAusfahrt = (taster_posflanke & TASTER_AUSFAHRT) >0;
	inZeit_ms = getSystemTimeMs();
//Verarbeitung----------------------------------------------------------------------------------------------------------
	switch (zustand){
		case Normalbetrieb:
			tor = TOR_GESCHLOSSEN;
			inEinfahrtRot = 1;
			inAusfahrtRot = 1;
			inEinfahrtGruen = 0;
			inAusfahrtGruen = 0;
			if(inEinfahrt){
				zustand = Einfahrt;
			}
			if(inAusfahrt){
				zustand = Ausfart;
			}
		break;
		
		case Einfahrt:
			if(entry){
				durchfahrt = inZeit_ms;
				entry = 0;
			}
			tor = TOR_OFFEN;
			inEinfahrtRot = 0;
			inEinfahrtGruen = 1;
			if((inZeit_ms-durchfahrt)>TIMER_DURCHFAHRT){
				entry = 1;
				zustand = Normalbetrieb;
			}
			if(inLichtschranke){
				zustand = Lichtschranke_unterbrochen;
				entry = 1;
			}
		break;
		
		case Ausfart:
			if(entry){
				durchfahrt = inZeit_ms;
				entry = 0;
			}
			tor = TOR_OFFEN;
			inAusfahrtRot = 0;
			inAusfahrtGruen = 1;
			if((inZeit_ms-durchfahrt)>TIMER_DURCHFAHRT){
				entry = 1;
				zustand = Normalbetrieb;
			}
			if(inLichtschranke){
				zustand = Lichtschranke_unterbrochen;
				entry = 1;
			}
		break;
		
		case Lichtschranke_unterbrochen:
			if(entry){
				lichtschranken_timer = inZeit_ms;
				entry = 0;
			}
			if(!inLichtschranke){
				entry = 1;
				zustand = Normalbetrieb;
			}
			if((inZeit_ms-lichtschranken_timer)>LICHTSCHRANKENTIMER){
				entry = 1;
				zustand = Fehler;
			}
		break;
		
		case Fehler:
			if(entry){
				blinktimer = inZeit_ms;
				entry = 0;
				inEinfahrtGruen = 1;
				inEinfahrtRot = 1;
				inAusfahrtGruen = 1;
				inAusfahrtRot = 1;
			}
			if((inZeit_ms-blinktimer)>500){
				inEinfahrtGruen = (inEinfahrtRot ^ 1);
				inEinfahrtRot = (inEinfahrtRot ^ 1);
				inAusfahrtGruen = (inAusfahrtGruen ^ 1);
				inAusfahrtRot = (inAusfahrtRot ^ 1);
				blinktimer = inZeit_ms;
			}
			if(!inLichtschranke){
				entry = 1;
				zustand = Normalbetrieb;
			}
		break;
	}
//Ausgabe---------------------------------------------------------------------------------------------------------------
	LED_LOW = tor | (inEinfahrtRot<<EINFAHRT_ROT) | inEinfahrtGruen | (inAusfahrtRot<<AUSFAHRT_ROT) | (inAusfahrtGruen<<AUSFAHRT_GRUEN);
    }
}
