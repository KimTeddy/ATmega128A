/*
 * UART0.h
 *
 * Created: 2022-10-07 오후 12:46:43
 *  Author: TeddyKim
 */ 


#ifndef UART0_H_
#define UART0_H_

void UART0_init(void);
unsigned char UART0_rx(void);
void UART0_tx(char data);
void UART0_print_1_byte_number(uint8_t n);
void UART0_print_string(char *str);
void UART0_print_float(float f);

#endif /* UART0_H_ */