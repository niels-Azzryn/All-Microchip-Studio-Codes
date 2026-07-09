/*
 * GccApplication1.c
 *
 * Created: 10.11.2025 15:54:59
 * Author : e4Menzi
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>



int main(void)
{
	uint8_t mein8BitWert = 00;
	uint16_t mein16BitWert = 0000;
	DDRC = 0x00;
	DDRL = 0x00;
	DDRA = 0xFF;
	DDRB = 0xFF;
	PORTH = 0x00;
    while(1){
		if((PINC & 0b00010000) == 16){
			PORTA = 255;
			}
		else {
			PORTA = 0x00;
			}
		 if(PINC == 255){
			PORTH = 100;
			}	
		else{
			PORTH = 0;
			}
	}
    
}

