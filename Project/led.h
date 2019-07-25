#ifndef	__LED_H
#define __LED_H
#include "stm32f10x_conf.h"
/*------------------------LED---------------------------*/
#define RCC_APB2Periph_LED	RCC_APB2Periph_GPIOD
#define GPIO_LedPort	GPIOD
#define LED1	GPIO_Pin_2
#define LED2	GPIO_Pin_3
#define LED3	GPIO_Pin_4
#define LED4	GPIO_Pin_7
#define LED1_ON()	GPIO_ResetBits(GPIO_LedPort,LED1)
#define LED1_OFF()	GPIO_SetBits(GPIO_LedPort,LED1)
#define LED2_ON()	GPIO_ResetBits(GPIO_LedPort,LED2)
#define LED2_OFF()	GPIO_SetBits(GPIO_LedPort,LED2)
#define LED3_ON()	GPIO_ResetBits(GPIO_LedPort,LED3)
#define LED3_OFF()	GPIO_SetBits(GPIO_LedPort,LED3)
#define LED4_ON()	GPIO_ResetBits(GPIO_LedPort,LED4)
#define LED4_OFF()	GPIO_SetBits(GPIO_LedPort,LED4)

/*------------------------BUTTON-----------------------*/
#define RCC_APB2Periph_BUTTON	RCC_APB2Periph_GPIOD
#define GPIO_ButtonPort	GPIOD
#define BUTTON1	GPIO_Pin_11


//IO¿ÚµØÖ·Ó³Éä
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
//#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
//#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
//#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
//#define PFout(n)	*((volatile unsigned long *)(0x42000000+((GPIOF_ODR_Addr-0x40000000)<<5)+(n<<2)))


#endif
