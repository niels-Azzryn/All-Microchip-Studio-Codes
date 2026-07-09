/*
 * Cafemaschine.c
 *
 * Created: 15.04.2026 14:34:34
 * Author : e4Menzi
 */ 

#include "ucBoardDriver.h"

#define ONOFFSCHALTER (0b0001)
#define TASTER (0b11000011)
#define TASTER_EINS (0b00000001)
#define TASTER_ZWEI (0b11000010)
#define TASTER_VIER (0b10000000)


int main(void){
	
	typedef enum {Aus,Ein} zustandonoff_t;
	zustandonoff_t onoff = Aus;
	typedef enum {Aufheizen,Bereit,Bruehen,Reinigen}arbeitszustand_t;
	arbeitszustand_t arbeitszustand = Aufheizen;
	
	uint8_t entryflag = 1;
	
	uint16_t blue = 0;
	uint16_t green = 0;
	uint16_t red = 0;
	
	uint8_t mode = 0;
	uint8_t brightness = 0;
	uint16_t led_anzeige = 0;
	uint8_t reinigung = 0;
	
	uint8_t taster = 0;
	uint8_t taster_neu = 0;
	uint8_t taster_alt = 0;
	uint8_t taster_flanke = 0;
	uint8_t pos_flanke_taster = 0;
	uint8_t pos_flanke_taster_eins = 0;
	uint8_t pos_flanke_taster_zwei = 0;
	uint8_t pos_flanke_taster_vier = 0;
	
	uint16_t wassterstand = 0;
	
	uint64_t blinktimer = 0;
	uint64_t timer = 0;
	uint64_t inZeit_ms = 0;
	
	initBoard(0);
	
    while (1) {
		
//Eingabe-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	onoff = (PINC & ONOFFSCHALTER)>0;
	taster = PINL & TASTER;
	
	inZeit_ms = getSystemTimeMs();
	
	wassterstand = adcRead(ADC_08_POTI_1);
	
	taster_alt = taster_neu;
	taster_neu = taster;
	taster_flanke = taster_alt ^ taster_neu;
	pos_flanke_taster = taster_flanke & taster_neu;
	pos_flanke_taster_eins = (pos_flanke_taster & TASTER_EINS)>0;
	pos_flanke_taster_zwei = (pos_flanke_taster & TASTER_ZWEI)>0;
	pos_flanke_taster_vier = (pos_flanke_taster & TASTER_VIER)>0;
	
//Verarbeitung-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
switch (onoff){
	case Aus:
	blue = 0;
	entryflag = 1;
	red = 0;
	green = 0;
	arbeitszustand = Aufheizen;
	lcdWriteText(0,0,"     ");
	lcdWriteText(3,0,"     ");
	lcdWriteText(3,11,"         ");
	led_anzeige = 0;
	break;
	case Ein:
		switch (arbeitszustand){
			case Aufheizen:
				if(entryflag){
					blinktimer = inZeit_ms;
					timer = inZeit_ms;
					entryflag = 0;
				}
				if((inZeit_ms-blinktimer)>500){
					blue = blue^1*500;
					blinktimer = inZeit_ms;
				}
				if((inZeit_ms-timer)>5000){
					arbeitszustand = Bereit;
					blue = 0;
					entryflag = 1;
				}
			break;
			
			case Bereit:
			lcdWriteText(0,0,"Klein");
			lcdWriteText(3,0,"Gross");
			if(wassterstand>205){
				green = 500;
				red = 0;
			}
			else{
				green = 0;
				red = 500;
			}
			if(pos_flanke_taster_eins&(!red)){
				mode = 1;
				lcdWriteText(0,0,"     ");
				lcdWriteText(3,0,"     ");
				arbeitszustand = Bruehen;
			}
			if(pos_flanke_taster_zwei&(!red)){
				mode = 2;
				lcdWriteText(0,0,"     ");
				lcdWriteText(3,0,"     ");
				arbeitszustand = Bruehen;
			}
			break;
			
			case Bruehen:
			if(entryflag){
				blinktimer = inZeit_ms+500;
				timer = inZeit_ms;
				entryflag = 0;
			}
			if((!brightness)&(green==1)){
				brightness = 1;
			}
			if(brightness&(green==1022)){
				brightness = 0;
			}
			if((!brightness)&(green>0)){
				green -=1;
			}
			if(brightness&(green<1022)){
				green +=1;
			}
			if((inZeit_ms-blinktimer)>500){
			led_anzeige=(led_anzeige<<1)+1;
			blinktimer = inZeit_ms;
			}
			if((inZeit_ms-timer)>4000*mode){
				led_anzeige = 0;
				entryflag = 1;
				reinigung +=1;
				if(reinigung>=5){
					green = 0;
					arbeitszustand = Reinigen;
				}
				else{
					arbeitszustand = Bereit;
				}
			}
			break;
			
			case Reinigen:
			if((inZeit_ms-blinktimer)>700){
				red = red^1*500;
				green = green^1*500;
				blue = blue^1*100;
				blinktimer = inZeit_ms;
			}
			lcdWriteText(3,11,"Abbrechen");
			if(pos_flanke_taster_vier){
				red = 0;
				blue = 0;
				lcdWriteText(3,11,"         ");
				reinigung = 0;
				arbeitszustand = Bereit;
			}
			break;
		}
	break;
}
//Ausgabe-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	rgbWrite(red,green,blue);
	lcdWriteZahl(0,16,green,0,0);
	PORTA = led_anzeige;
	PORTB = led_anzeige>>8;
}
}

