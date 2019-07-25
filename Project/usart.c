/********************   (C) COPYRIGHT 2013 www.armjishu.com   ********************
 * 文件名  ：SZ_STM32F107VC_LIB.c
 * 描述    ：提供STM32F107VC神舟IV号开发板的库函数
 * 实验平台：STM32神舟开发板
 * 作者    ：www.armjishu.com 
**********************************************************************************/
#include "usart.h"
#include "systemUI.h"
#include "string.h"
u8 SendBuff[5200];

/**-------------------------------------------------------
  * @函数名 __SZ_STM32_COMInit
  * @功能   面向用户的STM32的USART初始化函数
  * @参数1  COM1  对应STM32的USART1 对应开发板上串口1
  *         COM2  对应STM32的USART2 对应开发板上串口2
  * @参数2  BaudRate 串口的波特率，例如"115200"
  * @返回值 无
***------------------------------------------------------*/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

void SZ_STM32_COMInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;  
    USART_InitTypeDef USART_InitStructure; 
  
	/*使能GPIOD端口时钟、复用功能时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE); 

	/*使能USART2时钟*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);   
 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;  
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  
    GPIO_Init(GPIOD, &GPIO_InitStructure);  

	/*串口2重映射*/
	GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);   

    USART_InitStructure.USART_BaudRate = 115200;  
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  
    USART_InitStructure.USART_StopBits = USART_StopBits_1;  
    USART_InitStructure.USART_Parity = USART_Parity_No;  
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  
    USART_Init(USART2, &USART_InitStructure);  
		NVIC_Configuration();
		
		// 使能串口接收中断
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	
	
		// 使能串口
    USART_Cmd(USART2, ENABLE);  
}



/**-------------------------------------------------------
  * @函数名  MYDMA_Config
  * @功能   初始化配置串口2的DMA传输方式
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void MYDMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	/*使能DMA时钟*/
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	
	
	/*设置DMA传输的外设地址*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;  
	
	/*设置DMA传输内存地址(要传输的变量的指针)*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SendBuff; 
	
	/*DMA传输方向：从内存到外设*/	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; 
	
	/*DMA传输的数据量*/
	DMA_InitStructure.DMA_BufferSize = 5200;  

	/*外设地址不增*/
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	
	/*内存地址递增*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 

	/*外设数据单位 8位*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  
	
	/*内存数据单位 8位*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
	
	/*DMA模式：一次传输*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	
	/*优先级：中*/
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	
	/*禁止内存到内存的传输	*/
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输

	/*配置DMA1的17通道*/	
	DMA_Init(DMA1_Channel7, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	
	DMA_Cmd(DMA1_Channel7, ENABLE);  //使能USART1 TX DMA1 所指示的通道 	  	
} 

void JSON_Sender(int NowHum,int NowTemp,int TargetTemp)

{
	printf("{\"temperature\":\"%d\",\"humidity\":\"%d\",\"TargetTemp\":\"%d\"}",NowTemp,NowHum,TargetTemp);
}



/*
    加入以下代码,支持printf函数,不需要选择use MicroLIB	  
*/
#ifndef MicroLIB
//#pragma import(__use_no_semihosting)             //没有实现fgetc时需要声明该参数   
/* 标准库需要的支持函数 使用printf()调试打印不需要实现该函数 */               
struct __FILE 
{ 
	int handle; 
    /* Whatever you require here. If the only file you are using is */    
    /* standard output using printf() for debugging, no file handling */    
    /* is required. */
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
/* 重定义fputc函数 如果使用MicroLIB只需要重定义fputc函数即可 */  
int fputc(int ch, FILE *f)
{
    /* Place your implementation of fputc here */

	 /* e.g. write a character to the USART */
    USART_SendData(USART2, (uint8_t) ch);

    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
    {}

    return ch;
}
/*
可以直接使用putchar
不需要再定义 int putchar(int ch)，因为stdio.h中有如下定义
 #define putchar(c) putc(c, stdout)
*/

int ferror(FILE *f) {  
    /* Your implementation of ferror */  
    return EOF;  
} 
#endif 

FILE __stdin;

int fgetc(FILE *fp)
{
	int ch = 0;
	
    while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
    {
    }

    ch = (int)USART2->DR & 0xFF;
	
    putchar(ch); //回显
	
	return ch;
}

/******************* (C) COPYRIGHT 2013 www.armjishu.com *****END OF FILE****/
