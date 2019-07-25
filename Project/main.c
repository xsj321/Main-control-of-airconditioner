/******************** (C) COPYRIGHT 2013 www.armjishu.com  ********************
 * 文件名  ：main.c
 * 描述    ：实现STM32F107VC神舟IV号开发板的TFT彩屏显示中文英文字符实验
 * 实验平台：STM32神舟开发板
 * 标准库  ：STM32F10x_StdPeriph_Driver V3.5.0
 * 作者    ：www.armjishu.com 
*******************************************************************************/
#include "lcd.h"
#include "systemUI.h"
#include "user_config.h"
#include "hw_config.h"
#include "touch.h"
#include "usart.h"
#include "dht11.h"
#include "Systick.h"
#include "bee.h"
#include <stdio.h>
#include <string.h>
#include "stm32f10x_it.h"
/**-------------------------------------------------------
  * @函数名 main
  * @功能   主函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
int i = 15;
int last_i;
int POR = 3;
u8 USART2_RX_BUF[250]; 
u8 USART2_RX_CNT=0;
u16 USART2_RX_STA=0;
void USART2_Receive_Data(u8 *buf)
{
    u8 rxlen=USART2_RX_CNT;
    u8 i=0;
    Delay_mss(10);        //等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束
    while(rxlen!=USART2_RX_CNT)
    {
        rxlen=USART2_RX_CNT;
        Delay_mss(10);
    }
        for(i=0;i<(USART2_RX_CNT);i++)
        {
            buf[i] = USART2_RX_BUF[i];    
            USART2_RX_BUF[i] = 0;
        }        
        USART2_RX_CNT=0;        //清零
    
}

int main(void)
{
		int NowTemp = 0;
		int NowHum = 0;
		u16 x,y,x1,y1;
		int count = 0;
    /* TFT-LCD初始化 */	
		SZ_STM32_COMInit();
		SZ_STM32_SysTickInit(1000000);
    SZ_STM32_LCDInit();
		Touch_Configuration();
		BEE_init();
	  LCD_Clear(LCD_COLOR_WHITE);
		UI_Refresh(i,i,i);
		last_i = i;
		while(1)	   
		{
			count++;
			switch(POR)
			{
				case 1:
					if(i<30)
					{
						i++;
						POR = 3;
					}
				break;
				case 0:
					if(i>15)
					{
						i--;
						POR = 3;
					}
				break;
				case 3:
				break;
			}
			
			if(count==1000)
			{
				NowHum =dht11_hum_value(NowHum);
				NowTemp =dht11_temp_value(NowTemp);
				JSON_Sender(NowHum,NowTemp,i);
				count=0;
			}
			if(NowTemp>35)
			{
					BEE_start();
			}
			else
			{
				 BEE_stop();
			}
			
			if(!(GPIOC->IDR&0x20))	  /* PC5 判断触摸是否发生 */
			{
				Touch_GetXY(&x,&y,0);
				delay(TOUCH_DELAY);
				Convert_Pos(x,y,&x1,&y1);
				
				if(y<TOUCH_MINUS_BUTTON_LIFT&&y>TOUCH_MINUS_BUTTON_RIGHT&&i>15)
				{
					i--;
				}
				if(y>TOUCH_PLUS_BUTTON_RIGHT&&y<TOUCH_PLUS_BUTTON_LIFT&&i<30)
				{
					i++;
				}
				
			}
			UI_Value_Refresh(NowTemp,NowHum,i);
			
		}
		
		
    while (1); 
}
	


 void USART2_IRQHandler(void)
{
    u8 res;        
		int c;
     if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收到数据
    {          
        res =USART_ReceiveData(USART2);     //读取接收到的数据        
        if(USART2_RX_STA==0)
        {
            USART2_RX_BUF[USART2_RX_CNT] = res;        //记录接收到的值    
            //当数据结尾收到0xA0和0xA1代表数据接收完成，是一串完整的数据
            if(USART2_RX_BUF[USART2_RX_CNT-1]==0xA0&&USART2_RX_BUF[USART2_RX_CNT]==0xA1)
						{
							 USART2_RX_STA=1;//表示接收数据结束
						}  
            USART2_RX_CNT++;						//接收数据增加1 
        }
				POR = Str_to_int(USART2_RX_BUF); 
        }  
    //溢出-如果发生溢出需要先读SR,再读DR寄存器则可清除不断入中断的问题
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET)
    {
        USART_ReceiveData(USART2);
        USART_ClearFlag(USART2,USART_FLAG_ORE);
    }
     USART_ClearFlag(USART2,USART_IT_RXNE); //一定要清除接收中断  
		 USART2_RX_CNT = 0;
}




/******************* (C) COPYRIGHT 2013 www.armjishu.com *****END OF FILE****/
