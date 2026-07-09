/*
 * 7_segment.c
 *
 * Created: 06.02.2026 08:56:16
 * Author : e4Menzi
 */ 

#include "ucBoardDriver.h"

#define NULLLL	(0b01111101)
#define EINS			(0b01000100)
#define ZWEI			(0b01111010)
#define DREI			(0b01101110)
#define VIER			(0b01000111)
#define FUENF		(0b00101111)
#define SECHS		(0b00111111)
#define SIEBEN	(0b01100100)
#define ACHT			(0b01111111)
#define NEUN		(0b01101111)
#define A					(0b01110111)
#define B					(0b00011111)
#define C					(0b00011010)
#define D					(0b01011110)
#define E					(0b00111011)
#define F					(0b00110011)
//schieberegister zahlen
#define NULL_SR		(0b10111110)
#define EINS_SR			(0b00100010)
#define ZWEI_SR			(0b01011110)
#define DREI_SR			(0b01110110)
#define VIER_SR			(0b11100010)
#define FUENF_SR		(0b11110100)
#define SECHS_SR		(0b11111100)
#define SIEBEN_SR	(0b00100110)
#define ACHT_SR			(0b11111110)
#define NEUN_SR		(0b11110110)
#define A_SR					(0b11101110)
#define B_SR					(0b11111000)
#define C_SR					(0b01011000)
#define D_SR					(0b01111010)
#define E_SR					(0b11011100)
#define F_SR					(0b11001100)


int main(void)
{
 initBoard(0);
 
 uint8_t zaehler[16] = {NULL_SR,EINS_SR,ZWEI_SR,DREI_SR,VIER_SR,FUENF_SR,SECHS_SR,SIEBEN_SR,ACHT_SR,NEUN_SR,A_SR,B_SR,C_SR,D_SR,E_SR,F_SR};//zähler von 0-15
/* uint8_t zaehler[10] = {0b01111101,0b01111101,0b01100100,0b01111010,0b00111111,0b01100100,0b00111111,0b01100100,0b01101110,0b01111010}//Handynummer*/
 
 uint16_t zahl_schema = NULL_SR;
 uint8_t shifter =0;
 uint64_t time = 0; 
 uint64_t time_stamp = 0;

 
    while (1) {
		time = getSystemTimeMs();
		if((time-time_stamp)==10){
			time_stamp = time;
					if(shifter==8){
						zahl_schema = zaehler[PINC];
						shifter = 0;
					}
		}
		if(shifter<8){
			PORTB = zahl_schema;
			PORTB |=128;
			zahl_schema>>=1;
			shifter+=1;
		}
		else {
			PORTB = 64;
		}
	}
}


