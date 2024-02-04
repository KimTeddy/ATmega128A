/*
 * TimerCounter_hw1.c
 *
 * Created: 2022-11-10 오후 5:46:26
 * Author : 60191798 김영찬
 */ 

#define F_CPU 16000000L

#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "UART1.h"

volatile int count = 0;
uint8_t count_start = 0;
float period_cpu = 1.0/F_CPU;

FILE OUTPUT = FDEV_SETUP_STREAM(UART1_tx, NULL, _FDEV_SETUP_WRITE);
FILE INPUT = FDEV_SETUP_STREAM(NULL, UART1_rx, _FDEV_SETUP_READ);

void PORT_init();
void ADC_init(uint8_t ch);
void INT0_init();
void TIMER0_init();
int TIMER0_time(int);

void PORT_init()
{
	DDRD  &=~0X01;//PD0 을 외부 인터럽트용 버튼으로 사용 
	PORTD |= 0X01;//내부 풀업 저항 사용
}

void ADC_init(uint8_t ch)
{
	ADMUX  |= _BV(REFS0);			//기준전압 AVCC
	ADCSRA |= 0x07;					//분주비 128
	ADCSRA |= _BV(ADEN);			//ADC ENABLE
	ADCSRA |= _BV(ADIE);			//ADC 인터럽트 ENABLE
	ADMUX   = ((ADMUX & 0xE0) | ch);//채널 선택
	sei();							//SREG = 0x80;글로벌 인터럽트 허용
}

void INT0_init()
{
	EIMSK |= _BV(INT0);//INT0 외부 인터럽트핀(PD0) 사용
	EICRA |= _BV(ISC01);//버튼이 풀업 저항이기에 falling edge == 버튼이 눌릴 때
}

void TIMER0_init()
{
	TCCR0 |= _BV(CS02) /*| _BV(CS01)*/ | _BV(CS00);//분주비 128로 설정
	OCR0 = 255;//타이머가 255와 일치할 때
	TIMSK |= _BV(OCIE0);//비교 일치 인터럽트 ENABLE
}

int TIMER0_time(int time_ms)
{
	int delay = period_cpu*128.0*(OCR0+1)*count*1000;//밀리초 계산 식
	if( delay >= time_ms )//경과 시간이 time_sec보다 크거나 같으면
	{
		printf("\r%dms 경과", delay);//걸린 시간 누적 출력
		
		count_start = 0;//count 정지
		count = 0;//count 초기화
		return 1;//1 반환
	}
	else
	{
		return 0;
	}
}

ISR(INT0_vect)//버튼이 눌릴 때
{
	count = 0;//count 초기화
	count_start = 1;//카운터 시작
	printf("Button pressed.\r\n");
	//0.1초 카운트
}

ISR(TIMER0_COMP_vect)//타이머가 OCR0과 일치할 때
{//printf("\r%dms 경과", (int)(period_cpu*128.0*(OCR0+1)*count*1000));
	if( count_start == 1 )//버튼이 눌려서 카운터를 시작한 상태라면
	{
		count++;//타이머가 OCR0과 일치할 때의 횟수 세기
	}
	else count = 0;
	//printf("count = %d\r\n", count);
	
	if( TIMER0_time(100) )//0.1초 경과하면 true
	{
		printf(" 후 ADC start.\r\n");
		ADCSRA |= _BV(ADSC);//광센서가 붙어있는 ADC0 변환 시작
	}
}

ISR(ADC_vect)
{
	int read = ADC;//ADC 값 읽기
	printf("ADC end.\t");
	printf("ADC = %4d\r\n\n", read);//AD Conversion 된 값을 터미널로 전송
}

int main(void)
{
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	PORT_init();//포트 설정
	UART1_init(9600);//UART1 사용
	ADC_init(0);//ADC0 사용
	INT0_init();//INT0 외부 인터럽트 사용
	TIMER0_init();//TIMER0 사용
	
	printf("\r\n");
	printf("\r\n");
	printf("	Myongji University, Dept of Electronic Eng.\r\n");
	printf("		  60191798 김영찬\r\n");
	printf("\r\n");
	
	while (1) {}//while end
	return 0;
}//main end