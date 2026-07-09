/*
 * Powerbank.c
 *
 * Created: 21.11.2025 13:35:17
 * Author : e4Menzi
 */ 

#include <avr/io.h>

#define DDR_PORT_A		(DDRA)
#define DDR_PORT_B		(DDRB)
#define DDR_PIN_C		(DDRC)

#define SCHALTER		(PINC)
#define LED_LOW_BYTE	(PORTA)

#define ALLE_AUSGANG	(0xFF)
#define ALLE_EINGANG	(0x00)
#define ALLE_AUS		(0)

#define AKKU_VOLL		(15)

//Masken-----------------------------
	// Schalter
#define SCHALTER_7		(1<<7)
#define SCHALTER_6		(1<<6)
#define SCHALTER_5		(1<<5)
#define SCHALTER_4		(1<<4)
#define SCHALTER_3		(1<<3)
#define SCHALTER_1		(1<<1)
#define SCHALTER_0		(1<<0)
#define SCHALTER_6bis3	(1111<<3)

	//LEDs
#define LED_6			(1<<6)
#define LED_5			(1<<5)
#define LED_4			(1<<4)


int main(void)
{
	DDR_PORT_A	= ALLE_AUSGANG;
	DDR_PORT_B	= ALLE_AUSGANG;
	DDR_PIN_C	= ALLE_EINGANG;
	
	//Schalter Variabeln
	uint8_t power_schalter			= 0;
	uint8_t object_connected_switch	= 0;
	uint8_t akku_schalter_bit1_lsb	= 0; // |Stellen den Akkuzustand Binär da, von 0 bis 15
	uint8_t akku_schalter_bit2		= 0; // |
	uint8_t akku_schalter_bit3		= 0; // |
	uint8_t akku_schalter_bit4_msb	= 0; // |
	uint8_t powerbank_wird_geladen	= 0;
	
	//LED Variabeln
	uint8_t power_led				= 0; //powerbank ein oder aus
	uint8_t	object_connected_led	= 0; // etwas wird an der powerbank aufgeladen
	uint8_t led_powerbank_am_laden	= 0; //powerbank wird aufgeladen
	
	//zusätzliche Variabeln
	uint8_t on_off					= 0; //ob schaltung an oder aus ist
	uint8_t	adc_wert				= 0; //wie viel Spannung im akku ist(ladestand)
	uint8_t adc_wert_gerechnet		= 0; //ladestand zuerst + 4 und dann durch 5 gerechnet
	uint8_t akku_schutz				= 0; // blockiert die Schaltung falls der akku leer gegangen ist / bis wieder aufgeladen
	uint8_t inschalter				= 0; //gesamtwert aller schalter Bitweise
	
    while (1) 
    {
		//LED für akkustand anzeige
	uint8_t led_akku_voll			= 0;
	uint8_t led_akku_halb_voll		= 0;
	uint8_t led_akku_fast_leer		= 0;
	
//Eingabe
	inschalter					= SCHALTER;					//|Variabeln der Schalter werden auf 1 gestellt falls der schalter aktiviert ist
	power_schalter				= inschalter & SCHALTER_7;  //|
	on_off						= inschalter & SCHALTER_7;  //|
															//|
	object_connected_switch		= inschalter & SCHALTER_0;  //|
	powerbank_wird_geladen		= inschalter & SCHALTER_1;  //|
															//|
	akku_schalter_bit1_lsb		= inschalter & SCHALTER_3;  //|
	akku_schalter_bit2			= inschalter & SCHALTER_4;  //|
	akku_schalter_bit3			= inschalter & SCHALTER_5;  //|
	akku_schalter_bit4_msb		= inschalter & SCHALTER_6;  //|
	
//Verarbeitung
	power_led				= power_schalter;				//| wert von der Schalter Variabel wird auf LED variabel übertragen
	object_connected_led	= object_connected_switch;		//|
	adc_wert				= ((akku_schalter_bit1_lsb>>3) | (akku_schalter_bit2>>3) | (akku_schalter_bit3>>3) | (akku_schalter_bit4_msb>>3)); //alle schalter für akkuanzeige 
	adc_wert_gerechnet		= (adc_wert +4 )/5; //akku wert wird umgerechnet auf einen wert zwischen 0 und 3									   //werde binär zusammen gezählt
	switch(adc_wert_gerechnet){//je nach wert von akkuanziege wird ein case ausgeführ
		case 1: // wenn der wert 1 gibt
		led_akku_fast_leer	= LED_6;
		break;
		case 2: // wenn der wert 2 gibt
		led_akku_fast_leer	= LED_6;
		led_akku_halb_voll	= LED_5;
		break;
		case 3: // wenn der wert 3 gibt
		led_akku_fast_leer	= LED_6;
		led_akku_halb_voll	= LED_5;
		led_akku_voll		= LED_4;
	}
	if(adc_wert == AKKU_VOLL){  //schaltet lade LED aus wenn der akku voll geladen ist
		led_powerbank_am_laden = 0;
	}
	else{  // LED leuchtet falls powerbank auf aufladen ist
		led_powerbank_am_laden = powerbank_wird_geladen;
	}
	if(adc_wert==0){ // falls akku auf null fällt aktiviert es den akku schutz(setzt die variabvel auf 1)
		akku_schutz = 1;
	}
	if(powerbank_wird_geladen){ // wenn die Powerbank an den Strom angeschlossen wird, wird der akku schutz deaktiviert
		akku_schutz = 0;
	}
//Ausgabe
	if(on_off && (akku_schutz==0)){ //schaltet alle LED ein die aktiviert sein sollten, übertragung von LED variabel wert auf PORTA(LED_LOW_BYTE)
		LED_LOW_BYTE = (power_led | object_connected_led | led_akku_fast_leer | led_akku_halb_voll | led_akku_voll);
	}
	LED_LOW_BYTE = led_powerbank_am_laden; //alleine damit es immer leuchtet auch wenn powerbank ausgeschaltet ist
    }
}

