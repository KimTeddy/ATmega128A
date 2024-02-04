/*
 * 2.serial.c
 *
 * Created: 2022-09-29 오후 12:46:38
 * Author : TeddyKim
 */ 
#define F_CPU 16000000L

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "functions.h"

FILE OUTPUT
= FDEV_SETUP_STREAM(UART0_TX, NULL, _FDEV_SETUP_WRITE);

/*4-4(학번)
int main(void)
{
	UART0_init();
	stdout = &OUTPUT;
	
	char str[100] = "Test Starting";
	printf("** Data Types...\n\r");
	printf("Integer : %ld\n\r", 60191798);
	printf("Float : %f\n\r", 3.14);
	printf("String : %s\n\r", str);
	printf("Character : %c\n\r", 'A');
	
	while (1){}
	return 0;
}
*/

/*4-1
int main(void)
{
	UART0_init();
	stdout = &OUTPUT;
	unsigned int count = 0;
	
	while (1)
	{
		printf("%d\n\r", count++);
		_delay_ms(1000);
	}
	return 0;
}
//*/
/* 4-1(학번)
{
	UART0_init();
	stdout = &OUTPUT;
	
    while (1)
    {
		printf("%ld\n\r", 60191798);
		_delay_ms(1000);
    }
	return 0;
}
*/

/*4-3
int main(void)
{
	UART0_init();
	stdout = &OUTPUT;
	
	printf("** Size of Data Types\n\r");
	printf("%d\n\r", sizeof(char));
	printf("%d\n\r", sizeof(int));
	printf("%d\n\r", sizeof(short));
	printf("%d\n\r", sizeof(long));
	printf("%d\n\r", sizeof(float));
	printf("%d\n\r", sizeof(double));
	
	while (1){}
	return 0;
}
*/

/*4-5
int main(void)
{
	UART0_init();
	stdout = &OUTPUT;
	
	float r1, r2, r3, r4;
	r1 = 3 / 2;
	r2 = 3.0 / 2;
	r3 = 3 / 2.0;
	r4 = 3.0 / 2.0;
	printf("Result 1 : %f\n\r", r1);
	printf("Result 2 : %f\n\r", r2);
	printf("Result 3 : %f\n\r", r3);
	printf("Result 4 : %f\n\r", r4);
	
	while (1){}
	return 0;
}
*/