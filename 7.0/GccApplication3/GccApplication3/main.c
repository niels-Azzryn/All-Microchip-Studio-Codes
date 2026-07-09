/*
 * GccApplication3.c
 *
 * Created: 13.11.2025 08:19:17
 * Author : e4Menzi
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>



int main(void)
{
	DDRC = 0x00;
	DDRA = 0xFF;
	DDRB = 0xFF;
	uint8_t switch_value = 0;
	PORTA=switch_value;
	while(1){
		switch_value=0;
		if (PINC&1){
			switch_value= ++switch_value;
		}
		if (PINC&2){
			switch_value= ++switch_value;
		}
		if (PINC&4){
			switch_value= ++switch_value;
		}
		if (PINC&8){
			switch_value= ++switch_value;
		}
		if (PINC&16){
			switch_value= ++switch_value;
		}
		if (PINC&32){
			switch_value= ++switch_value;
		}
		if (PINC&64){
			switch_value= ++switch_value;
		}
		if (PINC&128){
			switch_value= ++switch_value;
		}
		PORTA=switch_value;
}
}



