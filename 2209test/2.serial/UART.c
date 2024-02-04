/*
 * UART.c
 *
 * Created: 2022-09-25 오후 5:17:44
 *  Author: TeddyKim
 */ 

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
void UART0_TX(unsigned char data)
{
	while( !(UCSR0A&(1 << UDRE0)) );
	UDR0 = data;
}