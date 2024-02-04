/*
 * 1.UART_lib.c
 *
 * Created: 2022-10-07 오후 12:26:03
 * Author : TeddyKim
 */ 
#define F_CPU 16000000L

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include "UART1.h"

int main(void)
{
	UART1_init();
	
	int index = 0;
	int process_data = 0;
	char buffer[100] = "0";
	char data;
	
    while (1) 
    {
		data = UART1_rx();

		if(data=='\r'){//수신 문자열 끝을 NULL문자로 변환
			buffer[index] = '\0';
			process_data = 1;
		}
		else				//알파벳은 대/소문자 변환
		{
			if(data>='A' && data<= 'Z'){
				buffer[index] = data + ('a' - 'A');
			}
			else if(data>='a' && data<= 'z'){
				buffer[index] = data - ('a' - 'A');
			}
			else{			//나머지 문자는 그대로
				buffer[index] = data;
			}
			index++;
		}
		if(process_data == 1)//문자열 수신 완료 후
		{
			UART1_print_string(buffer);//출력
			UART1_print_string("\n\r");
			process_data = 0;
			index = 0;		//index 초기화
		}
		
    }
}








//UART1_tx( UART1_rx() );
/*
	char str[] = "Test using UART1 Library";
	uint8_t num = 128;
	UART1_print_string("\n\r");
	UART1_print_string(str);
	UART1_print_string("\n\r");
	UART1_print_1_byte_number(num);
	UART1_print_string("\n\r");
	UART1_print_float(3.14);
	
	
	
	
	
	
	
	
	
	
			if(index > 20){
				UART1_print_string("Max String Number : 20\n\r");
				process_data = 0;
				index = 0;
			}
	*/