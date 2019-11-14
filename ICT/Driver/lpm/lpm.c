#include "lpm.h"
//�͹���ģʽ

//�˳�����ģʽ������ʱ��
void Run_Mode(void)
{
	/* ʹ�� HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  
  /* �ȴ� HSE ׼������ */
  while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
  {
  }
  
  /* ʹ�� PLL */ 
  RCC_PLLCmd(ENABLE);
  
  /* �ȴ� PLL ׼������ */
  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
  {
  }
  
  /* ѡ��PLL��Ϊϵͳʱ��Դ */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  
  /* �ȴ�PLL��ѡ��Ϊϵͳʱ��Դ */
  while (RCC_GetSYSCLKSource() != 0x0C)
  {
  }
	
	RCC_HSICmd(ENABLE);/* Enable the HSI oscillator */    
  while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);/* Check that HSI oscillator is ready */
	
}

//��������ģʽ
void Stop_Mode(void)
{
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR, ENABLE);
//	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
	PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
}








