#ifndef __UART_H
#define __UART_H
#include <STC15F2K60S2.H>
#include "Define.h"

void Uart_Init(void);		//9600bps@12.000MHz
void Uart_SendByte(uc Byte);
void Uart_SendString(uc *str);
#endif