/*
 * @Author: your name
 * @Date: 2021-08-08 10:02:31
 * @LastEditTime: 2021-08-08 18:31:54
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \stm8-irled\USER\led.c
 */


/******************** (C) COPYRIGHT  ��۵���Ƕ��ʽ���������� ***************************
 * �ļ���  ��led.c
 * ����    ��IO�����ú�����   
 * ʵ��ƽ̨����۵���STM8������
 * ��汾  ��V2.0.0
 * ����    ��ling_guansheng  QQ��779814207
 * ����    ��
 *�޸�ʱ�� ��2011-12-20

  ��۵���STM8������Ӳ������
    |--------------------|
    |  LED1-PD0          |
    |  LED2-PD1          |
    |  LED3-PD2          |
    |  LED4-PD3          |
    |--------------------|

****************************************************************************************/

#include "led.h"


uint16_t ir_led_test_display = 0;


void LED_Init(void)
{
    GPIO_Init(TestLedPort,(TestLedPin),\
              GPIO_MODE_OUT_PP_HIGH_FAST );//����LED�Ĺܽŵ�ģʽ
    LED1(OFF);
  
}

void SetLedOFF(void)
{
    GPIO_Write(GPIOD, 0xff);
}
// void LED_Display(void)
// {
//   u8 PortVal;
//   for(PortVal=0;PortVal<4;PortVal++)
//   {
//       if(PortVal==2)
//         GPIOD->ODR =(u8)~(1<<(PortVal+1));
//       else if(PortVal==3)
//         GPIOD->ODR =(u8)~(1<<(PortVal-1));
//       else
//       GPIOD->ODR =(u8)(~(1<<PortVal));
      
//       Delay(0x1ffff);
//   }
// }

void LED_ShowOneToOne(void)
{
    LED1(ON);
    // LED2(OFF);
    // LED3(OFF);
    // LED4(OFF);
    Delay(0x1ffff);
    LED1(OFF);
    // LED2(ON);
    // LED3(OFF);
    // LED4(OFF);
    // Delay(0x1ffff);
    // LED1(OFF);
    // LED2(OFF);
    // LED3(OFF);
    // LED4(ON);
    // Delay(0x1ffff);
    // LED1(OFF);
    // LED2(OFF);
    // LED3(ON);
    // LED4(OFF);
    Delay(0x1ffff);
}
void Delay(u32 nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}

void ir_led_resettestdisp(void)
{
  // almost 10*10ms
    ir_led_test_display = 10;
}

void irled_testdisp_on(uint8_t value)
{
    if (value)
        //led on 
        LED1(ON);
    else
        LED1(OFF);
}

void ir_led_task_testdisp(void)
{
  if (ir_led_test_display == 0)
      irled_testdisp_on(0);
  else
      irled_testdisp_on(ir_led_test_display--);
}



/******************* (C) COPYRIGHT ��۵���Ƕ��ʽ���������� *****END OF FILE****/