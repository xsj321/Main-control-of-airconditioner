#include "timer.h"

__IO uint32_t gTimingDelay;

/* SystemCoreClock / 1000    --> 1ms */
/* SystemCoreClock / 10000   --> 100us */
/* SystemCoreClock / 100000  --> 10us */
/* SystemCoreClock / 1000000 --> 1us */

void systick_init(void)
{
    while (SysTick_Config(SystemCoreClock / 1000000) == 1);
}

void timing_delay_decrement(void)
{
    if (gTimingDelay != 0x0)
    {
        gTimingDelay--;
    }
}

void SysTick_Handler(void)
{
    timing_delay_decrement();
}

void delay_us(__IO uint32_t n)
{
    gTimingDelay = n;
    while(gTimingDelay != 0);
}

void NVIC_TIM5Configuration(void)
{ 
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    /* Enable the TIM5 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

void TIM5_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* TIM5 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    /* 自动重装载寄存器周期的值(计数值) */ 
    TIM_TimeBaseStructure.TIM_Period = (10000 - 1);

	/* 累计 TIM_Period个频率后产生一个更新或者中断 */
    /* 这个就是预分频系数*/
    TIM_TimeBaseStructure.TIM_Prescaler = (7200 - 1);

    /* 采样分频 */
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;

    /* 向上计数模式 */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    /* 初始化定时器5 */
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

    /* 清除溢出中断标志 */
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);

    /* 溢出中断使能 */
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

    /* 计数器使能 */
    TIM_Cmd(TIM5, ENABLE); 
}