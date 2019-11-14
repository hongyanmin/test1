#ifndef __ADC_H
#define __ADC_H	

#include "pbdata.h"
#include "general_type.h"
#include "stm32l1xx_adc.h"
#include "stm32l1xx_dma.h"
#define ADC_CHANNEL_NUM				3


#define ADC_VOL_CH						ADC_Channel_13//��ѹ�ɼ�ͨ��
#define ADC_VOL_GPIO_PORT    	GPIOC
#define ADC_VOL_PIN          	GPIO_Pin_3
#define ADC_VOL_GPIO_CLK     	RCC_AHBPeriph_GPIOC

#define KEY_VOL_GPIO_PORT 		GPIOC//������أ�Ϊ0���Բɼ�
#define KEY_VOL_PIN          	GPIO_Pin_2
#define KEY_VOL_GPIO_CLK     	RCC_AHBPeriph_GPIOC

#define ADC_LIGHT1_CH					ADC_Channel_10//���ղɼ�ͨ��1
#define ADC_LIGHT1_GPIO_PORT  GPIOC
#define ADC_LIGHT1_PIN        GPIO_Pin_0
#define ADC_LIGHT1_GPIO_CLK   RCC_AHBPeriph_GPIOC

#define KEY_LIGHT1_GPIO_PORT  GPIOC//������أ�Ϊ0���Բɼ�
#define KEY_LIGHT1_PIN        GPIO_Pin_1
#define KEY_LIGHT1_GPIO_CLK   RCC_AHBPeriph_GPIOC

#define ADC_LIGHT2_CH					ADC_Channel_9//���ղɼ�ͨ��2
#define ADC_LIGHT2_GPIO_PORT  GPIOB
#define ADC_LIGHT2_PIN        GPIO_Pin_1
#define ADC_LIGHT2_GPIO_CLK   RCC_AHBPeriph_GPIOB

#define KEY_LIGHT2_GPIO_PORT  GPIOB//������أ�Ϊ0���Բɼ�
#define KEY_LIGHT2_PIN        GPIO_Pin_0
#define KEY_LIGHT2_GPIO_CLK   RCC_AHBPeriph_GPIOB

void Adc_Init(void); 				//ADCͨ����ʼ��
void Adc_Key_Init(void);		//������س�ʼ��
void DMA_init(void);				//DMA��ʼ��
u16  Get_Adc(u8 ch); 				//���ĳ��ͨ��ֵ 
u16 Get_Adc_Average(u8 ch,u8 times);//�õ�ĳ��ͨ��10�β�����ƽ��ֵ 	  
void Adc_Key_On(void);
void Adc_Key_Off(void);
#endif 















