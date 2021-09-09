/*
 * @Author: your name
 * @Date: 2021-08-08 17:27:06
 * @LastEditTime: 2021-09-10 01:08:30
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \stm8-irled\USER\ir_adc.c
 */

#include "ir_adc.h"
#include "led.h"
#include "uart.h"
#include "eeprom.h"

uint8_t ir_timeflag = 0;        // time flag
uint8_t ir_procflag = 0;        // process flag
uint8_t all_key_scan_flag = 1;      //init
uint8_t current_ir_scan = 0;        //  key of current scanning

uint16_t last_key_table = 0;        // last key scan result table
uint16_t key_long_release = 0;      // key release time big than 1s

// uint16_t adc_value=0;               // adc value
uint8_t adc_count=0;                // adc count 
uint8_t t_delaycount = 0;           // next delay adc time

static uint8_t ir_adc_scan200usflag = 0;        // multilate key scan flag
//static uint8_t ir_adc_scan2msflag = 0;          // single key scan flag

static uint8_t key_scan_mode = 0;

uint8_t adc_send_value = 0;

static uint16_t g_adc_envir = 0;
static uint16_t g_adc_envir_reflec = 0;

#define IR_EN_ON    (GPIOD->ODR &= (uint8_t)(~IR_GDEN_PIN)) 
#define IR_EN_OFF   (GPIOD->ODR |= (uint8_t)IR_GDEN_PIN)

// adc count
#define ENV_IR_COUNTS  5
#define ENV_PLUS_REFLECT_COUNTS 10

// uint8_t ref_adc_table[IR_MAX_POWER] = { 140, 149, 123, 141,
//                                         151, 145, 110, 139,
//                                         144, 153, 135, 131};

// uint8_t ref_adc_table[IR_MAX_POWER] = { 176, 171, 145, 164,
//                                         185, 182, 110, 170,
//                                         147, 155, 191, 191};

// uint8_t ref_adc_table[IR_MAX_POWER] = { 140, 151, 128, 145,
//                                         167, 150, 101, 146,
//                                         123, 152, 163, 136};
uint8_t ref_adc_table[IR_MAX_POWER] = { 150, 161, 108, 140,
                                        182, 135, 126, 174,
                                        128, 152, 158, 138};
                                        
// uint8_t ref_adc_res_table[IR_MAX_POWER] = { 26, 19, 11, 30,
//                                         26, 19, 13, 24,
//                                         32, 25, 18, 11};

uint8_t ref_adc_res_table[IR_MAX_POWER] = { 27, 20, 20, 21,
                                        18, 27, 10, 13,
                                        25, 27, 25, 14};

void IRLED_delay(uint16_t us);


void InitAD(void)
{
    IRLED_delay(2000);
    // power io init
    GPIO_Init(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIOA->ODR |= (uint8_t)(GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIO_Init(GPIOB, GPIO_PIN_4|GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIOB->ODR |= (uint8_t)(GPIO_PIN_4|GPIO_PIN_5);
    GPIO_Init(GPIOC, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIOB->ODR |= (uint8_t)(GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
    GPIO_Init(GPIOD, GPIO_PIN_2|GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIOD->ODR |= (uint8_t)(GPIO_PIN_2|GPIO_PIN_4);

    // adc io init
    GPIO_Init(IR_ADC_PORT, IR_ADC_PIN, GPIO_MODE_IN_FL_NO_IT);
    /* De-Init ADC peripheral*/
    ADC1_DeInit();
    //通道初始化
    
    ADC1_Init(ADC1_CONVERSIONMODE_SINGLE,
            (ADC1_Channel_TypeDef)IR_ADC_CHANNEL,
            ADC1_PRESSEL_FCPU_D18,
            ADC1_EXTTRIG_TIM,
            DISABLE,
            ADC1_ALIGN_RIGHT,
            (ADC1_SchmittTrigg_TypeDef)IR_ADC_CHANNEL,
            DISABLE);
    ADC1_Cmd(ENABLE);//启用ADC1


    //test ir to low state
    GPIO_Init(GPIOD, IR_GDEN_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
}
 
void AD_Start(void)
{
  //ADC1_ScanModeCmd(ENABLE);//启用扫描模式
  //ADC1_DataBufferCmd(ENABLE);//启用缓存寄存器存储数据
  //ADC1_ITConfig(ADC1_IT_EOCIE,DISABLE);//关闭中断功能 
  //ADC1_Cmd(ENABLE);//启用ADC1
 // ADC1_StartConversion();//开始转换*/
}
 
uint16_t getADCValue(void)
{
//   InitAD(IR_ADC_CHANNEL);  //channel 4
//   AD_Start();
    ADC1_StartConversion();//开始转换*/
    while(ADC1_GetFlagStatus(ADC1_FLAG_EOC) == RESET);
    ADC1_ClearFlag(ADC1_FLAG_EOC);  //软件清除
    return ADC1_GetConversionValue();
}


// void InitAD(void)
// {
//   ADC1->CR1 = 0x01;     //设置ADC不分频、单次转换
//   ADC1->CR2 = 0x08;     //设置不外部触发也不开启中断、数据右对齐、不开启扫描
//   ADC1->CSR = 0x06;     //清除转换标志 设置通道5为AD口
// }
 
// void setADCChannel(uint8_t channel)
// {
//   ADC1->CSR = (channel & 0x0F); //写入转换通道
// }
 
// uint16_t getADCValue(void)
// {
//   uint16_t adc = 0x00;
//   ADC1->CR1 |= 0x01;                                    //启动ADC开始转换
//   while((adc & 0x80) == 0x00){adc = ADC1->CSR;}         //等待转换完成
//   adc = ADC1->DRL;
//   adc |= (ADC1->DRH << 0x08);                           //获取ADC值
//   return adc;
// }


uint8_t ir_key_flag_get(uint16_t keytab, uint8_t keynum)
{
    return 0x01 & (keytab >> keynum);
}

uint8_t ir_last_key_getstatus(uint8_t keynum)
{
    if (keynum < IR_MAX_POWER )
        return ir_key_flag_get(last_key_table, keynum);
    else
        return 0;
}

void ir_key_flag_set(uint16_t* keytab, uint8_t keynum, uint8_t value)
{
    if (value == 1)
        *keytab |= ( 1 <<  keynum);
    else 
        *keytab &= (~( 1 <<  keynum));
}

void ir_last_key_setstatus(uint8_t keynum, uint8_t value)
{
    if (keynum < IR_MAX_POWER )
    {
        ir_key_flag_set(&last_key_table, keynum, value);
    }
}

uint8_t ir_release_key_getstatus(uint8_t keynum)
{
    if (keynum < IR_MAX_POWER )
        return ir_key_flag_get(key_long_release, keynum);
    else
        return 0;
}

void ir_release_key_setstatus(uint8_t keynum, uint8_t value)
{
    if (keynum < IR_MAX_POWER )
    {
        ir_key_flag_set(&key_long_release, keynum, value);
    }
}


void IRLED_poweron(uint8_t num)
{
    //PBout(num) = IR_POWER_ON;
    //IR1_POWER_ON;
    switch(num)
    {
        case IR1_POWER:
            GPIO_WriteLow(GPIOB, IR_NUM1_GBIO5_POWER);
            break;
        case IR2_POWER:
            GPIO_WriteLow(GPIOB, IR_NUM2_GBIO4_POWER);
            break;
        case IR3_POWER:
            GPIO_WriteLow(GPIOC, IR_NUM3_GCIO3_POWER);
            break;
        case IR4_POWER:
            GPIO_WriteLow(GPIOC, IR_NUM4_GCIO5_POWER);
            break;
        case IR5_POWER:
            GPIO_WriteLow(GPIOC, IR_NUM5_GCIO4_POWER);
            break;
        case IR6_POWER:
            GPIO_WriteLow(GPIOD, IR_NUM6_GDIO2_POWER);
            break;
        case IR7_POWER:
            GPIO_WriteLow(GPIOC, IR_NUM7_GCIO6_POWER);
            break;
        case IR8_POWER:
            GPIO_WriteLow(GPIOC, IR_NUM8_GCIO7_POWER);
            break;
        case IR9_POWER:
            GPIO_WriteLow(GPIOA, IR_NUM9_GAIO1_POWER);
            break;
        case IRx_POWER:
            GPIO_WriteLow(GPIOA, IR_NUMX_GAIO3_POWER);
            break;
        case IR0_POWER:
            GPIO_WriteLow(GPIOA, IR_NUM0_GAIO2_POWER);
            break;
        case IRn_POWER:
            GPIO_WriteLow(GPIOD, IR_NUMN_GDIO4_POWER);
            break;
        default:
            break;
    }
}

void IRLED_poweroff(uint8_t num)
{
    switch(num)
    {
        case IR1_POWER:
            GPIO_WriteHigh(GPIOB, IR_NUM1_GBIO5_POWER);
            break;
        case IR2_POWER:
            GPIO_WriteHigh(GPIOB, IR_NUM2_GBIO4_POWER);
            break;
        case IR3_POWER:
            GPIO_WriteHigh(GPIOC, IR_NUM3_GCIO3_POWER);
            break;
        case IR4_POWER:
            GPIO_WriteHigh(GPIOC, IR_NUM4_GCIO5_POWER);
            break;
        case IR5_POWER:
            GPIO_WriteHigh(GPIOC, IR_NUM5_GCIO4_POWER);
            break;
        case IR6_POWER:
            GPIO_WriteHigh(GPIOD, IR_NUM6_GDIO2_POWER);
            break;
        case IR7_POWER:
            GPIO_WriteHigh(GPIOC, IR_NUM7_GCIO6_POWER);
            break;
        case IR8_POWER:
            GPIO_WriteHigh(GPIOC, IR_NUM8_GCIO7_POWER);
            break;
        case IR9_POWER:
            GPIO_WriteHigh(GPIOA, IR_NUM9_GAIO1_POWER);
            break;
        case IRx_POWER:
            GPIO_WriteHigh(GPIOA, IR_NUMX_GAIO3_POWER);
            break;
        case IR0_POWER:
            GPIO_WriteHigh(GPIOA, IR_NUM0_GAIO2_POWER);
            break;
        case IRn_POWER:
            GPIO_WriteHigh(GPIOD, IR_NUMN_GDIO4_POWER);
            break;
        default:
            break;
    }
}

void IRLED_delay(uint16_t us)
{
    uint8_t i = 20;
    if (us == 0)
    {
        ir_timeflag = 1;
        return;
    }
        
    while(us--)
    {
			i = 10;
        while(i--)
            ;
    }
    ir_timeflag = 1;
}


/**
 * @description: adc scanflag and scan counts
 * @param {uint8_t} scanflag
 * @param {uint8_t} numbers
 * @return {*}
 */
void ir_adc_scan_200us_b(uint8_t scanflag, uint8_t numbers)
{
    // check release
    uint16_t  _adc = 0;
    uint8_t _adc_count = 0;
    if (scanflag)
    {
        while(_adc_count < numbers)
        {
             _adc += getADCValue();
             IRLED_delay(1);
             _adc_count++;
        }

        if (numbers == ENV_IR_COUNTS)
        {
            // envir adc
            g_adc_envir = _adc / numbers;
        }
        else
        {
            g_adc_envir_reflec = _adc / numbers;
        }
        
        // adc_value = getADCValue(IR_CHANNEL_ADC);
        // if (adc_value > 2234)
        //     adc_count++;
        // iradc_delay30us(30);

        // t_delaycount++;

        // if (t_delaycount > 30 || adc_count >20)
        // {

        //     ir_timeflag = 1;
        //     t_delaycount = 0;
        // }
    }
}

void ir_adc_scan_1ms_b(uint8_t scanflag , uint8_t numbers)
{
    uint16_t  _adc = 0;
    //uint8_t  _adc_diff = 0;
    // check release
    if (scanflag)
    {
        _adc = getADCValue();
        // if (adc_value > 2234)
        //     adc_count++;
        IRLED_delay(1);

        t_delaycount++;

       // if (t_delaycount > 30 || adc_count >20)
       if (t_delaycount > 30)
        {
            ir_timeflag = 1;
            t_delaycount = 0;
        }
    }
}

uint16_t iradc_get_press_threshold(uint16_t adc_env)
{
    // 1024 / 3.3 = 310.3
    // env_adc > 2v
    if (adc_env >= 620)
    {
        // 310* 0.52
        return 161;
    }
    // env_adc < 1v
    else if (adc_env <= 310)
    {
        return 140;
    }
    else 
        return 140 + (adc_env - 310) * 0.06;
}

uint8_t ir_adc_get_ref_value(uint8_t ir_number)
{
    if (ir_number < IR_MAX_POWER)
    {
        return ref_adc_table[ir_number];
    }
    else
    {
        return 150;
    }  
}

uint8_t ir_adc_get_release_ref_value(uint8_t ir_number)
{
    if (ir_number < IR_MAX_POWER)
    {
        return ref_adc_table[ir_number] - ref_adc_res_table[ir_number];
    }
    else
    {
        return 20;
    }  
}


uint16_t iradc_get_release_threshold(uint16_t adc_env)
{
    // 1024 / 3.3 = 310.3
    // env_adc > 2v
    if (adc_env >= 620)
    {
        // 310* 0.50
        return 155;
    }
    // env_adc < 1v
    else if (adc_env <= 310)
    {
        return 140;
    }
    else 
        return 140 + (adc_env - 310) * 0.06;
}

/**
 * @description: 
 * each channel scan twice.
 * adc1 means envir light adc
 * adc2 means envir + reflect light adc
 * @param {uint8_t} switchflag
 * @return {*}
 */
void ir_mul_key_scan_b(uint8_t switchflag)
{
    uint8_t key_valid = 0;
    static uint8_t key_send = 0;
    
    //difference between two adc
    static uint16_t _adc_diff = 0;

    ir_timeflag = 0;

    switch (ir_procflag)
    {
    case IR_PROC_POWER:
        IRLED_poweron(current_ir_scan);
        IR_EN_OFF;
        //ir_timeflag = 0;
        IRLED_delay(150);

        ir_adc_scan200usflag = 1;
        // scan envir adc
        ir_adc_scan_200us_b(ir_adc_scan200usflag, ENV_IR_COUNTS);
        IRLED_delay(0);

        ir_procflag = IR_PROC_ENON;
        break;

    case IR_PROC_ENON:
        IR_EN_ON;
        //TIME 300US
        ir_procflag = IR_PROC_ADC_START;
        //ir_timeflag = 0;
        IRLED_delay(150);
        break;

    case IR_PROC_ADC_START:
        ir_adc_scan200usflag = 1;
        ir_adc_scan_200us_b(ir_adc_scan200usflag, ENV_PLUS_REFLECT_COUNTS);
        IRLED_delay(0);
        ir_procflag = IR_PROC_ADC_END;
        // wait adc check 15 times
        break;
    case IR_PROC_ADC_END:
        //ir_timeflag = 0;
        //ir_adc_scan200usflag = 0;

        // compare value between envir_adc and envir_plus_reflec
        if (g_adc_envir > g_adc_envir_reflec)
        {
            _adc_diff = g_adc_envir - g_adc_envir_reflec;
            // difference is 1.0v means key was pressed
            // 1024/3.3 = 310

            //if (_adc_diff > 270) 

            //if (_adc_diff > iradc_get_press_threshold(g_adc_envir))
            if (_adc_diff > (ir_adc_get_ref_value(current_ir_scan) - 1))
            //if (_adc_diff > eep_get_adc_ref(current_ir_scan))
            {
                key_valid = 1;
                if (ir_last_key_getstatus(current_ir_scan))
                {
                    if(ir_release_key_getstatus(current_ir_scan) == 0)
                    {
                        key_send = 1;
                        //ir_led_testdisp(current_ir_scan);
                        //ir_led_resettestdisp();


                        // before send check tx_busy_flag
                        if (Gtx_busy_flag_get() >= 25)
                        {
                            // change to sene mode 
                            Init_UART1();
                            //IRLED_delay(10);
                            //send_data(current_ir_scan);
                            send_data_b(current_ir_scan, _adc_diff);    
                           //sends_adc_diff(_adc_diff);
                            // after send , change to exti mode
                            IRLED_delay(500);
                            UARTx_setEXTI();
                        }
                        else{
                            IRLED_delay(0);
                            break;
                        }

                    }
                }
            }
            else
            {
                // release key 
                ir_release_key_setstatus(current_ir_scan, 0);
            }
        }
        else
        {
            // may be was under the sun, two values are zero and equal
            // do nothing 
        }
                
        ir_last_key_setstatus(current_ir_scan, key_valid);
        ir_procflag = IR_PROC_END_DELAY_2MS;
        //no delay, set flag with mannel
        ir_timeflag = 1;
        break;
    case IR_PROC_END_DELAY_2MS:
        //ir_timeflag = 0;
        IR_EN_OFF;
        IRLED_poweroff(current_ir_scan);

        if (key_send)
        {
            key_send = 0;
            //send data TODO

            IRLED_delay(100);

            // change to sing scan release mode
            ir_procflag = IR_PROC_POWER;
            key_scan_mode = 1;

            break;

        }

        //current_ir_scan++;
        if (current_ir_scan++ >= IR_MAX_POWER )
        {
            current_ir_scan = 0;
            IRLED_delay(600);

        }
        else 
        {
            IRLED_delay(0);
        }
                    
        ir_procflag = IR_PROC_POWER;
        break;
    default:
        break;
    }
}


void IR_single_key_scan_b(uint8_t switchflag)
{
    
    static uint16_t ir_release_count = 0;
    static uint8_t key_exit = 0;

    //difference between two number
    uint16_t _adc_diff = 0;
    
    ir_timeflag = 0;
    
    switch (switchflag)
    {
    case IR_PROC_POWER:
            IRLED_poweron(current_ir_scan);
            IR_EN_OFF;
            //ir_timeflag = 0;
            IRLED_delay(150);

            ir_adc_scan200usflag = 1;
            // scan envir adc
            ir_adc_scan_200us_b(ir_adc_scan200usflag, ENV_IR_COUNTS);
            IRLED_delay(0);

            ir_release_count++;
            ir_procflag = IR_PROC_ENON;
            break;

    case IR_PROC_ENON:
            IR_EN_ON;
            //TIME 300US
            ir_procflag = IR_PROC_ADC_START;
            //ir_timeflag = 0;
            IRLED_delay(150);
            break;

    case IR_PROC_ADC_START:
            //ir_timeflag = 0;
            //ir_adc_scan2msflag = 1;

            ir_adc_scan_200us_b(ir_adc_scan200usflag, ENV_PLUS_REFLECT_COUNTS);
            IRLED_delay(0);

            ir_procflag = IR_PROC_ADC_END;
            // wait adc check 250 times
            break;
    case IR_PROC_ADC_END:
            //ir_timeflag = 0;
            //ir_adc_scan2msflag = 0;
            if (ir_release_count > 300 )
            {
                //exit immediately
                //key would not be sent in next
                // to multilate scan
                ir_release_key_setstatus(current_ir_scan, 1);
                key_exit= 1;
            }

            if (g_adc_envir > g_adc_envir_reflec)
            {
                _adc_diff = g_adc_envir - g_adc_envir_reflec;
                // difference is 0.3v means key was released
                // 1024/3.3 = 310
                //if (_adc_diff < 100) 
                //if (_adc_diff < iradc_get_release_threshold(g_adc_envir))
                //if (_adc_diff < (ir_adc_get_ref_value(current_ir_scan) - 20))
                //if (_adc_diff < eep_get_adc_ref(current_ir_scan) - 20)
                if (_adc_diff < ir_adc_get_release_ref_value(current_ir_scan)) 
                {
                    //release time less  than 1s
                    // clear key valid
                    ir_release_key_setstatus(current_ir_scan, 0);
                    ir_last_key_setstatus(current_ir_scan, 0);
                    key_exit= 1;
                }
                else
                {
                    //  key  was not released
                    //ir_release_key_setstatus(current_ir_scan, 0);
                }
            }
            else
            {
                // key is still be pressed
                // do nothing 
            }

            // if (adc_count > 20)
            // {
            //     // release time less  than 1s
            //     // clear key valid
            //     ir_release_key_setstatus(current_ir_scan, 0);
            //     ir_last_key_setstatus(current_ir_scan, 0);
            //     key_exit= 1;
            // }
            
            ir_procflag = IR_PROC_END_DELAY_2MS;
            adc_count = 0;
            //no delay, set flag with mannel
            // ir_timeflag = 1;
            IRLED_delay(0);

            break;

    case IR_PROC_END_DELAY_2MS:
            //ir_timeflag = 0;
            IR_EN_OFF;
            IRLED_poweroff(current_ir_scan);

            if (key_exit)
            {
                key_exit = 0;
                // reset current_ir_key 
                current_ir_scan = 0;
                ir_release_count = 0;
                IRLED_delay(600);
                key_scan_mode = 0;
            }
            else 
            {
                IRLED_delay(0);
            }
						
            ir_procflag = IR_PROC_POWER;
    default:
        break;
    }
}

void IRLED_process_B(void)
{

    if (ir_timeflag)
    {
        if (key_scan_mode == 1)
        {
            IR_single_key_scan_b(ir_procflag);
        }
        else
        {
            ir_mul_key_scan_b(ir_procflag);
        }
    }
    else
    {
        if (all_key_scan_flag)
        {
            current_ir_scan = 0;
            all_key_scan_flag = 0;
            //start scan
            // IRLED_poweron(0);
            // IRLED_delay(300);
            // IR_EN_OFF;
            // ir_procflag = IR_PROC_ENON;

            ir_procflag = IR_PROC_POWER;
            IRLED_delay(0);
        }
    }

    // ir_adc_scan_200us(ir_adc_scan200usflag);
    // ir_adc_scan_1ms(ir_adc_scan2msflag);
}