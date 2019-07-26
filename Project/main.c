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

#define MAX_Temp 30//设定最高温度
#define MIN_Temp 15//设定最低温度

#define MAX_EN_Temp 35//设定环境最高报警温度

int i = 15;//默认开机设定温度同时也是保存设定温度的变量
int last_i;//上一次的设定温度

/*

变量：POR
功能：标记对控制温度的加减
值	|		1		|		2		|		3		|
————|———————|———————|———————|
操作|	升温	|	降温	|	未操作|

*/
int POR = 3;

u8 USART2_RX_BUF[250]; 
u8 USART2_RX_CNT=0;
u16 USART2_RX_STA=0;

/***

	函数名：void USART2_Receive_Data(u8 *buf)
	参数：u8 *buf
	功能：从USART获取数据
	
***/
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
		int NowTemp = 0;//当前温度
		int NowHum = 0;//当前湿度
		u16 x,y,x1,y1;//触屏点击坐标
		int count = 0;
    /* TFT-LCD初始化 */	
		SZ_STM32_COMInit();//串口初始化
		SZ_STM32_SysTickInit(1000000);//SysTickInit初始化
    SZ_STM32_LCDInit();//LCD显示初始化
		Touch_Configuration();//触摸屏初始化
		BEE_init();//蜂鸣器初始化
	  LCD_Clear(LCD_COLOR_WHITE);//设定背景颜色
		UI_Refresh(i,i,i);//绘制数据界面，并数值刷新
		last_i = i;//将当前的数值保存起来
	
		//循环控制了对温度控制的操作
		while(1)	   
		{
			count++;
			switch(POR)
			{
				case 1:
					if(i<MAX_Temp)
					{
						i++;
						/*
						此处添加对升温的硬件控制操作
						*/
						POR = 3;
					}
				break;
				case 0:
					if(i>MIN_Temp)
					{
						i--;
						/*
						此处添加对降温的硬件控制操作
						*/
						POR = 3;
					}
				break;
				case 3:
				break;
			}
			
			//定时对画面的当前温度与当前湿度进行刷新
			if(count==1000)
			{
				NowHum =dht11_hum_value(NowHum);//读取当前的湿度
				NowTemp =dht11_temp_value(NowTemp);//读取当前的温度
				JSON_Sender(NowHum,NowTemp,i);//将数据格式化为JSON并从串口发送
				count=0;//清零计数器
			}
			
			//判断温度是否超过警报的触发值
			if(NowTemp>MAX_EN_Temp)
			{
					BEE_start();//开始警报
			}
			else
			{
				 BEE_stop();//停止警报
			}
			
			if(!(GPIOC->IDR&0x20))//判断触摸是否发生
			{
				Touch_GetXY(&x,&y,0);
				delay(TOUCH_DELAY);
				Convert_Pos(x,y,&x1,&y1);
				
				//判断触摸区域
				if(y<TOUCH_MINUS_BUTTON_LIFT&&y>TOUCH_MINUS_BUTTON_RIGHT&&i>15)
				{
					i--;
				}
				if(y>TOUCH_PLUS_BUTTON_RIGHT&&y<TOUCH_PLUS_BUTTON_LIFT&&i<30)
				{
					i++;
				}
				
			}
			
			UI_Value_Refresh(NowTemp,NowHum,i);//刷新数值
			
		}
		
		
    while (1); 
}
	
/***

	函数名：USART2_IRQHandler(void)
	参数：无
	功能：串口2终端服务函数，接受传输过来的数据
	
***/

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

