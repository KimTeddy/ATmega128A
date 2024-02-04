/*
 * Loadcell.c
 *
 * Created: 2022-12-03
 * Author : 60191798 김영찬
 */ 
#define F_CPU 16000000L

#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART1.h"

uint16_t count = 0;
static double period_cpu = 1.0/F_CPU;//클럭 주기 상수

FILE OUTPUT = FDEV_SETUP_STREAM(UART1_tx, NULL, _FDEV_SETUP_WRITE);
FILE INPUT = FDEV_SETUP_STREAM(NULL, UART1_rx, _FDEV_SETUP_READ);

void ADC_init(uint8_t ch)
{
	//ADMUX  |= _BV(REFS0) ;			//기준전압 AVCC
	//ADMUX  |= _BV(REFS0) | _BV(REFS1);			//기준전압 AVCC
	ADCSRA |= 0x07;					//분주비 128
	ADCSRA |= _BV(ADEN);			//ADC ENABLE
	ADCSRA |= _BV(ADIE);			//ADC 인터럽트 Flag ENABLE
	ADMUX   = ((ADMUX & 0xE0) | ch);//채널 선택
	sei();							//SREG = 0x80;글로벌 인터럽트 허용
}
void TIMER0_init()
{
	TCCR0 |= _BV(CS02) /*| _BV(CS01) */| _BV(CS00);//분주비 128로 설정
	TIMSK |= _BV(TOIE0);//오버플로우 인터럽트 ENABLE
}
int TIMER0_time(int time_ms)
{
	int delay = (int)(period_cpu*128.0*256*count*1000);//밀리초 계산 식
	if( delay >= time_ms )//경과 시간이 time_sec보다 크거나 같으면
	{
		//TCCR0 &= ~( _BV(CS02) | _BV(CS01) | _BV(CS00) );//타이머/카운터 정지
		count = 0;//count 초기화
		printf("\r%dms 경과, ", delay);//걸린 시간 출력
		return 1;//1 반환
	}
	else
	{
		TIMER0_init();//타이머 시작
		return 0;
	}
}
ISR(ADC_vect)
{
	int read = ADC;
	int gram = read-49;
	if(gram<0) gram = 0;
	printf("ADC = %4d\tread = %4d\t100원 = %2d\t10원 = %2d\r\n", read, gram, gram/5, ((gram%5)-1)/3);
	
}
ISR(TIMER0_OVF_vect)
{
	count++;
	if( TIMER0_time(100) )//약 10ms가 지나면
	{
		ADCSRA |= _BV(ADSC);//AD변환 시작
	}
}
int main(void)
{
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	UART1_init(9600);
	ADC_init(0);
	TIMER0_init();//TIMER0 사용
	sei();
	
	printf("\r\n");
	printf("\r\n");
	printf("	Myongji University, Dept of Electronic Eng.\r\n");
	printf("		  60191798 김영찬\r\n");
	printf("\r\n");
	while (1)
	{}//while end
	return 0;
}
