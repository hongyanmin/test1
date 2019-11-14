#ifndef __EXTI_H__
#define __EXTI_H__

#include "stm32l1xx.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_exti.h"
#include "stm32l1xx_syscfg.h"
#include "misc.h"

#include "delay.h"


#define WIRELESS_KEY_PORT 	GPIOB
#define WIRELESS_KEY_PIN 		GPIO_Pin_4
#define WIRELESS_KEY_CLK		RCC_AHBPeriph_GPIOB
#define WIRELESS_KEY_SOURCE	GPIO_PinSource4
#define WIRELESS_EXTI_LINE 	EXTI_Line4

#define WIRELESS_KEY_READ()	GPIO_ReadInputDataBit(WIRELESS_KEY_PORT,WIRELESS_KEY_PIN)

void Wireless_Key_Init(void);
void exti_down(void);
void exti_up(void);

#endif
