#ifndef __KEY_H
#define	__KEY_H

#include "sys.h"  



extern uint8_t KEY_Num;



//PB10 -- SCL
#define KEY_SCL_PORT 			GPIOB
#define KEY_SCL_PIN 			GPIO_Pin_10
#define KEY_SCL_CLK				RCC_AHBPeriph_GPIOB
#define KEY_SCL_SOURCE		GPIO_PinSource10
//PB11 -- SDA
#define KEY_SDA_PORT 			GPIOB
#define KEY_SDA_PIN 			GPIO_Pin_11
#define KEY_SDA_CLK				RCC_AHBPeriph_GPIOB
#define KEY_SDA_SOURCE		GPIO_PinSource11
//PB2 -- IRQ
#define KEY_IRQ_PORT 			GPIOB
#define KEY_IRQ_PIN 			GPIO_Pin_2
#define KEY_IRQ_CLK				RCC_AHBPeriph_GPIOB
#define KEY_IRQ_SOURCE		GPIO_PinSource2

#define KEY_ADDRESS_1			0xA0 
#define KEY_ADDRESS_2			0xA1

#define IIC_SCL    PBout(10) //SCL
#define READ_SCL   PBin(10)  // ‰»ÎSCL 
#define IIC_SDA    PBout(11) //SDA	 
#define READ_SDA   PBin(11)  // ‰»ÎSDA 

#define KEY_UP 			1 
#define KEY_DOWN		2 
#define KEY_LEFT		3 
#define KEY_RIGHT   4 


void KEY_Init(void);
u8 Key_Read(void);
void Key_Sensitivity_Configuration(void);

#endif
