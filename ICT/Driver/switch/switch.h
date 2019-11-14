#ifndef __SWITCH_H
#define __SWITCH_H	 

#include "sys.h"



#define LED1_PORT 					GPIOC
#define LED1_PIN 						GPIO_Pin_5
#define LED1_CLK						RCC_AHBPeriph_GPIOC

#define BEEP_PORT 					GPIOC
#define BEEP_PIN 						GPIO_Pin_4
#define BEEP_CLK						RCC_AHBPeriph_GPIOC

#define WIRELESS_KEY_PORT 	GPIOB
#define WIRELESS_KEY_PIN 		GPIO_Pin_4
#define WIRELESS_KEY_CLK		RCC_AHBPeriph_GPIOB

#define WATER_LEVEL_PORT1		GPIOA
#define WATER_LEVEL_PIN1		GPIO_Pin_11
#define WATER_LEVEL_CLK1		RCC_AHBPeriph_GPIOA

#define WATER_LEVEL_PORT2		GPIOD
#define WATER_LEVEL_PIN2		GPIO_Pin_2
#define WATER_LEVEL_CLK2		RCC_AHBPeriph_GPIOD

#define BEEP_ON()						GPIO_SetBits(BEEP_PORT, BEEP_PIN)
#define BEEP_OFF()					GPIO_ResetBits(BEEP_PORT, BEEP_PIN)

#define WIRELESS_KEY_READ()	GPIO_ReadInputDataBit(WIRELESS_KEY_PORT,WIRELESS_KEY_PIN)

void LED_Init(void);//≥ı ºªØ
void Run_Led(int i);

void BEEP_Init(void);
void Run_BEEP(void);

void POWER_Init(void);
void POWER_ON(int i);

//void Wireless_Key_Init(void);





#endif

















