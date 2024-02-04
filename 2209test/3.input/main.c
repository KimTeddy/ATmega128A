/*
 * 3.input.c
 *
 * Created: 2022-10-05 오후 6:04:16
 * Author : TeddyKim
 */ 
#define F_CPU 16000000L
#include <avr/io.h>

#define LED1 0x04
#define LED2 0x08
#define BUTTON1 0x01
#define BUTTON2 0x02
//*
int main(void)
{
    DDRB |= LED1;//PB1을 OUTPUT으로
	DDRB |= LED2;//DDRB = 0x0C;
	
	PORTB = 0x00;
	
    while (1) 
    {
		if( (PINB & BUTTON1) == BUTTON1 )
		{
			PORTB |= LED1;
		}
		else if ( (PINB & BUTTON2) == BUTTON2 )
		{
			PORTB |= LED2;
		}
		else
		{
			PORTB &= ~LED1;
			PORTB &= ~LED2;
		}
    }
}
//*/
/*
int main(void)
{
	DDRB=0X02;//PB1을 OUTPUT으로
	while (1)
	{
		if( (PINB & 0X01) )
		{
			PORTB ^= 0X02;
		}
		
	}
}
*/