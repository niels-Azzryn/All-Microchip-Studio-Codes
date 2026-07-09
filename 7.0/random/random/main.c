/*
 * random.c
 *
 * Created: 11.12.2025 10:20:14
 * Author : e4Menzi
 */ 

#include <avr/io.h>
#include <time.h>
#define F_CPU 16000000UL
#include <util/delay.h>

int main(void)
{
	DDRL = 0x00;
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRC = 0x00;
	uint8_t inschalter_c								= 0;
	uint8_t inschalter_l								= 0;
	uint8_t welche_maschine						= 1;
	uint8_t wasserkocher_onoff_schalter = 0;
	uint8_t delay_counter							= 0;
	uint8_t temperatur_wasserkocher		= 0;
	
	
    while (1) 
    {
			
		if (PINL & (1<<7)){
			welche_maschine = ++welche_maschine;
			_delay_ms(200);
		}
		if(!(welche_maschine<3)){
			welche_maschine = 1;
		}
		inschalter_l = PINL;
		inschalter_c = PINC;
		PORTB = welche_maschine;
		
		
		
		
		
		switch (welche_maschine){
			case 1: //Wasserkocher------------------------------------------
//Eingabe
	wasserkocher_onoff_schalter = (inschalter_l&(1<<1));
	temperatur_wasserkocher = (inschalter_c&(11<<0));
	
//Verarbeitung
	delay_counter = 2000 *temperatur_wasserkocher ;
	if(wasserkocher_onoff_schalter>0){
		PORTA|= 1;
		_delay_ms(2000);
		PORTA&=!1;
	}
//Ausgabe

		PORTA = temperatur_wasserkocher<<1;
			break;
	}

	}
}

