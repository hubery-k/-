#ifndef __UART_H
#define __UART_H
#include <STC15F2K60S2.H>
#include "Define.h"
void UartInit(void);		//9600bps@12.000MHz
void Send_String(char* str);
void Send_Byte(uc Byte);
#endif