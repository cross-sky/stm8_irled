/*
 * @Author: your name
 * @Date: 2021-08-08 11:40:46
 * @LastEditTime: 2021-09-08 22:40:45
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \stm8-irled\USER\ticktim4.c
 */
#include "ticktim4.h"
#include "led.h"
#include "ir_adc.h"
#include "uart.h"

uint8_t g_sys_timer=0;
uint8_t g_sys_timer1ms=0;
uint8_t g_sys_timer10ms = 0;
uint8_t g_sys_timer1s = 0;

void Tim4_Init(void)
{
  TIM4_DeInit();
  TIM4_TimeBaseInit(TIM4_PRESCALER_64,250-1);//64分频，向上计数，16M/64/250 = 1ms计数完了触发中断
  TIM4_ARRPreloadConfig(ENABLE);//使能自动重装
  TIM4_ITConfig(TIM4_IT_UPDATE , ENABLE);//数据更新中断
  TIM4_Cmd(ENABLE);//开定时器
   __enable_interrupt(); 
}


void test_adc(void)
{
    uint16_t _adc = 0;

    //mytest
    _adc = getADCValue();

    if (_adc > 500) 
    {
        GPIO_WriteHigh(GPIOA, GPIO_PIN_1);
    }
    else
    {
        GPIO_WriteLow(GPIOA, GPIO_PIN_1);
        //txsend 
        //ir_led_resettestdisp();
    }
}

void time_task_schedule(void)
{
    
    // switch(g_sys_timer)
    // {
    //     case 
    // }

    // 1 ms cycle
    if(g_sys_timer1ms == 1)
    {
        g_sys_timer1ms = 0;
        Gtx_busy_flag_add();
    }

    if(g_sys_timer >= 10)
    {   // 10ms
        g_sys_timer = 0;
        g_sys_timer10ms++;

        //func
        // no led flash
        //ir_led_task_testdisp();

    }
    if(g_sys_timer10ms >= 10)
    {   
        // 100ms
        g_sys_timer10ms = 0;
        g_sys_timer1s++;
        
        //func

    }
    if(g_sys_timer1s >= 10)
    {   
        // 1s
        g_sys_timer1s = 0;

        //func
        
        //mytest
        // test_adc();
        


    }
}


void TIM4_IRQHandler(void) 
{
    g_sys_timer++;
    g_sys_timer1ms=1;
    TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
}

