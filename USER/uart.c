/*
 * @Author: your name
 * @Date: 2021-08-11 22:32:23
 * @LastEditTime: 2021-09-10 00:58:13
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \stm8-irled\USER\uart.c
 */
#include "uart.h"

uint8_t const sendcode[12] = {
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 11
};

uint8_t g_tx_busy_flag = 0;

void Gtx_busy_flag_set(uint8_t num)
{
  g_tx_busy_flag = num;
}

uint8_t Gtx_busy_flag_get(void)
{
  return g_tx_busy_flag;
}

void Gtx_busy_flag_add(void)
{
  g_tx_busy_flag++;
}


void Init_UART1(void)
{
  GPIO_Init(TX_PORT, TX_PIN, GPIO_MODE_IN_FL_NO_IT);
	UART1_DeInit();
	UART1_Init((u32)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TX_ENABLE);
  disableInterrupts();
//	UART1_Cmd(ENABLE);
}

void UARTx_setEXTI(void)
{
  disableInterrupts(); // disable globe interrupt
  UART1_DeInit();
  GPIO_Init(TX_PORT, TX_PIN, TX_MODE);  
  
  EXTI_DeInit();  
  //EXTI_SetPortSensitivity(TX_EXTI_PORT, TX_EXTI_SENSITIVITYT);
  EXTI_SetExtIntSensitivity(TX_EXTI_PORT, EXTI_SENSITIVITY_FALL_ONLY);
  enableInterrupts();
}

void Get_txbusy_handler(void)
{
  if (GPIO_ReadInputPin(TX_PORT, TX_PIN) != RESET)
  {
      Gtx_busy_flag_set(0);
      EXTI_DeInit();
     disableInterrupts();
  }
  //GPIO_ReadInputPin(TX_EXTI_PORT, TX_PIN)
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

void send_data_b(uint8_t number, uint16_t diff)
{
    Send(0x0c);
    Send(sendcode[number]);
    Send(0x00);
    Send(0xcc);
    Send(diff >> 8);
    Send(diff & 0xff);
    //Send(0x00);
    //Send(0x00);
}

void sends_adc_diff(uint16_t num)
{
    Send(num >> 8);
    Send(num & 0xff);
}

void send_num_to_str(uint16_t num)
{
  uint8_t i=0;
  char buf[2] = "";
  
  // while (num > 0)
  // {
  //   //buf[i++] = (num % 10) + '0';
  //   buf[i++] = num % 100;
  //   num = num / 100;
  // }

  buf[1] = num / 100;
  buf[0] = num % 100;

  Send(buf[1]);
  Send(buf[0]);
  // Send('\r');
  // Send('\n');
}