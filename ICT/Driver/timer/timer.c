#include "timer.h"
#include "misc.h"
#include "stm32l1xx_tim.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_rcc.h"

uint8_t timer2_flag = 0;//定时器2标志位，为1表示到时间，60S延时，用于召测，15min召测一次
uint8_t timer10_flag = 0;//定时器10标志位，为1表示到时间，20s延时，用于主动上报，1min检测一次主动上报

void TIM2_Init(u16 arr,u16 psc)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  
		TIM_TimeBaseStructure.TIM_Period = arr;//Tout= ((arr+1)*(psc+1))/Tclk；
		TIM_TimeBaseStructure.TIM_Prescaler = psc;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		/* TIM IT enable */
		TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);
	
		/* TIM2 enable counter */
		TIM_Cmd(TIM2, ENABLE);			

		/* Enable the TIM2 Interrupt */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);	
}

/**
  * @brief  This function handles TIM2 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		timer2_flag=1;
    TIM_Cmd(TIM2, DISABLE);
  }
}

void TIM10_Init(u16 arr,u16 psc)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);  
		TIM_TimeBaseStructure.TIM_Period = arr;//Tout= ((arr+1)*(psc+1))/Tclk；
		TIM_TimeBaseStructure.TIM_Prescaler = psc;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

		TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStructure);
	
		TIM_ClearITPendingBit(TIM10, TIM_IT_Update);
		/* TIM IT enable */
		TIM_ITConfig(TIM10,TIM_IT_Update, ENABLE);
	
		/* TIM10 enable counter */
		TIM_Cmd(TIM10, ENABLE);			

		/* Enable the TIM10 Interrupt */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = TIM10_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);	
}

/**
  * @brief  This function handles TIM9 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM10_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM10, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM10, TIM_IT_Update);
		timer10_flag=1;
    TIM_Cmd(TIM10, DISABLE);
  }
}
