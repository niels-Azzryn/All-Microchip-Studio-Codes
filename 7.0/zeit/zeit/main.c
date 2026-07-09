/*
 * zeit.c
 *
 * Created: 11.12.2025 12:56:25
 * Author : e4Menzi
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

#define DDR_LED_LOW_BYTE		(DDRA)
#define DDR_RGB_LED					(DDRH)
#define DDR_SCHALTER					(DDRC)

#define ALLES_AUSGANG				(0xFF)
#define ALLES_EINGANG					(0x00)

#define LED_LOW_BYTE					(PORTA)
#define LED_RGB								(PORTH)	
#define SCHALTER								(PINC)			

#define MASKE_BELEUCHTUNG							(1<<0)
#define MASKE_TEAMPERATURE_HIGH			(1<<7)
#define MASKE_TEAMPERATURE_MEDIUM	(1<<6)
#define MASKE_TEAMPERATURE_LOW			(1<<5)
#define MASKE_RGB_RED										(1<<3)
#define MASKE_RGB_GREEN									(1<<4)
#define MASKE_RGB_BLUE									(1<<5)
#define MASKE_TIME_SELECTION						(0b00011110)
#define MASKE_SUMMER_LED								(1<<1)
#define MASKE_ROTATIONSMOTOR						(1<<2)

#define TAKT  (100)

int main(void)
{
	DDR_LED_LOW_BYTE	= ALLES_AUSGANG;
	DDR_RGB_LED					= ALLES_AUSGANG;
	DDR_SCHALTER				= ALLES_EINGANG;
	DDRB = 0xFF;
	
	uint8_t inschalter						= 0;
	uint8_t door_sensor					= 0;
	uint8_t high_temperature		= 0;
	uint8_t medium_temperature	= 0;
	uint8_t low_temperature			= 0;
	
	uint8_t beleuchtung					= 0;
	uint8_t temperature_rgb			= 0;
	uint8_t summer							= 0;
	
	uint8_t rotationmotor				= 0;
	uint8_t zustand							= 1;
	uint8_t temperatur						= 0;
	uint8_t zeitspeicher_schalter	= 0;
	uint16_t  zeitspeicher				= 0;
	uint8_t zeitspeicher_anzeige	= 0;
	uint8_t timer								= 0;
	
    while (1) 
    {
	inschalter = SCHALTER;

//Eingabe
	door_sensor					= (inschalter & MASKE_BELEUCHTUNG)>0;
	high_temperature			= (inschalter & MASKE_TEAMPERATURE_HIGH)>0;
	medium_temperature		= (inschalter & MASKE_TEAMPERATURE_MEDIUM)>0;
	low_temperature			= (inschalter & MASKE_TEAMPERATURE_LOW)>0;
	zeitspeicher_schalter		= (inschalter & MASKE_TIME_SELECTION);
	
//Verarbeitung
	temperatur = high_temperature | medium_temperature | low_temperature;
	
switch(zustand){
	case 1:
		beleuchtung = door_sensor<<7;
		
		if((door_sensor)&&(temperatur)&&(zeitspeicher_schalter>0)){
			zeitspeicher = ((zeitspeicher_schalter>>1) * 1000)+1000;
			zeitspeicher_anzeige = zeitspeicher / 1000;
			rotationmotor = MASKE_ROTATIONSMOTOR;
			
			if(high_temperature){
				temperature_rgb = MASKE_RGB_RED;
						zustand = zustand + 1;
			}
			
			else if (medium_temperature){
				temperature_rgb = MASKE_RGB_GREEN;
						zustand = zustand + 1;
			}
			
			else if(low_temperature){
				temperature_rgb = MASKE_RGB_BLUE;
						zustand = zustand + 1;
			}
		}
	break;
	
	case 2:
		beleuchtung = door_sensor<<7;
		if(door_sensor){
			zeitspeicher = zeitspeicher - 1;
		}
		zeitspeicher_anzeige = (zeitspeicher) / 1000;
		
		if(zeitspeicher==1000){
			temperature_rgb = 0;
			rotationmotor = 0;
		}		
		if(!zeitspeicher){
			zustand = zustand + 1;
		}
	break;
	case 3:
		timer = timer + 1;
		if(timer % 2){
			summer = MASKE_SUMMER_LED;
		}
		else{
			summer = 0;
		}
		
		if(!door_sensor){
			zustand = 1;
		}
		_delay_ms(499);
	break;
}

//Ausgabe
		if(door_sensor==1){
			LED_LOW_BYTE = beleuchtung | summer | rotationmotor;
			LED_RGB					= temperature_rgb;
		}
		else{
			LED_LOW_BYTE = 0;
			LED_RGB = 0;
		}
		PORTB = zeitspeicher_anzeige;
		
		// Verzögerung
		_delay_ms(1);
    }
}