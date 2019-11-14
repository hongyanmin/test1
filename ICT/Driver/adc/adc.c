#include "adc.h"
			   
u16 ADC_ConvertedValue[ADC_CHANNEL_NUM];

void Adc_Init(void)
{    
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure; 
	
	RCC_AHBPeriphClockCmd(ADC_VOL_GPIO_CLK|ADC_LIGHT1_GPIO_CLK|ADC_LIGHT2_GPIO_CLK,ENABLE);//使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE );//使能ADC1通道时钟
 
  GPIO_InitStructure.GPIO_Pin = ADC_VOL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;  //模拟输入
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//时钟速率
  GPIO_Init(ADC_VOL_GPIO_PORT, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = ADC_LIGHT1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;  //模拟输入
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//时钟速率
  GPIO_Init(ADC_LIGHT1_GPIO_PORT, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = ADC_LIGHT2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;  //模拟输入
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//时钟速率
  GPIO_Init(ADC_LIGHT2_GPIO_PORT, &GPIO_InitStructure); 
	
	RCC_HSICmd(ENABLE);/* Enable the HSI oscillator */    
  while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);/* Check that HSI oscillator is ready */
	
	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值
		
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = ADC_CHANNEL_NUM;
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
	  
	DMA_init();
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);  
	
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	Adc_Key_Init();
	ADC_SoftwareStartConv(ADC1);
	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//等待转换结束
}	

void DMA_init(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                       //开启DMA时钟
	
	DMA_DeInit(DMA1_Channel1);                                             //DMA复位
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));             //ADC地址
	DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)&ADC_ConvertedValue;     //存储地址
	DMA_InitStructure.DMA_DIR                        = DMA_DIR_PeripheralSRC;     //DMA传输方向为设备到内存
	DMA_InitStructure.DMA_BufferSize               = ADC_CHANNEL_NUM;                   //存储大小，所以前面的ADC我设置的是连续转换
	DMA_InitStructure.DMA_PeripheralInc          = DMA_PeripheralInc_Disable;                //外存地址固定                 
	DMA_InitStructure.DMA_MemoryInc            = DMA_MemoryInc_Enable;                 //内存地址递增，这样从ADC转换过来的数据才能依次传到目标地址上，否则的话就是固定写在一个地址上，并且前面的数据把后面的覆盖了                                                                             
	DMA_InitStructure.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_HalfWord;     //每次发送是半字，即16位
	DMA_InitStructure.DMA_MemoryDataSize    = DMA_MemoryDataSize_HalfWord;           //每次存储数据单位为半字，即16位
	DMA_InitStructure.DMA_Mode             	= DMA_Mode_Circular;                     //循环AD
	DMA_InitStructure.DMA_Priority                  = DMA_Priority_High;      //DMA优先级高
	DMA_InitStructure.DMA_M2M                      = DMA_M2M_Disable;         //DMA禁止内存到内存
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);                              //DMA初始化
	
	ADC_RegularChannelConfig(ADC1, ADC_VOL_CH, 1, ADC_SampleTime_384Cycles);//ADC_SampleTime_48Cycles
	ADC_RegularChannelConfig(ADC1, ADC_LIGHT1_CH, 2, ADC_SampleTime_384Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_LIGHT2_CH, 3, ADC_SampleTime_384Cycles);
	
	DMA_Cmd(DMA1_Channel1, ENABLE);                                           //启动DMA通道1	
}

void Adc_Key_Init(void)//ADC软件开关初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(KEY_VOL_GPIO_CLK|KEY_LIGHT1_GPIO_CLK|KEY_LIGHT2_GPIO_CLK,ENABLE);//使能GPIO时钟
 
  GPIO_InitStructure.GPIO_Pin = KEY_VOL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;//上拉
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//时钟速率
  GPIO_Init(KEY_VOL_GPIO_PORT, &GPIO_InitStructure); 

	
	GPIO_InitStructure.GPIO_Pin = KEY_LIGHT1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;//上拉
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//时钟速率
  GPIO_Init(KEY_LIGHT1_GPIO_PORT, &GPIO_InitStructure); 
	GPIO_SetBits(KEY_LIGHT1_GPIO_PORT,KEY_LIGHT1_PIN);
	
	GPIO_InitStructure.GPIO_Pin = KEY_LIGHT2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;//上拉
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//时钟速率
  GPIO_Init(KEY_LIGHT2_GPIO_PORT, &GPIO_InitStructure); 
	GPIO_SetBits(KEY_LIGHT2_GPIO_PORT,KEY_LIGHT2_PIN);
}

void Adc_Key_On(void)//ADC软件开关打开
{
		GPIO_ResetBits(KEY_VOL_GPIO_PORT,KEY_VOL_PIN);
		GPIO_ResetBits(KEY_LIGHT1_GPIO_PORT,KEY_LIGHT1_PIN);
		GPIO_ResetBits(KEY_LIGHT2_GPIO_PORT,KEY_LIGHT2_PIN);	
}

void Adc_Key_Off(void)//ADC软件开关关闭
{
		GPIO_SetBits(KEY_VOL_GPIO_PORT,KEY_VOL_PIN);
		GPIO_SetBits(KEY_LIGHT1_GPIO_PORT,KEY_LIGHT1_PIN);
		GPIO_SetBits(KEY_LIGHT2_GPIO_PORT,KEY_LIGHT2_PIN);	
}
	
//获得ADC值
//ch:通道值 0~16
//返回值:转换结果
u16 Get_Adc(u8 ch)   
{ 
	return ADC_ConvertedValue[ch];
}

//获取通道ch的转换值，取times次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=ADC_ConvertedValue[ch];
		delay_ms(5);
	}
	return temp_val/times;
} 

