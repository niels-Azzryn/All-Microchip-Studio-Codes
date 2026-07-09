/*
 * Labor_Test_Schulzimmer.c
 *
 * Created: 08.12.2025 13:44:57
 * Author : e4Menzi
 */ 

#include <avr/io.h>
#define DDR_SCHALTER			(DDRC)
#define DDR_LED_LOW_BYTE		(DDRA)
#define DDR_LED_HIGH_BYTE		(DDRB)
#define ALLE_EINGANG			(0x00)
#define ALLE_AUSGANG			(0xFF)
#define SCHALTER				(PINC)
#define EINGANG_LED_HIGH_BYTE	(PINB)
#define LED_LOW_BYTE			(PORTA)	
#define LED_HIGH_BYTE			(PORTB)

#define LED_SECHS				(1<<6)

#define MASKE_SCHALTER_NULL		(1<<0)
#define MASKE_SCHALTER_EINS		(1<<1)
#define MASKE_SCHALTER_ZWEI		(1<<2)
#define MASKE_SCHALTER_DREI		(1<<3)

int main(void)
{
	DDR_SCHALTER=ALLE_EINGANG;
	DDR_LED_LOW_BYTE=ALLE_AUSGANG;
	DDR_LED_HIGH_BYTE=ALLE_AUSGANG;
	
	uint8_t inschalter								= 0;
	uint8_t lichtschalter							= 0;
	uint8_t praesentations_modus					= 0;
	uint8_t bitte_nicht_stoeren						= 0;
	uint8_t stromversorgungs_schalter_lehrerpult	= 0;
	uint8_t stromversorgung_schalter_labortisch		= 0;
	uint8_t not_aus									= 0;
	
	uint8_t notaus_speicher							= 0;
	
	uint8_t led_klasse								= 0;
	uint8_t led_lehrer								= 0;
	uint8_t led_bitte_nicht_stoeren					= 0;
	uint8_t led_labortischstrom						= 0;
	uint8_t led_notaus_rot							= 0;
	uint8_t led_notaus_orange						= 0;
	
    while (1) 
    {
//Eingabe
	inschalter								= SCHALTER;
	
	lichtschalter							= inschalter & MASKE_SCHALTER_NULL;
	praesentations_modus					= inschalter & MASKE_SCHALTER_EINS;
	bitte_nicht_stoeren						= inschalter & MASKE_SCHALTER_ZWEI;
	stromversorgungs_schalter_lehrerpult	= inschalter & MASKE_SCHALTER_DREI;
	
	stromversorgung_schalter_labortisch		= EINGANG_LED_HIGH_BYTE & MASKE_SCHALTER_ZWEI;
	not_aus									= EINGANG_LED_HIGH_BYTE & MASKE_SCHALTER_DREI;
	
//Verarbeitung
	led_klasse = lichtschalter<<7;
	
	if((praesentations_modus==0)&&(lichtschalter==1)){
		led_lehrer = LED_SECHS;
	}
	else{
		led_lehrer = 0;
	}
	
	led_bitte_nicht_stoeren = bitte_nicht_stoeren>>2;
	led_labortischstrom = stromversorgungs_schalter_lehrerpult>>2;
	led_labortischstrom|= stromversorgung_schalter_labortisch>>1;
	
	if(!not_aus){
		notaus_speicher = 1;
	}
	
	led_notaus_rot = not_aus<<1;
	led_notaus_orange = notaus_speicher<<5;
//Ausgabe
	LED_LOW_BYTE = (led_klasse|led_lehrer);
	LED_HIGH_BYTE = led_bitte_nicht_stoeren|led_labortischstrom;
	LED_HIGH_BYTE|=led_notaus_rot|led_notaus_orange;
    }
}

