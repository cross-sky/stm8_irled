/*
 * @Author: your name
 * @Date: 2021-08-19 22:51:57
 * @LastEditTime: 2021-08-21 09:22:29
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \stm8-irled\USER\eeprom.c
 */
#include "eeprom.h"

#define EEPROM_ADDR                     0x4000 
#define EEPROM_DATA_OFFSET              0x00   

EEPDATA_STR _g_eepdata;
#define EEPHEAD1    0xaa
#define EEPHEAD2    0x55

// uint8_t const ref_adc_table1[IR_MAX_POWER] = { 140, 149, 123, 141,
//                                         151, 145, 110, 139,
//                                         144, 153, 135, 131};

uint8_t const ref_adc_table1[IR_MAX_POWER] = { 155, 150, 126, 146,
                                        163, 162, 96, 150,
                                        132, 137, 163, 151};
                                        


void EE_FLASH_WriteNByte(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t nByte)
{
    uint8_t i = 0;
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    while(FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET);
    for(i = 0; i < nByte; i++)
    {
        FLASH_ProgramByte((WriteAddr+i), pBuffer[i]);
    }
    FLASH_Lock(FLASH_MEMTYPE_DATA);
}

void EE_FLASH_ReadNByte(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t nByte)
{
    while(nByte--)
    {
        *pBuffer = FLASH_ReadByte(ReadAddr);
        ReadAddr++;
        pBuffer++;
    }
}

void EEPROM_data_init(void)
{
    uint8_t i;
    uint8_t sum = 0;
    uint8_t flag = 0;
    // read data
    EE_FLASH_ReadNByte((uint8_t*)(&_g_eepdata), EEPROM_ADDR+EEPROM_DATA_OFFSET, sizeof(_g_eepdata));

    // check data true
    // set data to table 
    if ((_g_eepdata.head1 == EEPHEAD1)  && (_g_eepdata.head2 == EEPHEAD2))
    {
        for ( i = 0; i < IR_MAX_POWER; i++)
        {
            sum += _g_eepdata.adc_table[i];
        }
        if (sum == _g_eepdata.sum)
        {
            flag = 1;
        }
    }

    //check data false
    // set defalt data to table
    if (flag == 0)
    {
        sum = 0;
        for ( i = 0; i < IR_MAX_POWER; i++)
        {
            _g_eepdata.adc_table[i] = ref_adc_table1[i];
            sum += ref_adc_table1[i];
        }
        _g_eepdata.head1 = EEPHEAD1;
        _g_eepdata.head2 = EEPHEAD2;
        _g_eepdata.sum = sum;
        EE_FLASH_WriteNByte((uint8_t*)(&_g_eepdata), EEPROM_ADDR+EEPROM_DATA_OFFSET, sizeof(_g_eepdata));
        
    }
}

uint8_t eep_get_adc_ref(uint8_t num)
{
    if (num < IR_MAX_POWER)
    {
        return _g_eepdata.adc_table[num];
    }
    else 
        return 145;
}

void Init_eeprom(void)
{
    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
    EEPROM_data_init();
}

/*
¶Á²Ù×÷£º
EE_FLASH_ReadNByte((uint8_t *)(&g_dev_info), EEPROM_ADDR+EEPROM_DATA_OFFSET, sizeof(g_dev_info));
Ð´²Ù×÷£º
EE_FLASH_WriteNByte((uint8_t *)(&g_dev_info), EEPROM_ADDR+EEPROM_DATA_OFFSET, sizeof(g_dev_info));
*/
