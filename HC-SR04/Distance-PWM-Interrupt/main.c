/*
 * Distance-PWM-Interrupt.c
 *
 * Created: 2022-11-30 오후 8:57:17
 * Author : TeddyKim
 * 기말고사 실기
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
void INT0_init();
void INT1_init();

void TIMER0_init();
void TIMER1_init();
//void TIMER2_init();
int TIMER0_overflow(void);
int TIMER0_time(int time_ms);
void ADC_init(uint8_t ch)
{
	ADMUX  |= _BV(REFS0);			//기준전압 AVCC
	ADCSRA |= 0x07;					//분주비 128
	ADCSRA |= _BV(ADEN);			//ADC ENABLE
	ADCSRA |= _BV(ADIE);			//ADC 인터럽트 ENABLE
	ADMUX   = ((ADMUX & 0xE0) | ch);//채널 선택
}
ISR(INT0_vect)
{
	TCNT2 = 0X00;
}

ISR(INT1_vect)
{
	double time0 = 0;
	float distance = 0;
	int dist = 0;
	
	count = TCNT2;
	
	time0 = count * 0.000064;
	distance = time0 * 340.0 / 2.0;
	
	dist = time0 * 340.0 * 100 / 2.0;
	
	//OCR0 = dist;
	
	printf("The Distance is %f [m]",distance);
	if (dist<30)
	{
		ADCSRA |= _BV(ADSC);
	}
	else
	{
		printf("\r\n");
		PORTB &=~0X02;
	}
}
ISR(ADC_vect)
{
	read = ADC;//ADC 값 받기
	
	printf("\t:\tThe CDS value is %d\n\r", read);
	
	if( read <= 300 )
	{
		PORTB |= 0X02;
		//printf("\tLED is ON!\n\r");
	}
	else
	{
		PORTB &=~0X02;
		//printf("\tLED is OFF.\n\r");
	}
}
int main(void)
{
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	PORT_init();//포트 설정
	UART1_init(9600);//UART1 사용
	TIMER0_init();//TIMER0 사용
	//TIMER1_init();
	TIMER2_init();
	ADC_init(0);
	
	INT0_init();
	INT1_init();
	while (1) {
		PORTB |= 0x01;
		_delay_us(12);
		PORTB &= ~0x01;
		
		_delay_ms(1000);
	}//while end
	return 0;
}//main end

void PORT_init()
{
	DDRB |= _BV(DDRB0);//Trigger
	DDRB |= _BV(DDRB4);//OC0
	DDRB |= _BV(DDRB7);//OC2
	DDRB |= _BV(DDRB1);//led
}
void TIMER2_init()
{
	TCCR2 |= ( _BV(CS02) | _BV(CS00) );//분주비 1024로 설정
}
void TIMER0_init()
{
	
	TCCR0 |= _BV(CS02) | _BV(CS01) | _BV(CS00);//분주비
	TCCR0 |= _BV(COM01);
	TCCR0 |= _BV(WGM01) | _BV(WGM00);
	OCR0 = 0;
}

void INT0_init()
{
	EIMSK |= _BV(INT0);
	EICRA |= _BV(ISC01) | _BV(ISC00);
	sei();
}
void INT1_init()
{
	EIMSK |= _BV(INT1);
	EICRA |= _BV(ISC11);
	sei();
}