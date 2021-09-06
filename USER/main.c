/*
 * @Author: your name
 * @Date: 2021-08-08 10:02:31
 * @LastEditTime: 2021-08-13 23:11:17
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \stm8-irled\USER\main.c
 */

/******************** (C) COPYRIGHT  风驰电子嵌入式开发工作室 ********************
 * 文件名  ：main.c
 * 描述    ：流水灯    
 * 实验平台：风驰电子STM8开发板
 * 库版本  ：V2.0.0
 * 作者    ：ling_guansheng  QQ：779814207
 * 博客    ：
 *修改时间 ：2011-12-20
**********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm8s_conf.h"
#include "stm8s_clk.h"
#include "led.h"
#include "ticktim4.h"
#include "ir_adc.h"
#include "uart.h"

//#define Bit_or_Port  1  



/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int main(void)
{
  /* Infinite loop */

  /*设置内部高速时钟16M为主时钟*/ 
   CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
 
  /*!<Set High speed internal clock */
   //LED_Init();
   //SetLedOFF(); /* 让所有灯灭 */

   Tim4_Init();
   InitAD();
   Init_UART1();
   
  while (1)
  {
    time_task_schedule();
    IRLED_process_B();
    /*  添加你的代码  */
   // #ifdef Bit_or_Port
    //LED_ShowOneToOne();
    /*   以上操作的是最简单的单个LED的点亮 利用宏定义实现的  */
   // #else
   // LED_Display();
    /*   实现位移操作   */
    //#endif
   
  
  }
}





#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 风驰电子嵌入式开发工作室 *****END OF FILE****/
