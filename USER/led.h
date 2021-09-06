/*
 * @Author: your name
 * @Date: 2021-08-08 10:02:31
 * @LastEditTime: 2021-08-08 12:26:53
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \stm8-irled\USER\led.h
 */


#ifndef __LED_H
#define __LED_H
#include "stm8s_gpio.h"
#include "com_io.h"

#define ON  0
#define OFF 1
#define LED1(ON_OFF)  if(ON_OFF==ON)GPIO_WriteLow(TestLedPort, TestLedPin);\
                      else GPIO_WriteHigh(TestLedPort, TestLedPin)

// #define LED2(ON_OFF)  if(ON_OFF==ON)GPIO_WriteLow(GPIOD, GPIO_PIN_1);\
//                       else GPIO_WriteHigh(GPIOD, GPIO_PIN_1)

// #define LED3(ON_OFF)  if(ON_OFF==ON)GPIO_WriteLow(GPIOD, GPIO_PIN_2);\
//                       else GPIO_WriteHigh(GPIOD, GPIO_PIN_2)

// #define LED4(ON_OFF)  if(ON_OFF==ON)GPIO_WriteLow(GPIOD, GPIO_PIN_3);\
//                       else GPIO_WriteHigh(GPIOD, GPIO_PIN_3)
void LED_Init(void);
void SetLedOFF(void);
void LED_Display(void);
void Delay(u32 nCount);
void LED_ShowOneToOne(void);

void ir_led_task_testdisp(void);
void ir_led_resettestdisp(void);

#endif


