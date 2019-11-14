#include "rtc.h"
#include "lpm.h"
#include "timer.h"
//#include "led.h"

sys_time sys_time_t;

RTC_TimeTypeDef RTC_time_current;
RTC_DateTypeDef RTC_date_current;

uint16_t call_time_interval = 15;//�����ϴ�Ƶ�ʼ��
extern uint8_t data_process_flag;//���ݴ����־λ
extern uint8_t data_report_flag;//�ն��쳣�ϴ���־λ��Ϊ1��ʾ�Ѿ��ϴ�����ֹ�ظ�����

//Ӳ��RTCʱ�ӳ�ʼ��
void RTC_Configuration(void)
{
	RTC_InitTypeDef RTC_InitStructure;

	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  /* Allow access to the RTC */
  PWR_RTCAccessCmd(ENABLE);
 
  /* Reset RTC Backup Domain */
  RCC_RTCResetCmd(ENABLE);
  RCC_RTCResetCmd(DISABLE);
 
  /* LSE Enable */
  RCC_LSEConfig(RCC_LSE_ON);
 
  /* Wait until LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
 
   /* RTC Clock Source Selection */ 
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); 
 
  /* Enable the RTC */
  RCC_RTCCLKCmd(ENABLE);   
	
	RTC_InitStructure.RTC_AsynchPrediv = 0X7F;
	RTC_InitStructure.RTC_SynchPrediv = 0XFF;	
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24; 
	RTC_Init(&RTC_InitStructure);

	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);  
	
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);		
}

//����RTC��ʼʱ��
void RTC_Time_Set(void)
{
	RTC_TimeTypeDef RTC_time;
	RTC_DateTypeDef RTC_date;
	RTC_date.RTC_Year = 19;
	RTC_date.RTC_Month = 8;
	RTC_date.RTC_Date = 21;
	RTC_SetDate(RTC_Format_BIN,&RTC_date);
	
	RTC_time.RTC_Hours = 10;
	RTC_time.RTC_Minutes = 14;
	RTC_time.RTC_Seconds = 20;
	RTC_time.RTC_H12 = RTC_H12_AM;
	RTC_SetTime(RTC_Format_BIN,&RTC_time);
}
	
void RTC_Alarm_Configuration(void)
{
	RTC_AlarmTypeDef RTC_AlarmStructure;
	
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;
	
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = RTC_time_current.RTC_Hours;	
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = RTC_time_current.RTC_Minutes+1;//1min����һ���ж�

	if(RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes == 60)
	{
		RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = RTC_time_current.RTC_Hours+1;	
		RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0;
	}
	
	if(RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours == 24)
	{
		RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = 0;
	}
#if 0		
	if(call_time_interval == 15)		
	{
		RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = RTC_time_current.RTC_Hours;	
		RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = (RTC_time_current.RTC_Minutes/15+1)*15;
		if(RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes == 60)
		{
			RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = RTC_time_current.RTC_Hours+1;	
			RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0;
		}
	}
	else if((call_time_interval/60 != 0) && (call_time_interval%60 == 0))//60min��������
	{
		RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = RTC_time_current.RTC_Hours+1;
		RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0;
	}
#endif

	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0;
  RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay;
  RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay|RTC_AlarmMask_Minutes;
	RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmStructure);
	
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);
	
	EXTI_ClearITPendingBit(EXTI_Line17);
	RTC_ClearFlag(RTC_FLAG_ALRAF);
}


	
void RTC_Alarm_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_ALRA)!=RESET)
	{		
		EXTI_ClearITPendingBit(EXTI_Line17);
		RTC_ClearFlag(RTC_FLAG_ALRAF);
		
		RTC_GetTime(RTC_Format_BIN,&RTC_time_current);//��ȡ��ǰʱ��
		RTC_Alarm_Configuration();//������һ������ʱ��
		
		Run_Mode();
		
		data_report_flag = 0;//��������ϱ���־
		data_process_flag |= DATA_REPORT_START;//1min���һ�δ������쳣
		TIM10_Init(50000-1,12800-1);//20��ʱ�������������ϱ�����
		GPIO_ToggleBits(LED1_PORT, LED1_PIN);//�ı�LED״̬		

		if(((15 == call_time_interval)&&(RTC_time_current.RTC_Minutes%3 == 0)) 
			||((60 == call_time_interval)&&(RTC_time_current.RTC_Minutes == 0))
			||((1440 == call_time_interval)&&(RTC_time_current.RTC_Hours == 0)))
		{
			data_process_flag |= DATA_CALL_START;//�ٲ�
			data_process_flag &= (~DATA_REPORT_START);//�����ٲ�ʱ�����������ϱ�
			TIM2_Init(50000-1,38400-1); // ��ʱ����Ϊ60s,�����ٲ�����
		}
	}
}



uint32_t iso_date(uint8_t *p)
{
	uint8_t year, month, day, hour, minute, second;
	uint32_t crtime, days, i;
	uint32_t monlen[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	year = 100 + p[0] - 70;
	month = p[1];
	day = p[2];
	hour = p[3];
	minute = p[4];
	second = p[5];
	
//	if (year < 0) 
//	{
//		crtime = 0;
//	} 
//	else 
	{
		days = year * 365;
		if (year > 2)
			days += (year+1) / 4;
		for (i = 1; i < month; i++)
			days += monlen[i-1];
		if (((year+2) % 4) == 0 && month > 2)
			days++;
		days += day - 1;
		crtime = ((((days * 24) + hour) * 60 + minute) * 60) + second;
		if(crtime>=28800)//����ʱ������
			crtime = crtime - 28800;
	}
	return crtime;
}
