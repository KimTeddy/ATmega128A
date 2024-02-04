/*
 * 1.blink.c
 *
 * Created: 2022-09-26 오후 10:59:08
 * Author : TeddyKim
 */ 
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRA=0X01;
	DDRB=0Xff;
    while (1)
    {
		//PORTA ^= 0b00000001;_delay_ms(500);
		
		PORTB=0x02;
		for(int i=0; i<2; i++)
		{
			PORTB <<= 2;
			_delay_ms(100);
		}
		for(int i=0; i<2; i++)
		{
			PORTB >>= 2;
			_delay_ms(100);
		}
		
    }
	return 0;
}

