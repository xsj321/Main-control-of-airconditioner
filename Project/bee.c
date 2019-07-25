#include "bee.h"

 void BEE_init(void)
 {
	 GPIO_InitTypeDef GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    /* 初始化LED的GPIO管脚，配置为推挽输出 */				  //设置端口
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;   //定义了4个led灯管脚
														 	  //设置对端口的模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		  //设置为 推挽(Push-Pull)输出
														 	  //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //最大输出速度为50MHz

    GPIO_Init(GPIOD, &GPIO_InitStructure);	                  //调用GPIO初始化函数
		GPIO_ResetBits(GPIOD,GPIO_Pin_2);
 }
 
 void BEE_start(void)
 {
		GPIO_SetBits(GPIOD,GPIO_Pin_2);
 }
 
 void BEE_stop(void)
 {
		GPIO_ResetBits(GPIOD,GPIO_Pin_2);
 }