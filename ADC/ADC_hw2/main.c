/*
 * ADC_hw2.c
 *
 * Created: 2022-10-27 오후 12:43:04
 * Author : TeddyKim
 */ 
#define F_CPU 16000000L

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART1.h"
#include "ADC.h"

FILE OUTPUT \
	= FDEV_SETUP_STREAM(UART1_tx, NULL, _FDEV_SETUP_WRITE);
FILE INPUT \
	= FDEV_SETUP_STREAM(NULL, UART1_rx, _FDEV_SETUP_READ);

int main(void)
{
	int adc3 = 0;
	float angle = 0;
	
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	UART1_init();
	
	while (1)
	{
		ADC_init(3);
		adc3 = read_ADC();
		angle = 90.0 * (float)adc3/366.0;
		printf("ADC = %d \t|\t가변저항 각도 = %6.2fº\n\r", adc3, angle);
	}//while end
	return 0;
}//main end