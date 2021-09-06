/*
 * @Author: your name
 * @Date: 2021-08-11 22:32:28
 * @LastEditTime: 2021-08-13 22:55:58
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \stm8-irled\USER\uart.h
 */

#ifndef __UART_H
#define __UART_H

#include "stm8s_uart1.h"

void Init_UART1(void);
void send_data(uint8_t number);

#endif
