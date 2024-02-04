/*
 * TimerCounter_hw2-1.c
 *
 * Created: 2022-11-18 오후 10:34:37
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
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	PORT_init();//포트 설정
	UART1_init(9600);//UART1 사용
	ADC_init(0);//ADC0 사용
	TIMER0_init();//TIMER0 사용
	
	printf("\r\n");
	printf("\r\n");
	printf("	Myongji University, Dept of Electronic Eng.\r\n");
	printf("		  60191798 김영찬\r\n");
	printf("\r\n");
	
	while (1) {
		TIMER0_overflow();
		
		if( TIMER0_time(500) )//약 500ms가 지나면
		{
			read = read_ADC();//ADC 값 받기
			float temperature = read*(5/10.24);
			
			printf("현재 온도는 %5.1fºC입니다.", temperature);
			
			if((int)temperature >= 27){
				PORTB |= 0X01;
				printf("\tLED is ON!\n\r");
			}
			else{
				PORTB &=~0X01;
				printf("\tLED is OFF.\n\r");
			}
		}
	}//while end
	return 0;
}//main end

void PORT_init()
{
	DDRB |= _BV(DDRB0);
}

void ADC_init(uint8_t ch)
{
	ADMUX  |= _BV(REFS0);		//기준전압 AVCC
	ADCSRA |= 0x07;		//분주비 128
	ADCSRA |= _BV(ADEN);		//ADC ENABLE
	ADCSRA |= _BV(ADFR);		//프리러닝 모드
	ADMUX   = ((ADMUX & 0xE0) | ch);//채널 선택
	ADCSRA |= _BV(ADSC);		//AD변환 시작
}

void TIMER0_init()
{
	TCCR0 |= ( _BV(CS02) /*| _BV(CS01) */| _BV(CS00) );//분주비 128로 설정
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