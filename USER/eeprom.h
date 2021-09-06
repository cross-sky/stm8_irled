/*
 * @Author: your name
 * @Date: 2021-08-19 22:52:05
 * @LastEditTime: 2021-08-19 23:42:37
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \stm8-irled\USER\eeprom.h
 */

#ifndef __EEPROM_H
#define __EEPROM_H

#include "stm8s_flash.h"
#include "com_io.h"

typedef struct EEPDATA
{
    uint8_t head1;
    uint8_t head2;
    uint8_t adc_table[IR_MAX_POWER];
    uint8_t sum;
}EEPDATA_STR;


uint8_t eep_get_adc_ref(uint8_t num);
void Init_eeprom(void);


#endif
