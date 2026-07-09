/*
 * Snack_Automat.c
 *
 * Created: 13.11.2025 15:45:59
 * Author : e4Menzi
 */ 

#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 16000000UL

int main(void)
{
uint8_t state = 0;
uint8_t ausgabeklappe = 0;
uint8_t spirale = 1;
uint8_t blinklicht = 255;
uint8_t auswahl = 0;

DDRA = 0xFF;
DDRC = 0x00;
DDRB = 0xFF;
DDRH = 0x10;

    while (1) 
    {
		switch(state){
			case 0: //auswahl wird getroffen und bezahlt
			PORTA = (PINC&0b111)<<5;
			PORTH = (1<<5);
			if((PINC&128)&&){
				PORTH = (1<<4);
				state=++state;
			}
			PORTA = 0x00;
			auswahl = (PINC&0b111)<<5;
			break;
			
			case 1://Produkt wird ausgegeben
			PORTA = auswahl;
			spirale = spirale<<1;
			PORTB = spirale;
			_delay_ms(13000);
			if (spirale==128)
			{
				ausgabeklappe=1;
				state=++state;
			}
			break;
			
			case 2://entnahme 
			PORTH = (1<<5);
			PORTA = auswahl;
			PORTB =blinklicht;
			_delay_ms(13000);
			blinklicht=~blinklicht;
			if(PINC&32){
				state=++state;
			}
			break;
			
			case 3: // restart
			PORTA = 255;
			PORTB = 255;
			_delay_ms(20000);
			PORTA = 0;
			PORTB = 0;
			PORTH = 0;
			if(PINC==0){
			state=0;
			spirale=1;
			}
			break;
		}
	}
}

