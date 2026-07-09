/*
 * SH_Test_11_05_2026.c
 *
 * Created: 11.05.2026 13:22:39
 * Author : e4Menzi
 */ 

#include "ucBoardDriver.h"
#define TASTER_EINS (1<<0)
#define LED_NULL (0)
#define LED_EINS (1)
#define LED_ZWEI (2)
#define MAX_ADC_WERT (1023)
#define EIN_VOLT (204)
#define HUNDERT_PROZENT (10.23)
#define DREI_SEKUNDEN (3000)
#define FORTIFIVE_PERCENT (45)
#define FIFTYFIVE_PERCENT (55)
#define MINIMUM (10)
#define DREI_VOLT (613)
#define MAXIMUM (50)
#define HALF (50)

int main(void){
	//Aufgabe 1---------------------------------------------------------------------
	uint16_t poti_eins = 0;
	uint8_t poti_eins_prozent;
	
	//Aufgabe 2---------------------------------------------------------------------
	uint8_t taster = 0;
	uint8_t taster_eins = 0;
	uint8_t taster_neu = 0;
	uint8_t taster_alt = 0;
	uint8_t taster_flanke = 0;
	uint8_t taster_posflanke = 0;
	uint8_t led_null = 0;
	uint64_t inZeit_ms = 0;
	uint64_t timer = 0;
	
	//Aufgabe 3---------------------------------------------------------------------
	uint8_t led_eins = 0;
	
	//Aufgabe 4---------------------------------------------------------------------
	uint8_t led_zwei = 0;
	
	//Aufgabe 5---------------------------------------------------------------------
	uint16_t poti_zwei = 0;
	uint8_t poti_zwei_ausgabe = 0;
	initBoard(0);
	
    while (1) {
//Eingabe----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//Aufgabe 1---------------------------------------------------------------------
	poti_eins = adcRead(ADC_08_POTI_1);
	lcdWriteZahl(2,0,adcRead(ADC_08_POTI_1),4,0);
	
	//Aufgabe 2---------------------------------------------------------------------
	taster = PINL;
	taster_eins = (taster & TASTER_EINS)>0;
	taster_alt = taster_neu;
	taster_neu = taster_eins;
	taster_flanke = taster_alt ^ taster_neu;
	taster_posflanke = taster_flanke & taster_neu;
	inZeit_ms = getSystemTimeMs();
	
	//Aufgabe 5---------------------------------------------------------------------
	poti_zwei = adcRead(ADC_09_POTI_2);
//Verarbeitung----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Ausgabe 1 & 3---------------------------------------------------------------------
	poti_eins_prozent = poti_eins / HUNDERT_PROZENT;
	
//Aufgabe2---------------------------------------------------------------------
	if(taster_posflanke){
		led_null = 1;
		timer = inZeit_ms;
	}
	if((inZeit_ms-timer)>DREI_SEKUNDEN){
		led_null = 0;
	}
	
	//Aufgabe 3---------------------------------------------------------------------
	if(poti_eins_prozent>HALF){
		led_eins = 1;
	}
	else{
		led_eins = 0;
	}
	
	//Aufgabe 4---------------------------------------------------------------------
	if(poti_eins_prozent>FIFTYFIVE_PERCENT){
		led_zwei = 1;
	}
	else if (poti_eins_prozent<FORTIFIVE_PERCENT){
		led_zwei = 0;
	}
	
	//Aufgabe 5---------------------------------------------------------------------
	if(poti_zwei<EIN_VOLT){
		poti_zwei_ausgabe = MINIMUM;
	}
	else if(poti_zwei>DREI_VOLT){
		poti_zwei_ausgabe = MAXIMUM;
	}
	else{
		poti_zwei_ausgabe = poti_zwei / 10 -10;
	}
//Ausgabe----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	lcdWriteText(0,4,"Prozent");
	lcdWriteZahl(0,0,poti_eins_prozent,3,0);
	ledWriteAll((led_null <<LED_NULL) |(led_eins << LED_EINS) | (led_zwei << LED_ZWEI));
	lcdWriteText(1,0,"Wert:%u",poti_zwei_ausgabe);
    }
}

