#include "exti.h"
#include "lpm.h"
#include "rtc.h"

extern uint8_t data_process_flag;//数据处理标志位
//无线开关外部中断，下降沿中断，磁铁靠近时检测到，进入中断退出低功耗模式，上升沿中断，磁铁移走时检测到，进入中断进入低功耗
void Wireless_Key_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_AHBPeriphClockCmd(WIRELESS_KEY_CLK, ENABLE); 

		GPIO_InitStructure.GPIO_Pin = WIRELESS_KEY_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_Init(WIRELESS_KEY_PORT, &GPIO_InitStructure);

		/* 外部中断配置 */
		//EXTI_DeInit();//会导致RTC中断不响应
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource4);
		EXTI_InitStructure.EXTI_Line = EXTI_Line4;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//EXTI_Trigger_Falling; 
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
		
		/* 中断优先级设置 */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn; 
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
		NVIC_Init(&NVIC_InitStructure);
}

////无线开关外部中断，下降沿中断，磁铁靠近时检测到，进入中断退出低功耗模式
//void exti_down(void)
//{
//		EXTI_InitTypeDef EXTI_InitStructure;
//		NVIC_InitTypeDef NVIC_InitStructure;

//		/* 外部中断配置 */
//		//EXTI_DeInit();//会导致RTC中断不响应
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
//		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource4);
//		EXTI_InitStructure.EXTI_Line = EXTI_Line4;
//		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
//		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//		EXTI_Init(&EXTI_InitStructure);
//		
//		/* 中断优先级设置 */
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn; 
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
//		NVIC_Init(&NVIC_InitStructure);
//}

////无线开关外部中断，上升沿中断，磁铁移走时检测到，进入中断进入低功耗
//void exti_up(void)
//{
//		EXTI_InitTypeDef EXTI_InitStructure;
//		NVIC_InitTypeDef NVIC_InitStructure;

//		/* 外部中断配置 */
//		//EXTI_DeInit();//会导致RTC中断不响应
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
//		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource4);
//		EXTI_InitStructure.EXTI_Line = EXTI_Line4;
//		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 
//		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//		EXTI_Init(&EXTI_InitStructure);
//		
//		/* 中断优先级设置 */
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn; 
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
//		NVIC_Init(&NVIC_InitStructure);
//}


/**
  * @brief  This function handles External line 4 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI4_IRQHandler(void)
{
		if (EXTI_GetITStatus(WIRELESS_EXTI_LINE) != RESET)
		{
			delay_ms(500);
			
			
			if(GPIO_ReadInputDataBit(WIRELESS_KEY_PORT,WIRELESS_KEY_PIN) == 0)//下降沿中断，检测到磁铁
			{
				//Run_Mode();
				//退出低功耗模式，显示屏显示主界面，可通过显示屏操作
				data_process_flag = DATA_EXTERNAL;
			}
			else////上升沿中断，检测到磁铁移走
			{
				data_process_flag &= (~DATA_EXTERNAL);
				//Stop_Mode();进入低功耗模式
			}
			
			EXTI_ClearITPendingBit(WIRELESS_EXTI_LINE);
			
#if 0			
			if((data_process_flag & DATA_EXTERNAL) != DATA_EXTERNAL)
			{
				Run_Mode();
				//退出低功耗模式，显示屏显示主界面，可通过显示屏操作
				data_process_flag = DATA_EXTERNAL;
				exti_up();//变为上升沿中断，检测磁铁是否移走
				EXTI_ClearITPendingBit(WIRELESS_EXTI_LINE);
			}
			else
			{			
				data_process_flag &= (~DATA_EXTERNAL);
				exti_down();
				EXTI_ClearITPendingBit(WIRELESS_EXTI_LINE);
				//Stop_Mode();进入低功耗模式
			}
#endif
		}
}


