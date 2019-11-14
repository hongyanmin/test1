#include "show.h"


float humidity;
u16 temperature;
u8 str_tmp[]={"Hello STM32L151!\r\n"};
float ADC_ConvertedValueLocal[3],voltage;
float light1,light2; 
u8 voltage_H,voltage_L;

//�豸��ǰʱ��
extern RTC_TimeTypeDef RTC_time_current;
extern RTC_DateTypeDef RTC_date_current;

//ICT���
extern u8 ICT_Num[9];

////Ҫд��ȥ��ʱ��
//RTC_TimeTypeDef RTC_time;
//RTC_DateTypeDef RTC_date;


void Show_HomePage(void)
{
	
		//u8 tem;
		u8 a,b,c;
		u8 i;
		u8 t,t1,t2;
	
//		delay_ms(1000);	
//	
//		OLED_CLS();								//����
//		OLED_DrawBMP(60,0,76,8,(unsigned char *)BMP1);		//BMPλͼ��ʾ

//		//��ʾsihan
//		OLED_ShowStr(0,0,"SiHan",1);				
//	
//		//OLED_CLS();			//����
//	
//		//OLED_Fill(0x00);	//ȫ����
	
		//��ȡʱ��
		RTC_GetDate(RTC_Format_BIN,&RTC_date_current);
		RTC_GetTime(RTC_Format_BIN,&RTC_time_current);
		
		//��ȡ���պ͵�ѹֵ
		ADC_ConvertedValueLocal[0] = (float)Get_Adc_Average(0,10)/4096*(float)2.5;//��׼��ѹ2.5V
		ADC_ConvertedValueLocal[1] = (float)Get_Adc_Average(1,10)/4096*(float)2.5;
		ADC_ConvertedValueLocal[2] = (float)Get_Adc_Average(2,10)/4096*(float)2.5;
		//����ֵ
		voltage = ADC_ConvertedValueLocal[0]*2.0;//������ѹ
		light1 = ADC_ConvertedValueLocal[1];//�ӽ�1��ʾû�й�
		light2 = ADC_ConvertedValueLocal[2];
	
		//��ȡ�¶�
		temperature = DS18B20_Get_Temp();	
			
			
			
		//1��ʾʱ��
		//ʱ����
		t=RTC_time_current.RTC_Hours;
		t1=t/10; OLED_ShowChar_Small(0,0,48+t1);
		t2=t%10; OLED_ShowChar_Small(6,0,48+t2);
		
		OLED_ShowChar_Small(12,0,':');				

		t=RTC_time_current.RTC_Minutes;
		t1=t/10; OLED_ShowChar_Small(16,0,48+t1);
		t2=t%10; OLED_ShowChar_Small(22,0,48+t2);
		
		OLED_ShowChar_Small(28,0,':');	
		
		t=RTC_time_current.RTC_Seconds;
		t1=t/10; OLED_ShowChar_Small(32,0,48+t1);
		t2=t%10; OLED_ShowChar_Small(38,0,48+t2);
		
		
		//2��ʾ�ź�
		OLED_ShowCN(60,0,40);	
//		delay_ms(1000);		OLED_ShowCN(80,0,39);	//��ʾ����,���ֿ���ʾ4��
//		delay_ms(1000);		OLED_ShowCN(80,0,38);	//��ʾ����,���ֿ���ʾ4��
//		delay_ms(1000);		OLED_ShowCN(80,0,37);	//��ʾ����,���ֿ���ʾ4��
//		delay_ms(1000);		OLED_ShowCN(80,0,36);	//��ʾ����,���ֿ���ʾ4��
//		delay_ms(1000);


		


		//3��ʾ����
		if(light1>1.5)				//������ղ��� ������������
		{
			OLED_ShowCN(85,0,42);	
		}
		else
		{
			OLED_ShowCN(85,0,41);	
		}	
		
//		//��ʾ��ѹֵ
//		voltage_H=((u8)(voltage*10))/10;
//		voltage_L=((u8)(voltage*10))%10;
//		USART_SendData(USART1,voltage_H);
//		OLED_ShowCN(0,4,29);	//��ʾ����,���ֿ���ʾ4��
//		OLED_ShowCN(16,4,30);	//��ʾ����,���ֿ���ʾ4��
//		OLED_ShowChar(32,4,':');				//��ʾ�ַ�
//		OLED_ShowChar(40,4,voltage_H+48);				//��ʾ�ַ�
//		OLED_ShowChar(48,4,'.');				//��ʾ�ַ�
//		OLED_ShowChar(56,4,voltage_L+48);				//��ʾ�ַ�
//		OLED_ShowChar(68,4,'V');				//��ʾ�ַ�

		
		//4��ʾ����
		if(voltage>3.3)								OLED_ShowCN(110,0,31);	
		else if((voltage>3)&&(voltage<=3.3))		OLED_ShowCN(110,0,33);	
		else if(voltage<=3)							OLED_ShowCN(110,0,35);	


		//5��ʾ�¶�
		a=temperature/100;
		b=((temperature-a*100)/10);
		c=(temperature-a*100-b*10);
		
		OLED_ShowCN(0,2,27);					//��
		OLED_ShowCN(16,2,28);					//��
		OLED_ShowChar(32,2,':');				//:

		OLED_ShowChar(40,2,a+48);				//��ʾ�ַ�
		OLED_ShowChar(48,2,b+48);				//��ʾ�ַ�
		OLED_ShowChar(56,2,'.');				//.
		OLED_ShowChar(60,2,c+48);				//��ʾ�ַ�
		OLED_ShowChar(70,1,'.');				//.
		OLED_ShowChar(74,2,'C');				//C
		
				
		//6��ʾ���
		OLED_ShowCN(0,4,2);	//��ʾ����,���ֿ���ʾ4��
		OLED_ShowCN(16,4,3);	//��ʾ����,���ֿ���ʾ4��
		OLED_ShowChar(32,4,':');				//��ʾ�ַ�
		for(i=0;i<=8;i++)
		{
			//OLED_ShowChar(40+i*8,4,49+i);				//��ʾ�ַ�
			OLED_ShowChar(40+i*8,4,48+ICT_Num[i]);				//��ʾ�ַ�
		}
		
		
		//7��ʾʱ��
		OLED_ShowCN(0,6,15);	//��ʾ����,���ֿ���ʾ4��
		OLED_ShowCN(16,6,16);	//��ʾ����,���ֿ���ʾ4��
		OLED_ShowChar(32,6,':');				//��ʾ�ַ�
		OLED_ShowChar(40,6,48+2);
		OLED_ShowChar(48,6,48+0);
		
		//������
		t=RTC_date_current.RTC_Year;
		t1=t/10; OLED_ShowChar(56,6,48+t1);
		t2=t%10; OLED_ShowChar(62,6,48+t2);
		OLED_ShowChar(70,6,'.');
		
		t=RTC_date_current.RTC_Month;
		t1=t/10; OLED_ShowChar(78,6,48+t1);
		t2=t%10; OLED_ShowChar(86,6,48+t2);
		OLED_ShowChar(94,6,'.');
		
		t=RTC_date_current.RTC_Date;
		t1=t/10; OLED_ShowChar(100,6,48+t1);
		t2=t%10; OLED_ShowChar(108,6,48+t2);
		

//		USART_SendData(USART1,RTC_date_current.RTC_Year);
//		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
//			USART_SendData(USART1,RTC_date_current.RTC_Month);
//		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
//			USART_SendData(USART1,RTC_date_current.RTC_Date);
//		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
//			USART_SendData(USART1,RTC_time_current.RTC_Hours);
//		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
//			USART_SendData(USART1,RTC_time_current.RTC_Minutes);
//		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
//			USART_SendData(USART1,RTC_time_current.RTC_Seconds);
//		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
//			USART_SendData(USART1,RTC_time_current.RTC_H12);
//		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
		
		//���ʱ�亯��
		//iso_date((uint8_t*)&sys_time_t);
	
}


void Show_OkPage(void)
{
	OLED_Fill(0x00);
	
	OLED_ShowChar(20,2,'S');
	OLED_ShowChar(28,2,'e');
	OLED_ShowChar(36,2,'e');
	OLED_ShowChar(44,2,'d');

	OLED_ShowChar(60,2,'O');
	OLED_ShowChar(68,2,'K');
	
	delay_ms(1000);
}

void Show_SendingPage(void)
{
	OLED_Fill(0x00);
	
	OLED_ShowChar(20,2,'S');
	OLED_ShowChar(28,2,'e');
	OLED_ShowChar(36,2,'e');
	OLED_ShowChar(44,2,'d');

	OLED_ShowChar(52,2,'i');
	OLED_ShowChar(60,2,'n');
	OLED_ShowChar(68,2,'g');

	
	delay_ms(1000);
}

