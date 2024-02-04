/*
 * 2209test.c
 *
 * Created: 2022-09-22 오전 11:41:57
 * Author : TeddyKim
 */ 
#define F_CPU 16000000L

#include <avr/io.h>
#include <util/delay.h>

#include <stdio.h>
#include "functions.h"

int main(void)
{
	int a=0;
	int adc0=0,adc1=0;
	char itoc[10]="0";
	DDRA=0XFF;
	DDRB=0X2A;
	DDRC=0XC0;
	
	UART_init();
	UART_TX_string("start");UART_TX_string("\n\r");
	//PORTA = 0x01;_delay_ms(50);PORTA = 0x02;_delay_ms(50);PORTA = 0x04;_delay_ms(50);PORTA = 0x08;_delay_ms(50);
	//PORTA = 0x10;_delay_ms(50);PORTA = 0x20;_delay_ms(50);PORTA = 0x40;_delay_ms(50);PORTA = 0x80;
	PORTA=0x01;
	for(int i=0; i<7; i++)
	{
		PORTA <<= 1;
		_delay_ms(50);
	}
	for(int i=0; i<7; i++)
	{
		PORTA >>= 1;
		_delay_ms(50);
	}
	
    while (1) 
    {
		adc0 = ADC_init(1);
		//adc1 = ADC_init(1);
		for(int i=0; i<10; i++)
		itoc[i]='\0';
		sprintf(itoc, "%d", adc0);
		UART_TX_string(itoc);UART_TX_string("\n\r");//UART_TX_string(" ");
		/*
		for(int i=0; i<10; i++)
			itoc[i]='\0';
		sprintf(itoc, "%d", adc1);
		UART_TX_string(itoc);UART_TX_string("\n\r");
		*/
		a = (double)adc0/778 * 10;
		switch(a)
		{
			case 0:PORTA &= 0x00;//PORTA ^= 0x01;_delay_ms(500);PORTC = 0x00;
				PORTA=0x01;
				for(int i=0; i<7; i++)
				{
					PORTA <<= 1;
					_delay_ms(50);
				}
				for(int i=0; i<7; i++)
				{
					PORTA >>= 1;
					_delay_ms(50);
				}
				/*
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
				}*/
				break;
			case 1:PORTA = 0x01;PORTC = 0x00;//PORTA = 0x01;
				break;
			case 2:PORTA = 0x03;PORTC = 0x00;//PORTA = 0x02;
				break;
			case 3:PORTA = 0x07;PORTC = 0x00;//PORTA = 0x04;
				break;
			case 4:PORTA = 0x0F;PORTC = 0x00;//PORTA = 0x08;
				break;
			case 5:PORTA = 0x1F;PORTC = 0x00;//PORTA = 0x10;
				break;
			case 6:PORTA = 0x3F;PORTC = 0x00;//PORTA = 0x20;
				break;
			case 7:PORTA = 0x7F;PORTC = 0x00;//PORTA = 0x40;
				break;
			case 8:PORTA = 0xFF;PORTC = 0x00;//PORTA = 0x80;
				break;
			case 9:PORTA = 0xFF;PORTC = 0x80;
				break;
			case 10:PORTA = 0xFF;PORTC = 0xC0;
				break;
			default:PORTA = 0xFF;PORTC = 0xC0;
				break;
		}//switch end
    }//while end
	return 0;
}//main end