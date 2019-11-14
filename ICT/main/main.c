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


extern uint8_t data_process_flag;//数据处理标志位
extern uint8_t data_report_flag;//终端异常上传标志位，为1表示已经上传，防止重复发送

u8 key_num_t=0;

extern uint8_t timer2_flag;//定时器2标志位，为1表示到时间，60S延时，用于召测，15min召测一次
extern uint8_t timer10_flag;//定时器10标志位，为1表示到时间，20s延时，用于主动上报，1min检测一次主动上报


int main(void)
{ 
	delay_init(32);//时钟初始化
	RTC_Configuration();//RTC时钟初始化
	RTC_Time_Set();//RTC初始时间设置
//	RTC_GetTime(RTC_Format_BIN,&RTC_time_current);//获取RTC时间
//	RTC_Alarm_Configuration();//RTC闹钟初始化
	
	//LED初始化
	LED_Init();    
	Run_Led(1);//点灯
	
	
	USART1_Configuration(9600);//串口初始化  
  NBBCxx_init();//nbiot初始化
  SPI_NRF_Init(); //SPI_NRF接口初始化 
	DS18B20_Init();//温度传感器初始化
	sht10_temp_init();//sht10温湿度传感器初始化
  if(NRF_Check() != SUCCESS) //检测NRF模块与MCU的连接
	{
		while(1);
	} 	
	Adc_Init();//ADC初始化
	
	//Run_Led(1);//点灯
	
	//王鸿雁的程序
	//oled和key电源初始化
	POWER_Init();
	//oled和key打开电源
	POWER_ON(1);
	
	//OLED初始化	
	OLED_Init();	
	OLED_Fill(0x00);	//全屏熄灭
	delay_ms(100);
	OLED_Fill(0xFF);	//全屏点亮
	
	KEY_Init();		//按键初始化
	
	BEEP_Init();	//蜂鸣器初始化	
	
	

	
	Wireless_Key_Init();//无线开关初始化

	Config_Init();//配置信息初始化
	Config_Info_Read();//配置信息读取
	

	
	//关闭电源暂时不能完全关闭 只能用OX00来黑屏
	POWER_ON(1);
	OLED_Fill(0x00);

	
	Run_Led(1);//开灯
  
	//NB_Connect();//登录NB平台	     
	//Stop_Mode();进入低功耗模式

	while(1)
	{
		//Data_Process();//协议数据处理
		//MenuReflash();
		
		static u8 flag = 0;	
	
		if((data_process_flag & DATA_EXTERNAL) == DATA_EXTERNAL)//处理显示按键时不管召测、上报、设置查询
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
		
		
//		if(first_flag == 0)//第一次先上报一条召测数据
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
		
		


		
//		//对时后，周期进入低功耗
//		//Data_Set_Query();		
//		
//		if(1)//当按键进行设置时
//		{
//			Sensor_ID_Set(1,1);//传感器ID设置
//		}
////		else if(0)//召测数据时
////		{
////			//设置上传，召测一次数据
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
			Data_Call();//数据召测
			data_process_flag |= DATA_CALL_OVER;//召测完成
			data_process_flag &= (~DATA_CALL_START);//清除召测开始标志位
		}
		else if((data_process_flag & DATA_REPORT_START) == DATA_REPORT_START)
		{
			Data_Report();//数据主动上报
		}
		else if((data_process_flag & DATA_RECEIVE_UP) == DATA_RECEIVE_UP)//收到设置查询指令
		{
			data_process_flag &= (~DATA_RECEIVE_UP);//清除标志位
			//先回复当前数据，等召测完毕再处理
			if((data_process_flag & DATA_CALL_OVER) == DATA_CALL_OVER)
			{
				Data_Set_Query();//数据设置查询
			}
		}
	}
	
	if(timer2_flag == 1)//定时时间到，进入低功耗
	{
		timer2_flag = 0;	
		data_process_flag &= (~DATA_CALL_START);
		data_process_flag &= (~DATA_CALL_OVER);
		//Stop_Mode();进入低功耗模式
	}

	if(timer10_flag == 1)//定时时间到，进入低功耗
	{
		timer10_flag = 0;	
		data_process_flag &= (~DATA_REPORT_START);
		//Stop_Mode();进入低功耗模式
	}
	}    
}



