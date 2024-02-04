/*
 * ADC1.c
 *
 * Created: 2022-09-25 오후 5:17:00
 *  Author: TeddyKim
 */ 
//#define _BV(bit) (1 << (bit))
#include <avr/io.h>
#include <avr/sfr_defs.h>

void ADC_init(uint8_t ch)
{
	ADMUX |= _BV(REFS0);		//기준전압 AVCC
	
	ADCSRA |= 0x07;				//분주비 128
	
	ADCSRA |= _BV(ADEN);		//ADC ENABLE
	//ADCSRA |= _BV(ADFR);		//프리러닝 모드
	ADMUX = (ADMUX & 0xE0) | ch;//채널 선택
	ADCSRA |= _BV(ADSC);		//AD변환 시작
}

uint16_t read_ADC(void)
{
	while ( !(ADCSRA & _BV(ADIF)) );
	return ADC;
}