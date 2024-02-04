/*
 * functions.h
 *
 * Created: 2022-09-25 오후 5:19:09
 *  Author: TeddyKim
 */ 
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
//UART
void UART_init();
void UART_TX(unsigned char data);
void UART_TX_string(char *str);
unsigned char UART_RX();
//ADC
uint16_t ADC_init(uint8_t ch);

#endif /* FUNCTIONS_H_ */