/*
 * @Author: your name
 * @Date: 2021-08-08 17:27:14
 * @LastEditTime: 2021-08-19 23:33:01
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \stm8-irled\USER\ir_adc.h
 */

#ifndef __IR_ADC_H
#define __IR_ADC_H
#include "com_io.h"
#include "stm8s_conf.h"


// typedef enum{
//     IR1_POWER = 0,
//     IR2_POWER,
//     IR3_POWER,
//     IR4_POWER,
//     IR5_POWER,
//     IR6_POWER,
//     IR7_POWER,
//     IR8_POWER,
//     IR9_POWER,
//     IRx_POWER,
//     IR0_POWER,
//     IRn_POWER,
//     IR_MAX_POWER
// }IR_POWER_ENUM;


typedef enum{
    IR_PROC_POWER = 1,
    IR_PROC_ENON = 2,
    IR_PROC_END_DELAY_2MS,
    IR_PROC_ADC_START,
    IR_PROC_ADC_END
    
}IRLED_PROCESS;


void IR_init(void);

void IRLED_process(void);

uint8_t ir_adc_value_get(void);
void IRLED_process_B(void);



void InitAD(void);
uint16_t getADCValue(void);


#endif