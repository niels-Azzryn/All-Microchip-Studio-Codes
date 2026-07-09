/*
 * Taster_einlesungen.c
 *
 * Created: 04.03.2026 13:07:44
 * Author : e4Menzi
 */ 

#include "ucBoardDriver.h"


int main(void){
	uint8_t single_counter = 0;
	uint8_t double_counter = 0;
	uint8_t flanken_switch = 0;
	uint8_t counter = 0;
	uint8_t lange_gedrueckt_counter = 0;
	uint8_t lang_flanke = 0;
	
	uint32_t sec_abstand_double_click = 300;
	
	uint8_t inFlanke = 0;
	uint8_t inTasterAlt = 0;
	uint8_t inTasterNeu = 0;
	uint8_t taster = 0;
	uint8_t inPosFlanke = 0;
	uint8_t inNegFlanke = 0;
	
	uint64_t inZeit_ms = 0;
	uint64_t double_timer = 0;
	uint64_t lang_timer = 1100;
	
	initBoard(0);
    while (1) {
//Eingabe
	inZeit_ms = getSystemTimeMs();
	taster = PINL;
	inTasterAlt = inTasterNeu;
	inTasterNeu = taster;
	inFlanke = inTasterAlt ^ inTasterNeu;
	inPosFlanke = inFlanke & inTasterNeu;
	inNegFlanke = inFlanke & inTasterAlt;
	
//Verarbeitung
if(inPosFlanke){
	lang_timer = inZeit_ms;
	lang_flanke = 1;
}
if(((inZeit_ms-lang_timer)>1000)&&((inZeit_ms-lang_timer)<1050)&&lang_flanke){
	lange_gedrueckt_counter++;
	lang_flanke = 0;
}
if(inNegFlanke){
	lang_flanke = 0;
}

// 	switch (counter){
// 	case 0:
// 	if(inPosFlanke){
// 		single_counter += 1;
// 		double_timer = inZeit_ms;
// 		counter += 1;
// 	}
// 	break;
// 	case 1:
// 	if(inPosFlanke&&((inZeit_ms-double_timer)>sec_abstand_double_click)){
// 		single_counter += 1;
// 		counter = 0;
// 	}
// 	if(inPosFlanke&&((inZeit_ms-double_timer)<sec_abstand_double_click)){
// 		double_counter += 1;
// 		single_counter += 1;
// 		counter = 0;
// 	}
// 	break;
// 	}
// 	if((((inZeit_ms-lang_timer)>1000)&&((inZeit_ms-lang_timer)<1050))&&(lang_flanke)){
// 		lange_gedrueckt_counter += 1;
// 		lang_flanke = 0;
// 	}
// 	if(inNegFlanke){
// 		lang_timer = 1051;
// 		lang_flanke = 1;
// 	}
// 	if(inPosFlanke&&lang_flanke){
// 		lang_timer = inZeit_ms;
// 	}
//Ausgabe
	lcdLight(255);
	lcdWriteText(0,0,"x1 %u", single_counter);
	lcdWriteText(1,0,"x2 %u", double_counter);
	lcdWriteText(2,0,"lang %u", lange_gedrueckt_counter);
    }
}

