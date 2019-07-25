/* Includes ------------------------------------------------------------------*/

//#include "usart.h"
#include "lcd.h"
#include "touch.h"
#include "stm32f10x.h"

#include <math.h>
#include <stdlib.h>


/*********************************************************************************************************
** Functoin name:       Touch_Configuration
** Descriptions: 		触屏用到的IO和SPI配置
** input paraments:     无
** output paraments:    无 
** Returned values:    	无
*********************************************************************************************************/
void Touch_Configuration()
{

	SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
	//启动SPI3时钟
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI3, ENABLE ) ;

	//重映射
	GPIO_PinRemapConfig(GPIO_Remap_SPI3,ENABLE);   
    //////下面是SPI相关GPIO初始化//////
    //Configure SPI3 pins: SCK,  MOSI 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_12; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //复用推挽输出
	GPIO_Init(GPIOC,&GPIO_InitStructure);  

	//MISO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//Configure PC9 pin: CS pin 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_SetBits(GPIOC,GPIO_Pin_9);// 使能触摸芯片


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure); 

    //////SPI模块配置//////
	/* SPI3 configuration */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI3, &SPI_InitStructure);

	/* Enable SPI3  */
	SPI_Cmd(SPI3, ENABLE);
	return;

}
/*SPI读写一字节*/
unsigned char SPI_SendByte(unsigned char byte)
{
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
	
	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI3, byte);
	
	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);
	
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI3);
}
 		 
/*********************************************************************************************************
** Functoin name:       ADS_Read_AD
** Descriptions:        向触摸芯片读一字节
** input paraments:     CMD：命令
** output paraments:    无    
** Returned values:    	读到的AD
*********************************************************************************************************/	   
uint16_t ADS_Read_AD(uint8_t CMD)	  
{ 	  	  
	uint16_t Num=0;
	  	 
	GPIO_ResetBits(GPIOC,GPIO_Pin_9); 					//选中ADS7843	 
	SPI_SendByte(CMD);		//发送命令字 
	
	Num = SPI_SendByte(0x00);
	Num = (Num << 8) | SPI_SendByte(0x00); 
	 	     	   	
	Num>>=4;   					//只有高12位有效.
	GPIO_SetBits(GPIOC,GPIO_Pin_9);						//释放ADS7843	 
	return(Num);   
}
/*********************************************************************************************************
** Functoin name:       ADS_Read_XY
** Descriptions:        读X或Y的值，并滤波
** input paraments:     xy:X或Y的读取命令
** output paraments:    无    
** Returned values:    	滤波后的值
*********************************************************************************************************/
#define READ_TIMES 15 						//读取次数
#define LOST_VAL 5	 						//丢弃值
uint16_t ADS_Read_XY(uint8_t xy)
{
	uint16_t i, j;
	uint16_t buf[READ_TIMES];
	uint16_t sum=0;
	uint16_t temp;
	for(i=0;i<READ_TIMES;i++)
	{				 
		buf[i]=ADS_Read_AD(xy);	    
	}				    
	for(i=0;i<READ_TIMES-1; i++)			//排序
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])				//升序排列
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)
		sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 
/*********************************************************************************************************
** Functoin name:       Read_ADSX
** Descriptions:        读X的值
** input paraments:     无
** output paraments:    x:读取后的值保存的地址    
** Returned values:    	1
*********************************************************************************************************/
uint8_t Read_ADSX(uint16_t *x)
{
	uint16_t xtemp;			 	 		  
	xtemp=ADS_Read_XY(0xd0);	  												   
	if(xtemp<100)return 0;					//读数失败
	*x=xtemp;

	return 1;								//读数成功
}
/*********************************************************************************************************
** Functoin name:       Read_ADSY
** Descriptions:        读Y的值
** input paraments:     无
** output paraments:    y:读取后的值保存的地址    
** Returned values:    	1
*********************************************************************************************************/
uint8_t Read_ADSY(uint16_t *y)
{
	uint16_t ytemp;			 	 		  
	ytemp=ADS_Read_XY(0x90);	  												   
	if(ytemp<100)return 0;					//读数失败
	*y=ytemp;

	return 1;								//读数成功
}

/*********************************************************************************************************
** Functoin name:       Read_ADSX
** Descriptions:        
				2次读取ADS7846,连续读取2次有效的AD值,且这两次的偏
				差不能超过50,满足条件,则认为读数正确,否则读数错误.
				该函数能大大提高准确度
** input paraments:     ext:是否阻塞式操作（阻塞：等待直到按下触屏，非阻塞：返回固定值）
** output paraments:    x:读取后的值保存的地址    
** Returned values:    	1
*********************************************************************************************************/

#define ERR_RANGE 50 						//误差范围 
uint8_t Touch_GexX(uint16_t *x,uint8_t ext) 
{
	uint16_t x1;
 	uint16_t x2;
 	uint8_t flag; 	
	if(ext){
		/*等待触屏按下*/
		while(T_INT);
	}    
    flag=Read_ADSX(&x1);   
    if(flag==0)return(0);
    flag=Read_ADSX(&x2);	   
    if(flag==0)return(0);  
	if(ext){
		/*等待触屏松开*/
		while(!(T_INT));
	} 
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE)))//前后两次采样在+-50内
    {
        *x=(x1+x2)/2;
//		Printf("x=0x%x\r\n",*x);
        return 1;
    }else return 0;	  
} 
/*********************************************************************************************************
** Functoin name:       Read_ADSY
** Descriptions:        
				2次读取ADS7846,连续读取2次有效的AD值,且这两次的偏
				差不能超过50,满足条件,则认为读数正确,否则读数错误.
				该函数能大大提高准确度
** input paraments:     ext:是否阻塞式操作（阻塞：等待直到按下触屏，非阻塞：返回固定值）
** output paraments:    y:读取后的值保存的地址    
** Returned values:    	1
*********************************************************************************************************/
uint8_t Touch_GexY(uint16_t *y,uint8_t ext) 
{
	uint16_t y1;
 	uint16_t y2;
 	uint8_t flag; 
	if(ext){
		/*等待触屏按下*/
		while(T_INT);
	}   
    flag=Read_ADSY(&y1);   
    if(flag==0)return(0);
    flag=Read_ADSY(&y2);	   
    if(flag==0)return(0);  
	if(ext){
		/*等待触屏松开*/
		while(!(T_INT));
	} 
    if(((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))//前后两次采样在+-50内
    {
        *y=(y1+y2)/2;
//		Printf("x=0x%x\r\n",*x);
        return 1;
    }else return 0;	  
}
/*********************************************************************************************************
** Functoin name:       Touch_GextXY
** Descriptions:        
						得到XY的AD值
** input paraments:     ext:是否阻塞式操作（阻塞：等待直到按下触屏，非阻塞：返回固定值）
** output paraments:    xy:读取后的值保存的地址    
** Returned values:    	无
*********************************************************************************************************/
void Touch_GetXY(uint16_t *x,uint16_t *y,uint8_t ext)
{
	if(ext){
		/*等待触屏按下*/
		while(T_INT);
	}
	Touch_GexX(x,0);
	Touch_GexY(y,0);
	if(ext){
		/*等待触屏松开*/
		while(!(T_INT));
	}	
}


/*********************************************************************************************************
** Functoin name:       Drow_Touch_Point
** Descriptions: 		画一个触摸校准点
** input paraments:     xy:画点的坐标
** output paraments:    无 
** Returned values:    	无
*********************************************************************************************************/
void Drow_Touch_Point(u16 x,u16 y)
{
	LCD_DrawLine(x-12,y,x+13,y);		//横线
	LCD_DrawLine(x,y-12,x,y+13);		//竖线
	LCD_DrawPoint(x+1,y+1);
	LCD_DrawPoint(x-1,y+1);
	LCD_DrawPoint(x+1,y-1);
	LCD_DrawPoint(x-1,y-1);
	Draw_Circle(x,y,6);					//画中心圈
}	  
/*********************************************************************************************************
** Functoin name:       Draw_Big_Point
** Descriptions: 		画一个大点 占4个位置
** input paraments:     xy:画点的坐标
** output paraments:    无 
** Returned values:    	无
*********************************************************************************************************/			   
void Draw_Big_Point(u16 x,u16 y)
{	    
	LCD_DrawPoint(x,y);//中心点 
	LCD_DrawPoint(x+1,y);
	LCD_DrawPoint(x,y+1);
	LCD_DrawPoint(x+1,y+1);	 	  	
}
/*********************************************************************************************************
** Functoin name:       Refreshes_Screen
** Descriptions: 		清除触摸屏
** input paraments:     xy:画点的坐标
** output paraments:    无 
** Returned values:    	无
*********************************************************************************************************/
void Refreshes_Screen(void)
{
	LCD_Clear(LCD_COLOR_WHITE);//清屏  

}
/*********************************************************************************************************
** Functoin name:       Convert_Pos
** Descriptions: 		将AD值转换为屏幕的坐标值
** input paraments:     x_in/y_in:需要转换的AD值
** output paraments:    x_out,y_out:转换后的AD值 
** Returned values:    	无
*********************************************************************************************************/
void Convert_Pos(u16 x_in,u16 y_in, u16* x_out, u16* y_out)
{		 
		*x_out=xfac*x_in+xoff;
		*y_out=yfac*y_in+yoff;  
}
/*********************************************************************************************************
** Functoin name:       Touch_Adjust
** Descriptions: 		校准触屏
** input paraments:     无
** output paraments:    无 
** Returned values:    	无
*********************************************************************************************************/
 float xfac;
 float yfac;
 short xoff;
 short yoff;
void Touch_Adjust(void)
{	
	u8 i;
						 
	u16 pos_temp[4][2];											//坐标缓存值
	u16 d1,d2;
	u32 tem1,tem2;
	float fac; 

	while(1){
								 
		for(i=0;i<4;i++){
		
			Refreshes_Screen();
			if(i==0){
				Drow_Touch_Point(20,20);
				Touch_GetXY(&pos_temp[0][0],&pos_temp[0][1],1);
			}
			else if(i == 1){
				Drow_Touch_Point(220,20);	
				Touch_GetXY(&pos_temp[1][0],&pos_temp[1][1],1);	
			}
			else if(i == 2){
				Drow_Touch_Point(20,300);
				Touch_GetXY(&pos_temp[2][0],&pos_temp[2][1],1);	
			}
			else {
			
				Drow_Touch_Point(220,300);
				Touch_GetXY(&pos_temp[3][0],&pos_temp[3][1],1);	
			}
		}

			tem1=abs(pos_temp[0][0]-pos_temp[1][0]);					//x1-x2
			tem2=abs(pos_temp[0][1]-pos_temp[1][1]);					//y1-y2
			tem1*=tem1;
			tem2*=tem2;
			d1=sqrt(tem1+tem2);											//得到1,2的距离
			
			tem1=abs(pos_temp[2][0]-pos_temp[3][0]);					//x3-x4
			tem2=abs(pos_temp[2][1]-pos_temp[3][1]);					//y3-y4
			tem1*=tem1;
			tem2*=tem2;
			d2=sqrt(tem1+tem2);											//得到3,4的距离
			fac=(float)d1/d2;
			if(fac<0.95||fac>1.05||d1==0||d2==0)						//不合格
			{
				continue;
			}
			tem1=abs(pos_temp[0][0]-pos_temp[2][0]);					//x1-x3
			tem2=abs(pos_temp[0][1]-pos_temp[2][1]);					//y1-y3
			tem1*=tem1;
			tem2*=tem2;
			d1=sqrt(tem1+tem2);											//得到1,3的距离
			
			tem1=abs(pos_temp[1][0]-pos_temp[3][0]);					//x2-x4
			tem2=abs(pos_temp[1][1]-pos_temp[3][1]);					//y2-y4
			tem1*=tem1;
			tem2*=tem2;
			d2=sqrt(tem1+tem2);											//得到2,4的距离
			fac=(float)d1/d2;
			if(fac<0.95||fac>1.05)										//不合格
			{
				continue;
			}
			//正确了
						   
			//对角线相等
			tem1=abs(pos_temp[1][0]-pos_temp[2][0]);				//x1-x3
			tem2=abs(pos_temp[1][1]-pos_temp[2][1]);				//y1-y3
			tem1*=tem1;
			tem2*=tem2;
			d1=sqrt(tem1+tem2);										//得到1,4的距离

			tem1=abs(pos_temp[0][0]-pos_temp[3][0]);				//x2-x4
			tem2=abs(pos_temp[0][1]-pos_temp[3][1]);				//y2-y4
			tem1*=tem1;
			tem2*=tem2;
			d2=sqrt(tem1+tem2);										//得到2,3的距离
			fac=(float)d1/d2;
			if(fac<0.95||fac>1.05)//不合格
			{
				continue;
			}
			//正确了

			//计算结果
			xfac=(float)220/(pos_temp[1][0]-pos_temp[0][0]);			//得到xfac		 
			xoff=(240-xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;			//得到xoff
				  
			yfac=(float)300/(pos_temp[2][1]-pos_temp[1][1]);			//得到yfac
			yoff=(320-yfac*(pos_temp[2][1]+pos_temp[1][1]))/2;			//得到yoff  

			LCD_Clear(LCD_COLOR_BLACK);
			break;	
	}
}
/*********************************************************************************************************
**                                        End Of File
*********************************************************************************************************/
