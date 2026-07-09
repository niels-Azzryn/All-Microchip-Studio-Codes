/*
 * Loetkolben_TP.c
 *
 * Created: 21.05.2026 10:30:29
 * Author : e4Menzi
 */ 

#include "ucBoardDriver.h"
#define AUS (0)
#define EIN (1)
#define NORMALBETRIEB (0)
#define HEIZEN (1)
#define FEHLERMODUS (2)
#define TEMP_ZU_HOCH (3)

#define ALLE_AUS (0b00000000)
#define STUFE_EINS (0b00000001)
#define STUFE_ZWEI (0b00000011)
#define STUFE_DREI (0b00000010)
#define STUFE_VIER (0b00000110)
#define STUFE_FUENF (0b00000100)
#define STUFE_SECHS (0b00001100)
#define STUFE_SIEBEN (0b00001000)
#define STUFE_ACHT (0b00011000)
#define STUFE_NEUN (0b00010000)
#define STUFE_ZEHN (0b00110000)
#define STUFE_ELF (0b00100000)
#define ALLE_EIN (0b00111111)

#define MASKE_ONOFF (0b10000000)
#define MASKEE_TASTER_UP_DOWN (0b00000011)
#define MASKE_TASTER_UP (0b00000001)
#define MASKE_TASTER_DOWN (0b00000010)

int main(void){
initBoard(0);

uint8_t onOff_zustaende = 0;
uint8_t arbeitszustaende = 0;
uint8_t temp_wert = 0;
uint8_t schalter = 0;
uint8_t onOff_schalter = 0;
uint8_t taster = 0;
uint8_t taster_up = 0;
uint8_t taster_down = 0;
uint8_t taster_up_neu = 0;
uint8_t taster_up_alt = 0;
uint8_t taster_up_flanke = 0;
uint8_t taster_up_posflanke = 0;
uint8_t taster_down_neu = 0;
uint8_t taster_down_alt = 0;
uint8_t taster_down_flanke = 0;
uint8_t taster_down_posflanke = 0;

uint8_t temp_anzeige [13] = {ALLE_AUS,STUFE_EINS,STUFE_ZWEI,STUFE_DREI,STUFE_VIER,STUFE_FUENF,STUFE_SECHS,STUFE_SIEBEN,STUFE_ACHT,STUFE_NEUN,STUFE_ZEHN,STUFE_ELF,ALLE_EIN};
	
    while (1) {
//eingabe
	schalter = PINC;
	onOff_schalter = (schalter & MASKE_ONOFF)>0;
	taster = (PINL & MASKEE_TASTER_UP_DOWN);
	taster_up = (taster & MASKE_TASTER_UP);
	taster_down = (taster & MASKE_TASTER_DOWN);
	
//verarbeitung
switch (onOff_zustaende){
	case AUS:
		arbeitszustaende = NORMALBETRIEB;
		temp_wert = ALLE_AUS;
		if(onOff_schalter){
			temp_wert = 5;
			onOff_zustaende = EIN;
		}
	break;
		case EIN:
			if(!onOff_schalter){
				onOff_zustaende = AUS;
			}
			switch (arbeitszustaende){
			case NORMALBETRIEB:
			break;
			case HEIZEN:
			break;
			case FEHLERMODUS:
			break;
			case TEMP_ZU_HOCH:
			break;
			}
	break;
}
//ausgabe
	ledWriteAll(temp_anzeige[temp_wert]);
	lcdWriteZahl(0,0,onOff_zustaende,1,0);
	lcdWriteZahl(1,0,arbeitszustaende,3,0);
	}
}


