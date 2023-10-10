#ifndef __UART_H
#define __UART_H
#include <STC15F2K60S2.H>

void UartInit(void);		//4800bps@12.000MHz
void UART_SendByte(unsigned char Byte);
void UART_SendString(char *String);

#endif