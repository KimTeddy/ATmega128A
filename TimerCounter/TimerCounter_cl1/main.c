/*
 * TimerCounter_cl1.c
 *
 * Created: 2022-11-17 오전 11:12:13
 * Author : TeddyKim
 */ 
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
#include <avr/delay.h>
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
	DDRB |= 0X10;
	//DDRD  &=~0X01;//PD0 을 외부 인터럽트용 버튼으로 사용 
	//PORTD |= 0X01;//내부 풀업 저항 사용
}

void ADC_init(uint8_t ch)
{
	ADMUX  |= _BV(REFS0);			//기준전압 AVCC
	ADCSRA |= 0x07;					//분주비 128
	ADCSRA |= _BV(ADEN);			//ADC ENABLE
	ADCSRA |= _BV(ADIE);			//ADC 인터럽트 ENABLE
	//ADCSRA |= _BV(ADFR);
	ADMUX   = ((ADMUX & 0xE0) | ch);//채널 선택
	sei();							//SREG = 0x80;글로벌 인터럽트 허용
}

void TIMER0_init()
{
	TCCR0 |= _BV(CS02) | _BV(CS01) | _BV(CS00);//분주비 
	TCCR0 |= _BV(COM01);
	TCCR0 |= _BV(WGM01) | _BV(WGM00);
	OCR0 = 0;
	//TIMSK |= _BV(OCIE0);
	//TCCR0 = 0x7F;
}

ISR(ADC_vect)
{
	int read = ADC;//ADC 값 읽기
	OCR0 = read/4;
	printf("ADC end.\t");
	printf("ADC = %4d\r\n\n", read);//AD Conversion 된 값을 터미널로 전송
	//_delay_ms(1000);
}

int main(void)
{
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	PORT_init();//포트 설정
	UART1_init(9600);//UART1 사용
	ADC_init(0);//ADC0 사용
	//INT0_init();//INT0 외부 인터럽트 사용
	TIMER0_init();//TIMER0 사용
	
	printf("\r\n");
	printf("\r\n");
	printf("	Myongji University, Dept of Electronic Eng.\r\n");
	printf("		  60191798 김영찬\r\n");
	printf("\r\n");
	
	
	
	while (1) {
		ADCSRA |= _BV(ADSC);//광센서가 붙어있는 ADC0 변환 시작
		}//while end
	return 0;
}//main end