/*
 * testojafowjaofj.c
 *
 * Created: 11.12.2025 15:02:16
 * Author : e4Menzi
 */ 

#include <avr/io.h>


int main(void)
{	
	DDRA = 0xFF;
	DDRC = 0x00;
    while (1) 
    {
		PORTA = PINC;
    }
}

