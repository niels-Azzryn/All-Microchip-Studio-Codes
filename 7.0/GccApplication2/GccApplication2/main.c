/*
 * GccApplication2.c
 *
 * Created: 12.11.2025 14:43:36
 * Author : e4Menzi
 */ 
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#define DDR_SCHALTER	(DDRC)	
#define DDR_LED_A		(DDRA)
#define DDR_LED_B		(DDRB)
#define ALLES_EINFANG	(0x00)
#define ALLES_AUSGANG	(0xFF)
#define PORT_LED_A		(PORTA)
#define PIN_SCHALTER	(PINC)
#define MASKE_EIN_S3	(1<<7)
#define MASKE_AUS_S3	(0b01111111)
#define MASKE_EIN_S2	(0b10111111)
#define MASKE_AUS_S2	(1<<6)
#define MASKE_EIN_S1	(0b00000011)
#define MASKE_AUS_S1	(0b11111100)
#define SCHALTER_3_WERT	(8)
#define SCHALTER_2_WERT	(4)
#define SCHALTER_1_WERT	(2)
int main(void)
{
	DDR_SCHALTER = ALLES_EINFANG;
	DDR_LED_A = ALLES_AUSGANG;
	DDR_LED_B = ALLES_AUSGANG;
	PORT_LED_A = ALLES_AUSGANG;
	while(1){
		if(PIN_SCHALTER&SCHALTER_3_WERT){
			PORT_LED_A |= MASKE_EIN_S3;
		}
		else{
			PORT_LED_A &= MASKE_AUS_S3;
		}
		if(PIN_SCHALTER&SCHALTER_2_WERT){
			PORT_LED_A &= MASKE_EIN_S2;
		}
		else{
			PORT_LED_A |= MASKE_AUS_S2;
		}
		if(PIN_SCHALTER&SCHALTER_1_WERT){
			PORT_LED_A |= MASKE_EIN_S1;
			}
		else{
			PORT_LED_A &= MASKE_AUS_S1;
			}
	}
	
}

