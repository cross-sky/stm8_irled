/*
 * @Author: your name
 * @Date: 2021-08-08 11:15:26
 * @LastEditTime: 2021-09-15 23:24:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \stm8-irled\USER\com_io.h
 */
#ifndef __COM_IO_H
#define __COM_IO_H

#include "stm8s_conf.h"

#ifdef VSEDIT
#define __interrupt static
#endif

#define TestLedPin    GPIO_PIN_5
#define TestLedPort     GPIOD

// GPIOD
// adc1 channel input 4
#define IR_ADC_PORT     GPIOD
#define IR_ADC_PIN      GPIO_PIN_3
#define IR_ADC_CHANNEL  ADC1_CHANNEL_4

// MODEFY TO RXD FOR TEMP   21.8.8
#define IR_GDEN_PIN   GPIO_PIN_6 
//pd 1 swim
//#define IR_GDEN_PIN   GPIO_PIN_1


// tx set to exti
#define TX_PORT                    GPIOD 
#define TX_PIN                     GPIO_PIN_5  
 #define TX_MODE                    GPIO_MODE_IN_PU_IT  
//#define TX_MODE                    GPIO_MODE_IN_FL_IT  
  
  
#define TX_EXTI_PORT               EXTI_PORT_GPIOD  
#define TX_EXTI_SENSITIVITYT       EXTI_TLISENSITIVITY_FALL_ONLY  


/*
num1 - ir_en1   --PB5
num2 - ir_en2   --PB4
num3 - ir_en3   --PC3
num4 - ir_en4   --PC5
num5 - ir_en5   --PC4
num6 - ir_en6   --PD2
num7 - ir_en7   --PC6
num8 - ir_en8   --PC7
num9 - ir_en9   --PA1
num* - ir_en10  --PA3
num0 - ir_en11  --PA2
num# - ir_en12  --PD4
*/
//GPIOB LED POWER EN
#define IR_NUM1_GBIO5_POWER     GPIO_PIN_5
#define IR_NUM2_GBIO4_POWER     GPIO_PIN_4
#define IR_NUM3_GCIO3_POWER     GPIO_PIN_3
#define IR_NUM4_GCIO5_POWER     GPIO_PIN_5
#define IR_NUM5_GCIO4_POWER     GPIO_PIN_4
#define IR_NUM6_GDIO2_POWER     GPIO_PIN_2
#define IR_NUM7_GCIO6_POWER     GPIO_PIN_6
#define IR_NUM8_GCIO7_POWER     GPIO_PIN_7
#define IR_NUM9_GAIO1_POWER     GPIO_PIN_1
#define IR_NUMX_GAIO3_POWER     GPIO_PIN_3
#define IR_NUM0_GAIO2_POWER     GPIO_PIN_2
#define IR_NUMN_GDIO4_POWER     GPIO_PIN_4

typedef enum{
    IR1_POWER = 0,
    IR2_POWER,
    IR3_POWER,
    IR4_POWER,
    IR5_POWER,
    IR6_POWER,
    IR7_POWER,
    IR8_POWER,
    IR9_POWER,
    IRx_POWER,
    IR0_POWER,
    IRn_POWER,
    IR_MAX_POWER
}IR_POWER_ENUM;


void IRLED_process_B(void);

#endif
