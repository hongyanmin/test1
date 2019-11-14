#include "stdio.h"
#include "string.h"
#include "delay.h"
#include "usart.h"
//#include "led.h"
#include "24l01.h"
#include "rtc.h"
#include "stmflash.h"
#include "adc.h"
#include "lpm.h"
#include "ds18b20.h"
#include "usart.h"
#include "stm32l1xx_rcc.h"
#include "timer.h"
#include "oled.h"
#include "temp.h"
#include "nb_process.h"
#include "exti.h"
#include "protocol.h"
#include "menu.h"
#include "switch.h"
#include "key.h"
#include "show.h"
#include "stm32l1xx_tim.h"

//extern RTC_TimeTypeDef RTC_time_current;

extern int8_t key_num;


extern uint8_t data_process_flag;//���ݴ����־λ
extern uint8_t data_report_flag;//�ն��쳣�ϴ���־λ��Ϊ1��ʾ�Ѿ��ϴ�����ֹ�ظ�����

u8 key_num_t=0;

extern uint8_t timer2_flag;//��ʱ��2��־λ��Ϊ1��ʾ��ʱ�䣬60S��ʱ�������ٲ⣬15min�ٲ�һ��
extern uint8_t timer10_flag;//��ʱ��10��־λ��Ϊ1��ʾ��ʱ�䣬20s��ʱ�����������ϱ���1min���һ�������ϱ�


int main(void)
{ 
	delay_init(32);//ʱ�ӳ�ʼ��
	RTC_Configuration();//RTCʱ�ӳ�ʼ��
	RTC_Time_Set();//RTC��ʼʱ������
//	RTC_GetTime(RTC_Format_BIN,&RTC_time_current);//��ȡRTCʱ��
//	RTC_Alarm_Configuration();//RTC���ӳ�ʼ��
	
	//LED��ʼ��
	LED_Init();    
	Run_Led(1);//���
	
	
	USART1_Configuration(9600);//���ڳ�ʼ��  
  NBBCxx_init();//nbiot��ʼ��
  SPI_NRF_Init(); //SPI_NRF�ӿڳ�ʼ�� 
	DS18B20_Init();//�¶ȴ�������ʼ��
	sht10_temp_init();//sht10��ʪ�ȴ�������ʼ��
  if(NRF_Check() != SUCCESS) //���NRFģ����MCU������
	{
		while(1);
	} 	
	Adc_Init();//ADC��ʼ��
	
	//Run_Led(1);//���
	
	//������ĳ���
	//oled��key��Դ��ʼ��
	POWER_Init();
	//oled��key�򿪵�Դ
	POWER_ON(1);
	
	//OLED��ʼ��	
	OLED_Init();	
	OLED_Fill(0x00);	//ȫ��Ϩ��
	delay_ms(100);
	OLED_Fill(0xFF);	//ȫ������
	
	KEY_Init();		//������ʼ��
	
	BEEP_Init();	//��������ʼ��	
	
	

	
	Wireless_Key_Init();//���߿��س�ʼ��

	Config_Init();//������Ϣ��ʼ��
	Config_Info_Read();//������Ϣ��ȡ
	

	
	//�رյ�Դ��ʱ������ȫ�ر� ֻ����OX00������
	POWER_ON(1);
	OLED_Fill(0x00);

	
	Run_Led(1);//����
  
	//NB_Connect();//��¼NBƽ̨	     
	//Stop_Mode();����͹���ģʽ

	while(1)
	{
		//Data_Process();//Э�����ݴ���
		//MenuReflash();
		
		static u8 flag = 0;	
	
		if((data_process_flag & DATA_EXTERNAL) == DATA_EXTERNAL)//������ʾ����ʱ�����ٲ⡢�ϱ������ò�ѯ
		{
		
			//POWER_ON(1);
			//delay_ms(1000);
		
			//Run_BEEP();
		
			key_num_t = 0;
		
			if(flag == 0)
			{
				
				Show_HomePage();
				key_num_t=Key_Read();
				if(key_num_t!=0)
				{
					flag = 1;
					Run_BEEP();
				}
			}
			else
			{
				MenuReflash();
			}
		
		
		//data_process_flag = DATA_EXTERNAL;
		
		
//		if(first_flag == 0)//��һ�����ϱ�һ���ٲ�����
//		{
//			
//			delay_ms(100);
//			
//			first_flag = 1;
//			
//			
//			//ICT_Data_Call();
//		}
//		else if(first_flag == 1)
//		{
//			
//			
//			delay_ms(500);
//			if(key_num!=0x00)
//			{
//				first_flag = 0;
//				MenuReflash();
//			}
//		}
		
		


		
//		//��ʱ�����ڽ���͹���
//		//Data_Set_Query();		
//		
//		if(1)//��������������ʱ
//		{
//			Sensor_ID_Set(1,1);//������ID����
//		}
////		else if(0)//�ٲ�����ʱ
////		{
////			//�����ϴ����ٲ�һ������
////			Data_Call();
////		}
	}
	else 
	{
		//POWER_ON(0);
		OLED_Fill(0x00);
		flag = 0;

		
		if((data_process_flag & DATA_CALL_START) == DATA_CALL_START) 
		{
			Data_Call();//�����ٲ�
			data_process_flag |= DATA_CALL_OVER;//�ٲ����
			data_process_flag &= (~DATA_CALL_START);//����ٲ⿪ʼ��־λ
		}
		else if((data_process_flag & DATA_REPORT_START) == DATA_REPORT_START)
		{
			Data_Report();//���������ϱ�
		}
		else if((data_process_flag & DATA_RECEIVE_UP) == DATA_RECEIVE_UP)//�յ����ò�ѯָ��
		{
			data_process_flag &= (~DATA_RECEIVE_UP);//�����־λ
			//�Ȼظ���ǰ���ݣ����ٲ�����ٴ���
			if((data_process_flag & DATA_CALL_OVER) == DATA_CALL_OVER)
			{
				Data_Set_Query();//�������ò�ѯ
			}
		}
	}
	
	if(timer2_flag == 1)//��ʱʱ�䵽������͹���
	{
		timer2_flag = 0;	
		data_process_flag &= (~DATA_CALL_START);
		data_process_flag &= (~DATA_CALL_OVER);
		//Stop_Mode();����͹���ģʽ
	}

	if(timer10_flag == 1)//��ʱʱ�䵽������͹���
	{
		timer10_flag = 0;	
		data_process_flag &= (~DATA_REPORT_START);
		//Stop_Mode();����͹���ģʽ
	}
	}    
}



