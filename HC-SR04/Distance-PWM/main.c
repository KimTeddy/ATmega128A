/*
 * Distance-PWM.c
 *
 * Created: 2022-11-30 오후 5:44:28
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
void TIMER1_init();
//void TIMER2_init();
int TIMER0_overflow(void);
int TIMER0_time(int time_ms);

int main(void)
{
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	double time0 = 0;
	float distance = 0;
	int motor = 0;
	
	PORT_init();//포트 설정
	UART1_init(9600);//UART1 사용
	//ADC_init(0);//ADC0 사용
	TIMER0_init();//TIMER0 사용
	//TIMER1_init();
	TIMER2_init();
	
	while (1) {
		PORTB |= 0x01;
		_delay_us(12);
		PORTB &= ~0x01;
		
		while ( !(PINB&_BV(PORTB2)) );
		TCNT2 = 0X00;
		while ( (PINB&_BV(PORTB2)) );
		count = TCNT2;
		
		time0 = count * 0.000064;
		distance = time0 * 340.0 * 100 / 2.0;
		
		motor = (int)distance;
		if (motor>255)	motor = 255;
		else if(motor<5)motor = 0;
		
		OCR0 = motor;
		
		printf("Distance: %f[cm],\tMotor: %d\n\r",distance,motor);
		
		//_delay_ms(100);
	}//while end
	return 0;
}//main end

void PORT_init()
{
	DDRB |= _BV(DDRB0);//Trigger
	DDRB |= _BV(DDRB4);//OC0
	DDRB |= _BV(DDRB7);//OC2
}
/*
void TIMER0_init()
{
	TCCR0 |= ( _BV(CS02) | _BV(CS01) | _BV(CS00) );//분주비 1024로 설정
}

void TIMER2_init()
{
	TCCR2 |= ( _BV(CS22) | _BV(CS21) | _BV(CS20) );//분주비 1024로 설정
	TCCR2 |= _BV(COM21);
	TCCR2 |= _BV(WGM21) | _BV(WGM20);
	OCR2 = 0;
	TCNT2 = 0;
}
*/
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
void TIMER1_init()
{
	
	TCCR1B |= _BV(CS12) | _BV(CS10);//분주비
}