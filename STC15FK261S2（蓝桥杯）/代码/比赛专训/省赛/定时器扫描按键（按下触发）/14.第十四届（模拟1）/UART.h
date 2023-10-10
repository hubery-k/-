#ifndef __UART_H
#define __UART_H
#include <STC15F2K60S2.H>
#include "Define.h"
void UartInit(void);		//9600bps@11.0592MHz
void UART_SendByte(uc Byte);
void UART_SendString(char *String);

#endif