/*
 * HC-SR04.c
 *
 * Created: 2022-11-24 오전 10:03:01
 * Author : TeddyKim
 */ 
#define F_CPU 16000000L

#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART1.h"

int read = 0;
uint16_t count = 0;
static double period_cpu = 1.0/F_CPU;//클럭 주기 상수

FILE OUTPUT = FDEV_SETUP_STREAM(UART1_tx, NULL, _FDEV_SETUP_WRITE);
FILE INPUT = FDEV_SETUP_STREAM(NULL, UART1_rx, _FDEV_SETUP_READ);

void PORT_init();
void ADC_init(uint8_t ch);
int read_ADC(void);
void TIMER0_init();
int TIMER0_overflow(void);
int TIMER0_time(int time_ms);

int main(void)
{
	//stdout = &OUTPUT;
	//stdin = &INPUT;
	double time0 = 0;
	float distance = 0;
	
	char str[] = "The Distance is ";
	char str1[] = " [cm] ";
	PORT_init();//포트 설정
	UART1_init(9600);//UART1 사용
	//ADC_init(0);//ADC0 사용
	TIMER0_init();//TIMER0 사용
	
	UART1_print_string("\r\n");
	UART1_print_string("\r\n");
	UART1_print_string("	Myongji University, Dept of Electronic Eng.\r\n");
	UART1_print_string("		  60191798 김영찬\r\n");
	UART1_print_string("\r\n");
	
	while (1) {
		PORTB |= 0x01;
		_delay_us(12);
		PORTB &= ~0x01;
		
		while ( !(PINB&_BV(PORTB2)) );
		TCNT0 = 0X00;
		while ( (PINB&_BV(PORTB2)) );
		count = TCNT0;
		
		time0 = count * 0.000064;
		distance = time0 * 340.0 * 100 / 2.0;
		
		UART1_print_string("\r\n");
		UART1_print_float(distance);
		UART1_print_string(str1);
		UART1_print_string("\r\n");
		
		_delay_ms(100);
	}//while end
	return 0;
}//main end

void PORT_init()
{
	DDRB |= _BV(DDRB0);
}


void TIMER0_init()
{
	TCCR0 |= ( _BV(CS02) | _BV(CS01) | _BV(CS00) );//분주비 1024로 설정
}

int read_ADC(void)
{
	while ( !(ADCSRA & _BV(ADIF)) );//AD 변환 완료 플래그 polling으로 확인
	return ADC;
}

int TIMER0_overflow(void)
{
	while ( !(TIFR & _BV(TOV0)) );//타이머 overflow 플래그 polling으로 확인
	TIFR |= _BV(TOV0); //1 넣어서 플래그 내려주기
	count++;//플래그 올라간 횟수 세기
	//printf("\r%d %d", count, (TIFR & _BV(TOV0)));
	return 1;
}

int TIMER0_time(int time_ms)
{
	int delay = (int)(period_cpu*128.0*256*count*1000);//밀리초 계산 식
	if( delay >= time_ms )//경과 시간이 time_sec보다 크거나 같으면
	{
		printf("\r%dms 경과, ", delay);//걸린 시간 출력
		TCCR0 &= ~( _BV(CS02) | _BV(CS01) | _BV(CS00) );//타이머/카운터 정지
		count = 0;//count 초기화
		return 1;//1 반환
	}
	else
	{
		TIMER0_init();//타이머 시작
		return 0;
	}
}