/*
 * @Author: your name
 * @Date: 2021-08-11 22:32:28
 * @LastEditTime: 2021-09-10 00:22:21
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \stm8-irled\USER\uart.h
 */

#ifndef __UART_H
#define __UART_H

#include "com_io.h"
#include "stm8s_conf.h"

void Init_UART1(void);
void send_data(uint8_t number);
void Gtx_busy_flag_set(uint8_t num);
uint8_t Gtx_busy_flag_get(void);
void Gtx_busy_flag_add(void);
void UARTx_setEXTI(void);
void Get_txbusy_handler(void);
void sends_adc_diff(uint16_t num);
void send_data_b(uint8_t number, uint16_t diff);
#endif
