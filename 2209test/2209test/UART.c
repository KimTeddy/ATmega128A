/*
 * UART.c
 *
 * Created: 2022-09-25 오후 5:17:44
 *  Author: TeddyKim
 */ 
//#define _BV(bit) (1 << (bit))
#include <avr/io.h>
#include <avr/sfr_defs.h>

void UART_init()
{
	UBRR1H = 0x00;
	UBRR1L = 207;
	UCSR1A |= _BV(U2X1);
	UCSR1C |= 0X06;
	UCSR1B |= _BV(RXEN1);
	UCSR1B |= _BV(TXEN1);
}
void UART_TX(unsigned char data)
{
	while( !(UCSR1A&(1 << UDRE1)) );
	UDR1 = data;
}
unsigned char UART_RX(void)
{
	while( !(UCSR1A&(1 << RXC1)) );
	return UDR1;
}
void UART_TX_string(/*short out, */char *str)
{/*
	switch(out)
	{
	case 0:
		for(int i=0; str[i]; i++)
		UART0_TX(str[i]);
		break;
	case 1:*/
	for(int i=0; str[i]; i++)
	UART_TX(str[i]);
		//break;
	//}
}

/*
#include <avr/io.h>
#include <avr/sfr_defs.h>

void UART0_init()
{
	UBRR0H = 0x00;
	UBRR0L = 207;
	UCSR0A |= _BV(U2X0);
	UCSR0C |= 0X06;
	UCSR0B |= _BV(RXEN0);
	UCSR0B |= _BV(TXEN0);
}
void UART1_init()
{
	UBRR1H = 0x00;
	UBRR1L = 207;
	UCSR1A |= _BV(U2X1);
	UCSR1C |= 0X06;
	UCSR1B |= _BV(RXEN1);
	UCSR1B |= _BV(TXEN1);
}
void UART0_TX(unsigned char data)
{
	while( !(UCSR0A&(1 << UDRE0)) );
	UDR0 = data;
}
void UART1_TX(unsigned char data)
{
	while( !(UCSR1A&(1 << UDRE1)) );
	UDR1 = data;
}
unsigned char UART_RX(void)
{
	while( !(UCSR1A&(1 << RXC1)) );
	return UDR1;
}
void UART_TX_string(char *str)
{
	for(int i=0; str[i]; i++)
		UART_TX(str[i]);
}
*/