#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void SZ_STM32_SysTickInit(uint32_t HzPreSecond);
//void Delay_us(__IO u32 nTime);
void Delay_mss(__IO u32 nTime);
void Delay_s(__IO u32 nTime);

#endif 



