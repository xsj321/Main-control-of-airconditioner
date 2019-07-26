/*********************************************************************************
 * 文件名  ：systemUI.c
 * 描述    ：实现对LCD屏幕上的界面的绘制和对数值的更新
 * 作者    ：xsj321 
**********************************************************************************/
#include "systemUI.h"
#include "lcd.h"
#include "string.h"
#include "math.h"

/**

登记需要用到的文字包括标题

**/
const uint8_t Header_1[]= "Temperature  control";
const uint8_t BlankLine[]= "                    ";
const uint8_t Him[] = "Humidity:";
const uint8_t Tem[] = "Temperature:";
const uint8_t NowTem[] = "Set Aim ";
const uint8_t ButtonPlus[] = "+";
const uint8_t ButtonMinus[] = "-";
const uint8_t Celsius[] = "C";
const uint8_t percent[] = "%";





int Str_to_int(unsigned char *source)
{
	int length = strlen(source); //计算长度
	int sum = 0;
	int i=0;
	for(i=0;i<length;i++)
    {
        sum += (source[i]-'0') * pow(10,length-1-i); //字符转数字并合并
    }
	return sum;
}

void Int_To_Str(int x,char *Str)
{
	int t;
	char *Ptr,Buf[5];
	int i = 0;
	Ptr = Str;
	if(x < 10)
	{
		*Ptr ++ = '0'; 
		*Ptr ++ = x+0x30;
	}
	else
	{
		while(x > 0)
		{
			t = x % 10;
			x = x / 10;
			Buf[i++] = t+0x30;
		}
		i -- ;
		for(;i >= 0;i --) 	
		{
			*(Ptr++) = Buf[i];
		}
	}
	*Ptr = '\0';
}

void UI_Refresh(int INT_TEM,int INT_HIM,int INT_NOWTEM)
{
		uint8_t Tem_num[] ="";
		uint8_t Him_num[] = "";
		uint8_t NowTem_num[] = "";
		Int_To_Str(INT_TEM,(char *)Tem_num);
		Int_To_Str(INT_NOWTEM,(char *)NowTem_num);
		Int_To_Str(INT_HIM,(char *)Him_num);
    /* 设置显示的字体大小*/
    LCD_SetFont(&Font16x24);
	/*显示英文字符*/
		LCD_SetBackColor(LCD_COLOR_BLUE2);
	//绘制标题
		LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)BlankLine,0);
		LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)Header_1,0);	
		LCD_DisplayStringLine(LCD_LINE_2, (uint8_t *)BlankLine,0);
	//绘制主体结构
		LCD_SetBackColor(LCD_COLOR_BLUE2);
	  LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_DrawFullRect(75, 1, 99,164);
		LCD_DrawFullRect(75, 102, 116,164);
		LCD_SetTextColor(LCD_COLOR_BLUE2);
		LCD_DrawFullRect(75, 220, 99,164);
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_DrawLine(159,103,116,LCD_DIR_HORIZONTAL);
		LCD_DrawLine(160,103,116,LCD_DIR_HORIZONTAL);
		LCD_DrawLine(161,103,116,LCD_DIR_HORIZONTAL);
	
	//绘制环境温度选框
		LCD_SetFont(&Font8x8);
		
	//环境湿度
		LCD_SetTextColor(LCD_COLOR_BLACK);
		LCD_DisplayStringLine(LCD_LINE_10, (uint8_t *)Him,DISPLAY_INFOBOX_LIFT);
		//CD_DisplayStringLine(LCD_LINE_12, (uint8_t *)Him_num,DISPLAY_INFOBOX_LIFT);
		LCD_DisplayString(100,105,(uint8_t *)Him_num);
		
	//环境温度	
	  LCD_DisplayStringLine(LCD_LINE_15, (uint8_t *)Tem,DISPLAY_INFOBOX_LIFT);
		//LCD_DisplayStringLine(LCD_LINE_17, (uint8_t *)Tem_num,DISPLAY_INFOBOX_LIFT);
		LCD_DisplayString(140,105,(uint8_t *)Tem_num);
   		
	//绘制目标温度选框
		LCD_DisplayStringLine(LCD_LINE_21, (uint8_t *)NowTem,DISPLAY_INFOBOX_LIFT);
		LCD_DisplayStringLine(LCD_LINE_23, (uint8_t *)Tem,DISPLAY_INFOBOX_LIFT);
		LCD_SetFont(&Font12x24);
		//LCD_DisplayStringLine(LCD_LINE_8, (uint8_t *)NowTem_num,120);
		LCD_DisplayString(200,110,(uint8_t *)NowTem_num);
		//LCD_DisplayStringLine(LCD_LINE_8, (uint8_t *)Celsius,160);
		LCD_DisplayString(200,150,(uint8_t *)Celsius);
		
		LCD_SetFont(&Font24x48);
		LCD_DisplayStringLine(LCD_LINE_3, (uint8_t *)ButtonMinus,40);
		LCD_DisplayStringLine(LCD_LINE_3, (uint8_t *)ButtonPlus,260);
		
	
}

void UI_Value_Refresh(int INT_TEM,int INT_HIM,int INT_NOWTEM)
{
	uint8_t Tem_num[] ="";
	uint8_t Him_num[] = "";
	uint8_t NowTem_num[] = "";
	Int_To_Str(INT_TEM,(char *)Tem_num);
	Int_To_Str(INT_NOWTEM,(char *)NowTem_num);
	Int_To_Str(INT_HIM,(char *)Him_num);
	LCD_SetFont(&Font8x8);
	LCD_SetBackColor(LCD_COLOR_BLUE2);
	LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_DisplayString(100,105,(uint8_t *)Him_num);
	LCD_DisplayString(140,105,(uint8_t *)Tem_num);
	
	LCD_SetFont(&Font12x24);
	LCD_DisplayString(200,150,(uint8_t *)Celsius);
	LCD_DisplayString(200,110,(uint8_t *)NowTem_num);
}

