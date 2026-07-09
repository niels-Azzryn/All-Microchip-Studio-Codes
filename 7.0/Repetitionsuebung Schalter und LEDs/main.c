/*
 * Repetitionsuebung Schalter und LEDs.c
 *
 * Created: 13.11.2025 11:00:13
 * Author : e4Menzi
 */ 

#include <avr/io.h>


int main(void)
{
	uint8_t status = 0;
	DDRA = 0xFF
 
    while (1) 
    {
		if (PINC&1)
		{
			PORTA = 1;
		}
    }
}

