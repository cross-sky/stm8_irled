/*
 * @Author: your name
 * @Date: 2021-08-08 11:41:04
 * @LastEditTime: 2021-08-08 12:59:57
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \stm8-irled\USER\ticktime4.h
 */

#ifndef __TICKTIM4_H
#define __TICKTIM4_H

#include "com_io.h"
#include "stm8s_conf.h"

void time_task_schedule(void);
void Tim4_Init(void);
void TIM4_IRQHandler(void) ;
uint8_t uvTimeFlag5s_get(void);
void vvTimeFlag5s_reset(void);

#endif
