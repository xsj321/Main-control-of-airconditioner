#ifndef __USER_CONFIG_H
#define __USER_CONFIG_H
#include "stm32f10x_conf.h"
#include "delay.h"
#include "107_32_Driver_IO16.h"
#include "led.h"
/*----------------------------USART----------------------------*/
/*使用串口之前先打开相应时钟
 * USART		TX		RX
   USART1       PA9     PA10
   USART2		PA2		PA3*/
#define GPIO_UsartPort	GPIOA
#define UsartPort   USART1
#define UsartTX	GPIO_Pin_9
#define UsartRX	GPIO_Pin_10
//#define LED3	GPIO_Pin_8
//#define LED4	GPIO_Pin_9
//#define LED1_ON()	GPIO_ResetBits(GPIO_LedPort,LED1)
//#define LED1_OFF()	GPIO_SetBits(GPIO_LedPort,LED1)
//#define LED2_ON()	GPIO_ResetBits(GPIO_LedPort,LED2)
//#define LED2_OFF()	GPIO_SetBits(GPIO_LedPort,LED2)
//#define LED3_ON()	GPIO_ResetBits(GPIO_LedPort,LED3)
//#define LED3_OFF()	GPIO_SetBits(GPIO_LedPort,LED3)
//#define LED4_ON()	GPIO_ResetBits(GPIO_LedPort,LED4)
//#define LED4_OFF()	GPIO_SetBits(GPIO_LedPort,LED4)

#endif
