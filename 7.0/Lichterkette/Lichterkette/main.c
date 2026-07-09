/*
 * Lichterkette.c
 *
 * Created: 12.12.2025 12:53:27
 * Author : e4Menzi
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

#define DDR_SCHALTER					(DDRC)
#define DDR_LED_LOW_BYTE		(DDRA)
#define DDR_LED_HIGH_BYTE	(DDRB)

#define SCHALTER								(PINC)
#define LED_LOW_BYTE					(PORTA)
#define LED_HIGH_BYTE				(PORTB)

#define ALLES_AUSGANG	(0xFF)
#define ALLES_EINGANG		(0x00)

#define MASKE_ONOFF_SWITCH						(1<<0)
#define MASKE_ALLE_LOW_BYTE_LED		(0b11111111)
#define MASKE_ALLE_HIGH_BYTE_LED	(0b11111111)

#define TAKT		(1)

int main(void)
{
	DDR_SCHALTER				= ALLES_EINGANG;
	DDR_LED_LOW_BYTE	= ALLES_AUSGANG;
	
	uint8_t inschalter			= 0;
	uint16_t  system_clock = 0;
	uint16_t start_time		= 0;
	uint16_t time_jetzt		= 0;
	uint16_t vergangen		= 0;
	
	uint8_t onoff_switch = 0;
	
	uint8_t	led_low_byte		= 0;
	uint8_t	led_high_byte	= 0;
	
    while (1) {
		inschalter = SCHALTER;
		
//Eingabe
	onoff_switch = (inschalter & MASKE_ONOFF_SWITCH)>0;

//Verarbeitung
	system_clock	+= TAKT;
	if(!onoff_switch | vergangen>=18000){
		system_clock = 0;
		start_time	= system_clock;
	}
	time_jetzt		= system_clock;
	vergangen		= time_jetzt - start_time;
	if((vergangen <= 6000)&&(vergangen>0)){
		led_high_byte	= MASKE_ALLE_HIGH_BYTE_LED;
		led_low_byte		= MASKE_ALLE_LOW_BYTE_LED;
	}
	else{
		led_high_byte	= 0;
		led_low_byte		= 0;
	}
	
//Ausgabe
	LED_HIGH_BYTE	= led_high_byte;
	LED_LOW_BYTE		= led_low_byte;
//Verzögerung
_delay_ms(TAKT);
    }
}






