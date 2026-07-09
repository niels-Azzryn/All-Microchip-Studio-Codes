/*
 * Flankenerkennung.c
 *
 * Created: 05.02.2026 08:21:35
 * Author : e4Menzi
 */ 

#include "ucBoardDriver.h"


int main(void)
{
	initBoard(1);
	
	
	uint8_t taster_alt = 0;
	uint8_t taster_neu = 0;
	uint8_t inflanke = 0;
	uint8_t inposflanke = 0;
	uint8_t inposflankeT1 = 0;
	uint8_t innegflanke = 0;
	uint16_t anzahl_besucher = 0;
	uint8_t on_off = 0;
	uint16_t akku = 0xFFFF;
	uint64_t akku_anziege = 0xFFFF;
	uint8_t akku_drain_speed = 1;
	
	uint8_t helligkeits_array_led [8] = {(1<<3),(11<<3),(1011<<2),(0b01011010),(0b10101101),(0b01111110),(0b11101111),(0b11111111)};
	uint8_t helligkeits_array_lcd [8] = {32,63,95,127,159,191,223,255};
	
    while (1) 
    {
		
		akku_anziege = akku;
		lcdWriteText(0,0,"lvl: %u" ,anzahl_besucher,7);
		lcdWriteText(1,0,"akku: %u",akku_anziege,7);
		
		if(((akku<10000)&&(akku>9990))|((akku<1000)&&(akku>990))|((akku<100)&&(akku>90))|((akku<10)&&(akku>0))){
		lcdWriteText(1,6,"     ",0);
		}
		if(akku<11){
			akku = 0;
		}
		
        taster_alt = taster_neu;
        taster_neu = PINL;
        
        inflanke = taster_alt ^ taster_neu;
        inposflanke = inflanke & taster_neu;
        innegflanke = inflanke & taster_alt;
        
        inposflankeT1 = inposflanke & 0b11000011;
		 if ((inposflankeT1&0b0001)&&(on_off==1)){
			 anzahl_besucher = anzahl_besucher+1;
			akku_drain_speed +=1;
		}
		 if (inposflankeT1&0b01000000){
			 on_off = !on_off;
		 }
		if(anzahl_besucher==8){
			anzahl_besucher = 0;
			akku_drain_speed = 1;
		}
		if((akku_drain_speed<3)&&(akku<6540)&&(akku>6530)){
			anzahl_besucher = 3;
		}
		if((akku<6540)&&(anzahl_besucher == 4)){
			anzahl_besucher = 3;
			akku_drain_speed = 4;
		}
		if((on_off==1)&&(akku>0)){
			akku -= akku_drain_speed;
		}
		 if (inposflankeT1&0b10000000){
			akku = 0xFFFF;
		 }
		 if((PINC&0x01)&&(akku<65520)){
			 akku+=15;
		 }
		 if (akku==0){
			 anzahl_besucher = 0;
		 }
		 if(on_off){
			 ledWriteAll(helligkeits_array_led [anzahl_besucher]);
			 lcdWriteText(0,16,"on ",7);
			 lcdLight(helligkeits_array_lcd[anzahl_besucher]);
		 }
		 else{
			 lcdWriteText(0,16,"off,7");
			 lcdLight(0);
			 ledWriteAll(0);
		 }
    }
}

