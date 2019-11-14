#include "switch.h"


void LED_Init(void)
{    
  GPIO_InitTypeDef GPIO_InitStructure; 
  RCC_AHBPeriphClockCmd(LED1_CLK, ENABLE);
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Pin = LED1_PIN;
  GPIO_Init(LED1_PORT, &GPIO_InitStructure);
 // GPIO_ResetBits(LED1_PORT, LED1_PIN);   

}


void Run_Led(int i)
{
    if(i)
    {
		GPIO_ResetBits(LED1_PORT, LED1_PIN);  
    }
    else
    {
       GPIO_SetBits(LED1_PORT, LED1_PIN);  
    } 
}

//蜂鸣器
void BEEP_Init(void)
{    
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHBPeriphClockCmd(BEEP_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC, GPIO_Pin_4);  
}

void Run_BEEP(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_4);  
	delay_ms(100);
	GPIO_ResetBits(GPIOC, GPIO_Pin_4);  
}

//oled vcc输出初始化
void POWER_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void POWER_ON(int i)
{
	if(i == 1)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_5);  
	}
	else if(i == 0)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);  
	}
}


////无线开关
//void Wireless_Key_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure; 
//  RCC_AHBPeriphClockCmd(WIRELESS_KEY_CLK, ENABLE);
//	
//	GPIO_InitStructure.GPIO_Pin = WIRELESS_KEY_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//  GPIO_Init(WIRELESS_KEY_PORT, &GPIO_InitStructure);	
//}

