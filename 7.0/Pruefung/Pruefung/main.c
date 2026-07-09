/*
 * Pruefung.c
 *
 * Created: 28.11.2025 07:48:16
 * Author : e4Menzi
 */ 

#include <avr/io.h>
//Hardware Register
#define DDR_LED_LOW_BYTE			(DDRA)
#define DDR_LED_HIGH_BYTE		(DDRB)
#define DDR_SCHALTER						(DDRC)
#define PORT_LED_LOW_BYTE		(PORTA)
#define SCHALTER									(PINC)

//Ein oder Ausgang
#define ALLE_AUSGANG		(0xFF)
#define ALLE_EINGANG		(0x00)

//Masken----------------------------------
//Schalter
#define MASKE_SCHALTER_S5		(1<<0)
#define MASKE_SCHALTER_S6		(1<<1)
#define MASKE_SCHALTER_S7		(1<<2)
#define MASKE_SCHALTER_S8		(1<<3)
#define MASKE_SCHALTER_S9		(1<<4)
#define MASKE_SCHALTER_S10	(1<<5)
//LED
#define LED_2				(1<<2)
#define LED_3				(1<<3)
#define LED_4				(1<<4)
#define LED_5				(1<<5)
#define LED_6				(1<<6)
#define LED_7				(1<<7)




int main(void)
{
	//Hardware initialisieren
	DDR_LED_LOW_BYTE=ALLE_AUSGANG;
	DDR_LED_HIGH_BYTE=ALLE_AUSGANG;
	DDR_SCHALTER=ALLE_EINGANG;

	//Schalter Variabeln
	uint8_t inschalter						= 0;
	uint8_t on_off_schalter			= 0;
	uint8_t hot_or_cold_schalter	= 0;
	uint8_t schalter_wertigkeit_1	= 0;
	uint8_t schalter_wertigkeit_2	= 0;
	uint8_t schalter_wertigkeit_4	= 0;
	uint8_t schalter_wertigkeit_8	= 0;
	
	//zusätzliche Variabeln für z.B. rechnungen
	uint8_t zweiter_ring				= 0;
	uint8_t inschalter_7bis10		= 0;
	
	//LED Variabeln
	uint8_t power_led				= 0;
	uint8_t heiss_led				= 0;
	uint8_t heizstufe_total		= 0;

 
    while (1) 
    {
	//Variabeln für Rechnungen/LED/ Heizstufen
	uint8_t heizstufe_1		= 0;
	uint8_t heizstufe_2		= 0;
	uint8_t heizstufe_3		= 0;
	uint8_t heizstufe_4		= 0;
	uint8_t heizstufe_5		= 0;
	
//Eingabe

	//setzt den wert von inschalter auf den binären wert von PINC
	inschalter = SCHALTER;
	
	//sagt ob bestimmter schalter aktiviert ist mit 0 oder 1
	on_off_schalter				= inschalter & MASKE_SCHALTER_S5;
	schalter_wertigkeit_1	= inschalter & MASKE_SCHALTER_S7;
	schalter_wertigkeit_2	= inschalter & MASKE_SCHALTER_S8;
	schalter_wertigkeit_4	= inschalter & MASKE_SCHALTER_S9;
	schalter_wertigkeit_8	= inschalter & MASKE_SCHALTER_S10;
	hot_or_cold_schalter	= inschalter & MASKE_SCHALTER_S6;
	
	
//Verarbeitung

	//inschalter_7bis10 bekommt den wert von schalter 7 bis 10
	inschalter_7bis10 = ((schalter_wertigkeit_1>>2)+(schalter_wertigkeit_2>>2)+(schalter_wertigkeit_4>>2)+(schalter_wertigkeit_8>>2));
	
	//wenn inschalter_7bis10 den Wert 15 hat wird der zweite Ring aktiviert(variabel:zweiter_ring) oder deaktiviert wenn es nicht der fall ist
	if(inschalter_7bis10==15){
		zweiter_ring = LED_7;
	}
	if(on_off_schalter == 0){
		zweiter_ring = 0;
	}
	
	//inschalter_7bis10 durch 2 rechnen damit werte von 0-7 entstehen
	inschalter_7bis10 = inschalter_7bis10/2;
	
	//der gerechnete Wert von inschalter_7bis10 wird ausgelesen und aktiviert für jeden zusatnd von 1-7 eine bestimmte anzahl LED
	switch (inschalter_7bis10){
		case 1:
		heizstufe_1=LED_2;
		break;
		case 2:
		heizstufe_1=LED_2;
		heizstufe_2=LED_3;
		break;
		case 3:
		heizstufe_1=LED_2;
		heizstufe_2=LED_3;
		heizstufe_3=LED_4;
		break;
		case 4:
		heizstufe_1=LED_2;
		heizstufe_2=LED_3;
		heizstufe_3=LED_4;
		heizstufe_4=LED_5;
		break;
		case 5:
		case 6:
		case 7:
		heizstufe_1=LED_2;
		heizstufe_2=LED_3;
		heizstufe_3=LED_4;
		heizstufe_4=LED_5;
		heizstufe_5=LED_6;
		break;
	}
	
	//fässt alle LED Variabeln in eine einzige Variabel zusammen
	heizstufe_total = (heizstufe_1+heizstufe_2+heizstufe_3+heizstufe_4+heizstufe_5);
	
	//wert von schalter wird auf led variabel übertragen
	power_led = on_off_schalter; 
	heiss_led = hot_or_cold_schalter;
	
	
//Ausgabe

	//wenn on_off aktiviert ist läuft die schaltung sonst nicht
	if(on_off_schalter==1){
		//eine bestimmte LED wird aktiviert, auserhalb des if's weil es auch aktiviert werden muss wenn die schaltung deaktiviert ist
		PORT_LED_LOW_BYTE = (power_led|heizstufe_total|zweiter_ring); 
	}
	
	//eine bestimmte LED wird aktiviert, auserhalb des if's weil es auch aktiviert werden muss wenn die schaltung deaktiviert ist
	PORT_LED_LOW_BYTE |= heiss_led; 
	
	//alles LED werden ausgeschaltet
	PORT_LED_LOW_BYTE = 0;
}
}

