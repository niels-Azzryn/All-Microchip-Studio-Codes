/*
 * Poti_Test.c
 *
 * Created: 20.11.2025 07:52:09
 * Author : e4Menzi
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>


int main(void)
{
	DDRA = 0xFF;
	while (1)
	{
		PORTA = -2;
	}
}

