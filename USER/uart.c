/*
 * @Author: your name
 * @Date: 2021-08-11 22:32:23
 * @LastEditTime: 2021-08-21 11:34:57
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \stm8-irled\USER\uart.c
 */
#include "uart.h"

uint8_t const sendcode[12] = {
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 11
};

void Init_UART1(void)
{
	UART1_DeInit();
	UART1_Init((u32)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TX_ENABLE);
//	UART1_Cmd(ENABLE);
}
void Send(uint8_t dat)
{
  while(( UART1_GetFlagStatus(UART1_FLAG_TXE)==RESET));
	
	UART1_SendData8(dat);
	
}

void send_data(uint8_t number)
{
    Send(0x0c);
    Send(sendcode[number]);
    Send(0x00);
    Send(0xcc);
}

