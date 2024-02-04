/*
 * UART1.h
 *
 * Created: 2022-10-07 오후 12:45:31
 *  Author: TeddyKim
 */ 


#ifndef UART1_H_
#define UART1_H_

void UART1_init(unsigned long);
unsigned char UART1_rx(void);
void UART1_tx(unsigned char data);
void UART1_print_1_byte_number(uint8_t n);
void UART1_print_string(char *str);
void UART1_print_float(float f);

#endif /* UART1_H_ */