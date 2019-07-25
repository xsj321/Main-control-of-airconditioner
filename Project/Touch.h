#ifndef	__Touch_h__
#define	__Touch_h__

#include "stm32f10x.h"

//变量申明
extern  float xfac;
extern  float yfac;
extern  short xoff;
extern  short yoff;

//SPI读写定义


#define	T_DIN		(1&((GPIOC->IDR)>>11))

#define	T_INT		(1&((GPIOC->IDR)>>5))


//函数申明
void ADS_Write_Byte(uint8_t num) ;	   
uint16_t ADS_Read_AD(uint8_t CMD)	;
void Touch_Configuration(void);
void Draw_Big_Point(u16 x,u16 y);
uint8_t Touch_GexX(uint16_t *y,uint8_t ext);
uint8_t Touch_GexY(uint16_t *y,uint8_t ext);
void Touch_GetXY(uint16_t *x,uint16_t* y,uint8_t ext);
void Touch_Adjust(void);
void Convert_Pos(u16 x_in,u16 y_in, u16* x_out, u16* y_out);


#endif
