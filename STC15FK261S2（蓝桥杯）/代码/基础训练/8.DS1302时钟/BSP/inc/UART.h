#ifndef __UART_H
#define __UART_H
#include <STC15F2K60S2.H>

void UART_Init(void);
void UART_SendByte(unsigned char Byte);
#endif