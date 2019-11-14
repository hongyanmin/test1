#include "lpm.h"
//低功耗模式

//退出休眠模式需设置时钟
void Run_Mode(void)
{
	/* 使能 HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  
  /* 等待 HSE 准备就绪 */
  while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
  {
  }
  
  /* 使能 PLL */ 
  RCC_PLLCmd(ENABLE);
  
  /* 等待 PLL 准备就绪 */
  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
  {
  }
  
  /* 选择PLL作为系统时钟源 */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  
  /* 等待PLL被选择为系统时钟源 */
  while (RCC_GetSYSCLKSource() != 0x0C)
  {
  }
	
	RCC_HSICmd(ENABLE);/* Enable the HSI oscillator */    
  while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);/* Check that HSI oscillator is ready */
	
}

//进入休眠模式
void Stop_Mode(void)
{
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR, ENABLE);
//	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
	PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
}








