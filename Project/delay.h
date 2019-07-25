#ifndef __DELAY_H
#define __DELAY_H
#include "stm32f10x_conf.h"
void Delay_Init(void);
void Delay_ms(u16 nms);
void Delay_us(u32 nus);

void Delay_10ms(uint32_t nCount);
#endif
