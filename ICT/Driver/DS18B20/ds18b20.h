#ifndef __DS18B20_H
#define __DS18B20_H 
#include "pbdata.h"

#define DS18B20_GPIO_PORT		GPIOB
#define DS18B20_GPIO_PIN		GPIO_Pin_3
#define DS18B20_GPIO_CLK		RCC_AHBPeriph_GPIOB

//#define	DS18B20_DQ_OUT_1()		GPIO_SetBits(GPIOB, GPIO_Pin_3)
//#define	DS18B20_DQ_OUT_0()		GPIO_ResetBits(GPIOB, GPIO_Pin_3)
										   
//#define DS18B20_DQ_IN()			GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)  

//////IO��������											   
#define	DS18B20_DQ_OUT	 PBout(3) //���ݶ˿�	PA0 
#define	DS18B20_DQ_IN()  PBin(3)  //���ݶ˿�	PA0 

u8 DS18B20_Init(void);			//��ʼ��DS18B20
short DS18B20_Get_Temp(void);	//��ȡ�¶�
void DS18B20_Start(void);		//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);		//����һ���ֽ�
u8 DS18B20_Read_Bit(void);		//����һ��λ
u8 DS18B20_Check(void);			//����Ƿ����DS18B20
void DS18B20_Rst(void);			//��λDS18B20    
void DS18B20_IO_OUT(void);	//����Ϊ���ģʽ
void DS18B20_IO_IN(void);		//����Ϊ����ģʽ
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

u8 DS18B20_Init(void);			//��ʼ��DS18B20
short DS18B20_Get_Temp(void);	//��ȡ�¶�
void DS18B20_Start(void);		//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);		//����һ���ֽ�
u8 DS18B20_Read_Bit(void);		//����һ��λ
u8 DS18B20_Check(void);			//����Ƿ����DS18B20
void DS18B20_Rst(void);			//��λDS18B20    
void DS18B20_IO_OUT(void);	//����Ϊ���ģʽ
void DS18B20_IO_IN(void);		//����Ϊ����ģʽ
#endif
#endif














