/*
 * Schalter_Kombinationen.c
 *
 * Created: 20.11.2025 15:37:46
 * Author : e4Menzi
 */ 

#include <avr/io.h>	
#define DDR_PORT_A		(DDRA)
#define DDR_PORT_B		(DDRB)
#define LED_PORT_A		(PORTA)
#define	SCHALTER		(PINC)
#define ALLE_AUSGANG	(0xFF)

//Masken------------------------
#define	SCHALTER_7		(1<<7)
#define SCHALTER_6		(1<<6)
#define SCHALTER_5		(1<<5)
#define SCHALTER_4		(1<<4)
#define SCHALTER_3		(1<<3)
#define SCHALTER_2		(1<<2)
#define SCHALTER_1		(1<<1)
#define SCHALTER_0		(0<<0)

#define SCHALTER_1BIS3	(SCHALTER_1|SCHALTER_2|SCHALTER_3)
#define SCHALTER_4BIS6	(SCHALTER_4|SCHALTER_5|SCHALTER_6)

#define LED_0			(1<<0)
#define	LED_1			(1<<1)
#define LED_2			(1<<2)
#define LED_3			(1<<3)
#define LED_4			(1<<4)
#define LED_5			(1<<5)
#define LED_6			(1<<6)
#define LED_7			(1<<7)

#define ALLE_LED_AUS	(0<<0)	
//------------------------------

int main(void)
{
	DDR_PORT_A = ALLE_AUSGANG;
	DDR_PORT_B = ALLE_AUSGANG;
	uint8_t led_out = 0;
	uint8_t schalter_6 = 0;
	uint8_t schalter_5 = 0;
	uint8_t schalter_4 = 0;
	uint8_t led_2 = 0;
	

while (1) {
	schalter_6 = (PINC &= SCHALTER_6);
	schalter_5 = (PINC &= SCHALTER_5);
	schalter_4 = (PINC &= SCHALTER_4);
	led_2 = (!schalter_4 || !schalter_5 || !schalter_6);
		led_out	= led_out + led_2;
		led_out = 0;
	/*if(SCHALTER & SCHALTER_7){ 
		led_out = led_out + LED_0;
    }
	if((SCHALTER & SCHALTER_7)&&(SCHALTER & SCHALTER_6)&&((SCHALTER & SCHALTER_5)==0)){
		led_out = led_out + LED_1;
		}
	if(SCHALTER & SCHALTER_1){
		led_out = led_out + LED_3;
	}
	if(SCHALTER&(SCHALTER_2|SCHALTER_3)&&((SCHALTER & SCHALTER_1)==0)){
		led_out = led_out + LED_4;
	}
	if((SCHALTER%17)==0){
		led_out = led_out + LED_5;
	}
	if(((SCHALTER &= SCHALTER_1BIS3)>>1)>((SCHALTER &= SCHALTER_4BIS6)>>4)){
		led_out = led_out + LED_6;
	}
	if(((SCHALTER &= SCHALTER_1BIS3)>>1)==((SCHALTER &= SCHALTER_4BIS6)>>4)){
		led_out = led_out + LED_7;
	}*/
	LED_PORT_A = led_out;
	LED_PORT_A &= ALLE_LED_AUS;
	}
}



