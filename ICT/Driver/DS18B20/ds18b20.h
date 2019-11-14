#ifndef __DS18B20_H
#define __DS18B20_H 
#include "pbdata.h"

#define DS18B20_GPIO_PORT		GPIOB
#define DS18B20_GPIO_PIN		GPIO_Pin_3
#define DS18B20_GPIO_CLK		RCC_AHBPeriph_GPIOB

//#define	DS18B20_DQ_OUT_1()		GPIO_SetBits(GPIOB, GPIO_Pin_3)
//#define	DS18B20_DQ_OUT_0()		GPIO_ResetBits(GPIOB, GPIO_Pin_3)
										   
//#define DS18B20_DQ_IN()			GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)  

//////IO操作函数											   
#define	DS18B20_DQ_OUT	 PBout(3) //数据端口	PA0 
#define	DS18B20_DQ_IN()  PBin(3)  //数据端口	PA0 

u8 DS18B20_Init(void);			//初始化DS18B20
short DS18B20_Get_Temp(void);	//获取温度
void DS18B20_Start(void);		//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);		//读出一个字节
u8 DS18B20_Read_Bit(void);		//读出一个位
u8 DS18B20_Check(void);			//检测是否存在DS18B20
void DS18B20_Rst(void);			//复位DS18B20    
void DS18B20_IO_OUT(void);	//设置为输出模式
void DS18B20_IO_IN(void);		//设置为输入模式
#endif

#if 0
#ifndef __DS18B20_H
#define __DS18B20_H 
#include "pbdata.h"

#define DS18B20_GPIO_PORT    GPIOB
#define DS18B20_GPIO_PIN     GPIO_Pin_3
#define DS18B20_GPIO_CLK     RCC_AHBPeriph_GPIOB

#define	DS18B20_DQ_OUT_1()  GPIO_SetBits(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN)
#define	DS18B20_DQ_OUT_0()  GPIO_ResetBits(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN)
										   
#define DS18B20_DQ_IN()		  GPIO_ReadInputDataBit(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN)  

u8 DS18B20_Init(void);			//初始化DS18B20
short DS18B20_Get_Temp(void);	//获取温度
void DS18B20_Start(void);		//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);		//读出一个字节
u8 DS18B20_Read_Bit(void);		//读出一个位
u8 DS18B20_Check(void);			//检测是否存在DS18B20
void DS18B20_Rst(void);			//复位DS18B20    
void DS18B20_IO_OUT(void);	//设置为输出模式
void DS18B20_IO_IN(void);		//设置为输入模式
#endif
#endif














