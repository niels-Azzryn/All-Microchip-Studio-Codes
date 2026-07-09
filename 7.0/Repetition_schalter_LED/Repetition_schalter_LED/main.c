/*
 * Repetition_schalter_LED.c
 *
 * Created: 13.11.2025 11:07:01
 * Author : e4Menzi
 */ 

#include <avr/io.h>
#define DDR_LED_A					(DDRA)
#define DDR_LED_B					(DDRB)
#define ALLE_AUSGANG				(0xFF)
#define DDR_SCHALTER				(DDRC)
#define ALLE_EINGANG				(0x00)
#define LED_PORT_A					(PORTA)
#define LED_PORT_B					(PORTB)
#define HAUPTSCHALTER				(PINC&1)
#define SCHALTER_UNTEN				(PINC&2)
#define SCHALTER_OBEN				(PINC&4)
#define NOTSCHALTER					(PINC&128)
#define SCHALTER_UNTEN_EIN			(0b111111)
#define SCHALTER_UNTEN_AUS			(0b11<<6)
#define SCHALTER_OBEN_EIN_PORT_A	(0b11<<6)
#define SCHALTER_OBEN_EIN_PORT_B	(0b1111)
#define SCHALTER_OBEN_AUS_PORT_A	(0b111111)
#define SCHALTER_OBEN_AUS_PORT_B	(0b1111<<4)
#define NOTSCHALTER_EIN				(0b1111<<12)
#define NOTSCHALTER_AUS				(0b1111)
#define SCHALTER_OBEN_16BIT			(0b111111<<6)
int main(void)
{
	DDR_SCHALTER = ALLE_EINGANG;
	DDR_LED_A = ALLE_AUSGANG;
	DDR_LED_B = ALLE_AUSGANG;
	
	uint8_t	 Hauptschalter		= 0;
	uint8_t	 Schalter_Unten		= 0;
	uint8_t	 Schalter_Oben		= 0;
	uint8_t	 Notschalter		= 0;
	uint8_t	 LED_Low			= 0;
	uint8_t	 LED_High			= 0;
	uint8_t  LED_Notlicht		= 0;
	uint16_t Ausgabe			= 0;	
			
    while (1) 
    {
		//eingabe
		Hauptschalter	= HAUPTSCHALTER		> 0;
		Schalter_Unten	= SCHALTER_UNTEN	> 0;
		Schalter_Oben	= SCHALTER_OBEN		> 0;
		Notschalter		= NOTSCHALTER		> 0;
		
		
		//verarbeitung
		if (Hauptschalter)
		{
			LED_Low		 = Schalter_Unten;
			LED_High	 = Schalter_Oben;
		}
		else{
			LED_High	 = 0;
			LED_Low		 = 0;
		}
			LED_Notlicht = Notschalter;
			
		//ausgabe
		Ausgabe = ( (LED_Low	* SCHALTER_UNTEN_EIN)
				| (LED_High		* SCHALTER_OBEN_16BIT)
				| (LED_Notlicht * NOTSCHALTER_EIN));
				
		PORTA = Ausgabe;
		PORTB = Ausgabe>>8;
	}
}

