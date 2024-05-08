#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/

void uart_init(u32 bound);
void uart4_init(u32 bound);
void uart4_sendchar(u8 p);
void uart5_init(u32 bound);
void uart5_sendchar(u8 p);
void usart2_sendchar(u8 p);
void uart2_init(u32 bound);
void usart1_sendchar(u8 p);
#endif


