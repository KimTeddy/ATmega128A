/*
 * Interrupt_hw1.c
 *
 * Created: 2022-11-06 오전 10:09:00
 * Author : TeddyKim
 */ 

#define F_CPU 16000000L

#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART1.h"

FILE OUTPUT \
= FDEV_SETUP_STREAM(UART1_tx, NULL, _FDEV_SETUP_WRITE);
FILE INPUT \
= FDEV_SETUP_STREAM(NULL, UART1_rx, _FDEV_SETUP_READ);

void ADC_init(uint8_t ch)
{
	ADMUX  |= _BV(REFS0);			//기준전압 AVCC
	ADCSRA |= 0x07;					//분주비 128
	ADCSRA |= _BV(ADEN);			//ADC ENABLE
	ADCSRA |= _BV(ADIE);			//ADC 인터럽트 Flag ENABLE
	ADMUX   = ((ADMUX & 0xE0) | ch);//채널 선택
	sei();							//SREG = 0x80;글로벌 인터럽트 허용
}

ISR(ADC_vect)
{
	int read = ADC;
	printf("ADC = %4d\t", read);
	if( read < 400 ){
		PORTB |= 0x01; printf("LED is ON!\r\n");
	}
	else{
		PORTB &= ~0x01; printf("LED is OFF.\r\n");
	}
}

int main(void)
{
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	DDRB |= 0X02;
	UART1_init();
	ADC_init(0);
	
	printf("\r\n");
	printf("\r\n");
	printf("	Myongji University, Dept of Electronic Eng.\r\n");
	printf("		  60191798 김영찬\r\n");
	printf("\r\n");
	while (1)
	{
		ADCSRA |= _BV(ADSC);	//AD변환 시작
		_delay_ms(500);
	}//while end
	return 0;
}//main end