/*
 * ADC_hw1-1.c
 *
 * Created: 2022-10-27 오후 12:31:26
 * Author : TeddyKim
 */ 
#define F_CPU 16000000L

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <stdio.h>
#include "UART1.h"

FILE OUTPUT \
	= FDEV_SETUP_STREAM(UART1_tx, NULL, _FDEV_SETUP_WRITE);
FILE INPUT \
	= FDEV_SETUP_STREAM(NULL, UART1_rx, _FDEV_SETUP_READ);

void ADC_init(uint8_t ch)
{
	ADMUX  |= _BV(REFS0);		//기준전압 AVCC
	ADCSRA |= 0x07;				//분주비 128
	ADCSRA |= _BV(ADEN);		//ADC ENABLE
	ADCSRA |= _BV(ADFR);		//프리러닝 모드
	ADMUX   = ((ADMUX & 0xE0) | ch);//채널 선택
	ADCSRA |= _BV(ADSC);		//AD변환 시작
}

int read_ADC(void)
{
	while ( !(ADCSRA & _BV(ADIF)) );
	return ADC;
}

int main(void)
{
	int read = 0;
	float angle = 0;
	
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	DDRA=0XFF;
	DDRC=0XC0;
	
	UART1_init();
	ADC_init(0);
	
	while (1) 
	{
		read = read_ADC();
		angle = 90.0 * (float)read/366.0;
		printf("ADC = %d \t|\t가변저항 각도 = %6.2fº\n\r", read, angle);
		_delay_ms(1000);
	}//while end
	return 0;
}//main end