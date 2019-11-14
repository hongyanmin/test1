#ifndef __ADC_H
#define __ADC_H	

#include "pbdata.h"
#include "general_type.h"
#include "stm32l1xx_adc.h"
#include "stm32l1xx_dma.h"
#define ADC_CHANNEL_NUM				3


#define ADC_VOL_CH						ADC_Channel_13//电压采集通道
#define ADC_VOL_GPIO_PORT    	GPIOC
#define ADC_VOL_PIN          	GPIO_Pin_3
#define ADC_VOL_GPIO_CLK     	RCC_AHBPeriph_GPIOC

#define KEY_VOL_GPIO_PORT 		GPIOC//软件开关，为0可以采集
#define KEY_VOL_PIN          	GPIO_Pin_2
#define KEY_VOL_GPIO_CLK     	RCC_AHBPeriph_GPIOC

#define ADC_LIGHT1_CH					ADC_Channel_10//光照采集通道1
#define ADC_LIGHT1_GPIO_PORT  GPIOC
#define ADC_LIGHT1_PIN        GPIO_Pin_0
#define ADC_LIGHT1_GPIO_CLK   RCC_AHBPeriph_GPIOC

#define KEY_LIGHT1_GPIO_PORT  GPIOC//软件开关，为0可以采集
#define KEY_LIGHT1_PIN        GPIO_Pin_1
#define KEY_LIGHT1_GPIO_CLK   RCC_AHBPeriph_GPIOC

#define ADC_LIGHT2_CH					ADC_Channel_9//光照采集通道2
#define ADC_LIGHT2_GPIO_PORT  GPIOB
#define ADC_LIGHT2_PIN        GPIO_Pin_1
#define ADC_LIGHT2_GPIO_CLK   RCC_AHBPeriph_GPIOB

#define KEY_LIGHT2_GPIO_PORT  GPIOB//软件开关，为0可以采集
#define KEY_LIGHT2_PIN        GPIO_Pin_0
#define KEY_LIGHT2_GPIO_CLK   RCC_AHBPeriph_GPIOB

void Adc_Init(void); 				//ADC通道初始化
void Adc_Key_Init(void);		//软件开关初始化
void DMA_init(void);				//DMA初始化
u16  Get_Adc(u8 ch); 				//获得某个通道值 
u16 Get_Adc_Average(u8 ch,u8 times);//得到某个通道10次采样的平均值 	  
void Adc_Key_On(void);
void Adc_Key_Off(void);
#endif 















