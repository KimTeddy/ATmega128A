﻿/*
 * UART0.c
 *
 * Created: 2022-10-07 오후 12:44:29
 *  Author: TeddyKim
 */

#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <stdio.h>

void UART0_init(void)
{
	UBRR0H = 0x00;			//통신 속도 9600
	UBRR0L = 207;			//통신 속도 9600
	UCSR0A |= _BV(U2X0);	//2배속 모드
	UCSR0C |= 0X06;			//비동기, 8비트데이터, 패리티 없음, 1비트 정지 비트 모드
	UCSR0B |= _BV(RXEN0);	//수신 활성화
	UCSR0B |= _BV(TXEN0);	//발신 활성화
}
unsigned char UART0_rx(void)
{
	while( !(UCSR0A&(1 << RXC0)) );//수신 대기
	return UDR1;
}
void UART0_tx(char data)
{
	while( !(UCSR0A&(1 << UDRE0)) );//송신 대기
	UDR1 = data;					//데이터 전송
}
/*
void UART0_print_1_byte_number(uint8_t n)
{
	char numString[4] = "0";
	int i, index = 0;
	
	if (n > 0)						//문자열 변환
	{
		for (i = 0; n != 0; i++)
		{
			numString[i] = n % 10 + '0';
			n = n / 10;
		}
		numString[i] = '\0';
		index = i - 1;
	}
	for (i = index; i >= 0; i--)	//변환된 문자열 역순 출력
	UART0_tx(numString[i]);
}
*/
void UART0_print_1_byte_number(uint8_t n)
{
	char numString[4] = "0";
	sprintf(numString, "%d", n);	//문자열로 변환
	UART0_print_string(numString);  //문자열 출력
}
void UART0_print_string(char *str)
{
	for(int i=0; str[i]; i++)		//널문자 만날 때까지 반복
	UART0_tx(str[i]);				//바이트 단위 출력
}
void UART0_print_float(float f)
{
	char numString[20] = "0";
	
	sprintf(numString, "%f", f);
	UART0_print_string(numString);
}