/*
 * TimerCounter_hw2-2.c
 *
 * Created: 2022-11-19 오후 3:26:49
 * Author : 60191798 김영찬
 */ 

#define F_CPU 16000000L

#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "UART1.h"

int read = 0;
uint16_t count = 0;
double period_cpu = 1.0/F_CPU;//클럭 주기 상수

FILE OUTPUT = FDEV_SETUP_STREAM(UART1_tx, NULL, _FDEV_SETUP_WRITE);
FILE INPUT = FDEV_SETUP_STREAM(NULL, UART1_rx, _FDEV_SETUP_READ);

void PORT_init();
void ADC_init(uint8_t ch);
void TIMER0_init();
int TIMER0_time(int time_ms);

ISR(TIMER0_OVF_vect)
{
	count++;
	if( TIMER0_time(500) )//약 500ms가 지나면
	{
		ADCSRA |= _BV(ADSC);//AD변환 시작
	}
}

ISR(ADC_vect)
{
	read = ADC;//ADC 값 받기
	float temperature = read*(5/10.24);
	
	printf("현재 온도는 %5.1fºC입니다.", temperature);
	
	if( (int)temperature >= 27 )
	{
		PORTB |= 0X01;
		printf("\tLED is ON!\n\r");
	}
	else
	{
		PORTB &=~0X01;
		printf("\tLED is OFF.\n\r");
	}
}
int main(void)
{
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	PORT_init();//포트 설정
	UART1_init(9600);//UART1 사용
	ADC_init(0);//ADC0 사용
	TIMER0_init();//TIMER0 사용
	sei();
	
	printf("\r\n");
	printf("\r\n");
	printf("	Myongji University, Dept of Electronic Eng.\r\n");
	printf("		  60191798 김영찬\r\n");
	printf("\r\n");
	
	while (1) {
	}//while end
	return 0;
}//main end

void PORT_init()
{
	DDRB |= _BV(DDRB0);
}

void ADC_init(uint8_t ch)
{
	ADMUX  |= _BV(REFS0);			//기준전압 AVCC
	ADCSRA |= 0x07;					//분주비 128
	ADCSRA |= _BV(ADEN);			//ADC ENABLE
	ADCSRA |= _BV(ADIE);			//ADC 인터럽트 ENABLE
	ADMUX   = ((ADMUX & 0xE0) | ch);//채널 선택
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
		TCCR0 &= ~( _BV(CS02) | _BV(CS01) | _BV(CS00) );//타이머/카운터 정지
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