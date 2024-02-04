/*
 * ADC1.c
 *
 * Created: 2022-09-25 오후 5:17:00
 *  Author: TeddyKim
 */ 
//#define _BV(bit) (1 << (bit))
#include <avr/io.h>
#include <avr/sfr_defs.h>

uint16_t ADC_init(uint8_t ch) {
	
	ADMUX |= _BV(REFS0);
	ADCSRA |= 0x07;
	ADCSRA |= _BV(ADEN);
	ADCSRA |= _BV(ADFR);
	ADMUX = (ADMUX & 0xE0) | ch;
	ADCSRA |= _BV(ADSC);
	
	while (!(ADCSRA & _BV(ADIF)));
	//ADC = (ADCL | (ADCH << 8));
	return ADC;
}