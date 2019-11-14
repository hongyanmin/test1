#include "exti.h"
#include "lpm.h"
#include "rtc.h"

extern uint8_t data_process_flag;//���ݴ����־λ
//���߿����ⲿ�жϣ��½����жϣ���������ʱ��⵽�������ж��˳��͹���ģʽ���������жϣ���������ʱ��⵽�������жϽ���͹���
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

		/* �ⲿ�ж����� */
		//EXTI_DeInit();//�ᵼ��RTC�жϲ���Ӧ
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource4);
		EXTI_InitStructure.EXTI_Line = EXTI_Line4;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//EXTI_Trigger_Falling; 
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
		
		/* �ж����ȼ����� */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn; 
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
		NVIC_Init(&NVIC_InitStructure);
}

////���߿����ⲿ�жϣ��½����жϣ���������ʱ��⵽�������ж��˳��͹���ģʽ
//void exti_down(void)
//{
//		EXTI_InitTypeDef EXTI_InitStructure;
//		NVIC_InitTypeDef NVIC_InitStructure;

//		/* �ⲿ�ж����� */
//		//EXTI_DeInit();//�ᵼ��RTC�жϲ���Ӧ
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
//		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource4);
//		EXTI_InitStructure.EXTI_Line = EXTI_Line4;
//		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
//		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//		EXTI_Init(&EXTI_InitStructure);
//		
//		/* �ж����ȼ����� */
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn; 
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
//		NVIC_Init(&NVIC_InitStructure);
//}

////���߿����ⲿ�жϣ��������жϣ���������ʱ��⵽�������жϽ���͹���
//void exti_up(void)
//{
//		EXTI_InitTypeDef EXTI_InitStructure;
//		NVIC_InitTypeDef NVIC_InitStructure;

//		/* �ⲿ�ж����� */
//		//EXTI_DeInit();//�ᵼ��RTC�жϲ���Ӧ
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
//		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource4);
//		EXTI_InitStructure.EXTI_Line = EXTI_Line4;
//		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 
//		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//		EXTI_Init(&EXTI_InitStructure);
//		
//		/* �ж����ȼ����� */
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
			
			
			if(GPIO_ReadInputDataBit(WIRELESS_KEY_PORT,WIRELESS_KEY_PIN) == 0)//�½����жϣ���⵽����
			{
				//Run_Mode();
				//�˳��͹���ģʽ����ʾ����ʾ�����棬��ͨ����ʾ������
				data_process_flag = DATA_EXTERNAL;
			}
			else////�������жϣ���⵽��������
			{
				data_process_flag &= (~DATA_EXTERNAL);
				//Stop_Mode();����͹���ģʽ
			}
			
			EXTI_ClearITPendingBit(WIRELESS_EXTI_LINE);
			
#if 0			
			if((data_process_flag & DATA_EXTERNAL) != DATA_EXTERNAL)
			{
				Run_Mode();
				//�˳��͹���ģʽ����ʾ����ʾ�����棬��ͨ����ʾ������
				data_process_flag = DATA_EXTERNAL;
				exti_up();//��Ϊ�������жϣ��������Ƿ�����
				EXTI_ClearITPendingBit(WIRELESS_EXTI_LINE);
			}
			else
			{			
				data_process_flag &= (~DATA_EXTERNAL);
				exti_down();
				EXTI_ClearITPendingBit(WIRELESS_EXTI_LINE);
				//Stop_Mode();����͹���ģʽ
			}
#endif
		}
}


