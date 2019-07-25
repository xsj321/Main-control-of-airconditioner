/********************   (C) COPYRIGHT 2013 www.armjishu.com   ********************
 * 文件名  ：SZ_STM32F107VC_LIB.h
 * 描述    ：提供STM32F107VC神舟IV号开发板的库函数
 * 实验平台：STM32神舟开发板
 * 作者    ：www.armjishu.com 
**********************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>

#define  DEBUG_USART_IRQ                USART2_IRQn

#ifdef __cplusplus
 extern "C" {
#endif 


//void __SZ_STM32_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);
void SZ_STM32_COMInit(void);

void MYDMA_Config(void);
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx);
void JSON_Sender(int NowHum,int NowTemp,int TargetTemp);

#ifdef __cplusplus
}
#endif
/******************* (C) COPYRIGHT 2013 www.armjishu.com *****END OF FILE****/

