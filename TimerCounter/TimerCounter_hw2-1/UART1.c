/*
 * UART.c
 *
 * Created: 2022-09-25 오후 5:17:44
 *  Author: TeddyKim
 */

#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <stdio.h>

void UART1_init(unsigned long baudrate)
{
	unsigned long ubrr = (16000000/(8*baudrate)) - 1;
	UBRR1H = ubrr>>8;			//통신 속도
	UBRR1L = ubrr & 0x0ff;			//통신 속도
	UCSR1A |= _BV(U2X1);	//2배속 모드
	UCSR1C |= 0X06;			//비동기, 8비트데이터, 패리티 없음, 1비트 정지 비트 모드
	UCSR1B |= _BV(RXEN1);	//수신 활성화
	UCSR1B |= _BV(TXEN1);	//발신 활성화
}
unsigned char UART1_rx(void)
{
	while( !(UCSR1A&(1 << RXC1)) );//수신 대기
	return UDR1;
}
void UART1_tx(unsigned char data)
{
	while( !(UCSR1A&(1 << UDRE1)) );//송신 대기
	UDR1 = data;					//데이터 전송
}
void UART1_print_1_byte_number(uint8_t n)
{
	char numString[4] = "0";
	sprintf(numString, "%d", n);	//문자열로 변환
	UART1_print_string(numString);	//문자열 출력
}
void UART1_print_string(char *str)
{
	for(int i=0; str[i]; i++)		//널문자 만날 때까지 반복
		UART1_tx(str[i]);			//바이트 단위 출력
}
void UART1_print_float(float f)
{
	char numString[20] = "0";
	sprintf(numString, "%f", f);
	UART1_print_string(numString);
}