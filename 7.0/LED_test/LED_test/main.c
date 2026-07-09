/*
 * LED_test.c
 *
 * Created: 11.09.2025 14:28:57
 * Author : e4Menzi
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#define BIT3 0x08
#define RGB_R_Ein PORTH = PORTH | BIT3
#define RGB_G_Ein PORTH = PORTH | BIT4
#define BIT4 0x10
#define RGB_B_Ein PORTH = PORTH | BIT5
#define BIT5 0x20
#define switch1 = PORTL | BIT0
#define BIT0 0x01
#define LED_TOGGLE PINL |= (1<<PINL0)

int main(void){
	DDRA = 255;
	DDRB = 255;
	DDRH = 255;
	DDRL;

	int i = 0;
	int b = 0;
    /* Replace with your application code */
    while (1) {
	PORTA = i;
	PORTB = b;
	_delay_ms(1000);
	i = i + 1;
	if(i>60)
	{
		i = 0;
	}
    
	if (i==0)
	{
		b=b+1;
	} 
	}
	
}




