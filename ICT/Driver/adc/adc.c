#include "adc.h"
			   
u16 ADC_ConvertedValue[ADC_CHANNEL_NUM];

void Adc_Init(void)
{    
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure; 
	
	RCC_AHBPeriphClockCmd(ADC_VOL_GPIO_CLK|ADC_LIGHT1_GPIO_CLK|ADC_LIGHT2_GPIO_CLK,ENABLE);//ʹ��GPIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE );//ʹ��ADC1ͨ��ʱ��
 
  GPIO_InitStructure.GPIO_Pin = ADC_VOL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;  //ģ������
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//ʱ������
  GPIO_Init(ADC_VOL_GPIO_PORT, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = ADC_LIGHT1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;  //ģ������
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//ʱ������
  GPIO_Init(ADC_LIGHT1_GPIO_PORT, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = ADC_LIGHT2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;  //ģ������
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//ʱ������
  GPIO_Init(ADC_LIGHT2_GPIO_PORT, &GPIO_InitStructure); 
	
	RCC_HSICmd(ENABLE);/* Enable the HSI oscillator */    
  while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);/* Check that HSI oscillator is ready */
	
	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
		
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = ADC_CHANNEL_NUM;
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   
	  
	DMA_init();
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);  
	
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	Adc_Key_Init();
	ADC_SoftwareStartConv(ADC1);
	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//�ȴ�ת������
}	

void DMA_init(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                       //����DMAʱ��
	
	DMA_DeInit(DMA1_Channel1);                                             //DMA��λ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));             //ADC��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)&ADC_ConvertedValue;     //�洢��ַ
	DMA_InitStructure.DMA_DIR                        = DMA_DIR_PeripheralSRC;     //DMA���䷽��Ϊ�豸���ڴ�
	DMA_InitStructure.DMA_BufferSize               = ADC_CHANNEL_NUM;                   //�洢��С������ǰ���ADC�����õ�������ת��
	DMA_InitStructure.DMA_PeripheralInc          = DMA_PeripheralInc_Disable;                //����ַ�̶�                 
	DMA_InitStructure.DMA_MemoryInc            = DMA_MemoryInc_Enable;                 //�ڴ��ַ������������ADCת�����������ݲ������δ���Ŀ���ַ�ϣ�����Ļ����ǹ̶�д��һ����ַ�ϣ�����ǰ������ݰѺ���ĸ�����                                                                             
	DMA_InitStructure.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_HalfWord;     //ÿ�η����ǰ��֣���16λ
	DMA_InitStructure.DMA_MemoryDataSize    = DMA_MemoryDataSize_HalfWord;           //ÿ�δ洢���ݵ�λΪ���֣���16λ
	DMA_InitStructure.DMA_Mode             	= DMA_Mode_Circular;                     //ѭ��AD
	DMA_InitStructure.DMA_Priority                  = DMA_Priority_High;      //DMA���ȼ���
	DMA_InitStructure.DMA_M2M                      = DMA_M2M_Disable;         //DMA��ֹ�ڴ浽�ڴ�
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);                              //DMA��ʼ��
	
	ADC_RegularChannelConfig(ADC1, ADC_VOL_CH, 1, ADC_SampleTime_384Cycles);//ADC_SampleTime_48Cycles
	ADC_RegularChannelConfig(ADC1, ADC_LIGHT1_CH, 2, ADC_SampleTime_384Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_LIGHT2_CH, 3, ADC_SampleTime_384Cycles);
	
	DMA_Cmd(DMA1_Channel1, ENABLE);                                           //����DMAͨ��1	
}

void Adc_Key_Init(void)//ADC������س�ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(KEY_VOL_GPIO_CLK|KEY_LIGHT1_GPIO_CLK|KEY_LIGHT2_GPIO_CLK,ENABLE);//ʹ��GPIOʱ��
 
  GPIO_InitStructure.GPIO_Pin = KEY_VOL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;//����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//ʱ������
  GPIO_Init(KEY_VOL_GPIO_PORT, &GPIO_InitStructure); 

	
	GPIO_InitStructure.GPIO_Pin = KEY_LIGHT1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;//����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//ʱ������
  GPIO_Init(KEY_LIGHT1_GPIO_PORT, &GPIO_InitStructure); 
	GPIO_SetBits(KEY_LIGHT1_GPIO_PORT,KEY_LIGHT1_PIN);
	
	GPIO_InitStructure.GPIO_Pin = KEY_LIGHT2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;//����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//ʱ������
  GPIO_Init(KEY_LIGHT2_GPIO_PORT, &GPIO_InitStructure); 
	GPIO_SetBits(KEY_LIGHT2_GPIO_PORT,KEY_LIGHT2_PIN);
}

void Adc_Key_On(void)//ADC������ش�
{
		GPIO_ResetBits(KEY_VOL_GPIO_PORT,KEY_VOL_PIN);
		GPIO_ResetBits(KEY_LIGHT1_GPIO_PORT,KEY_LIGHT1_PIN);
		GPIO_ResetBits(KEY_LIGHT2_GPIO_PORT,KEY_LIGHT2_PIN);	
}

void Adc_Key_Off(void)//ADC������عر�
{
		GPIO_SetBits(KEY_VOL_GPIO_PORT,KEY_VOL_PIN);
		GPIO_SetBits(KEY_LIGHT1_GPIO_PORT,KEY_LIGHT1_PIN);
		GPIO_SetBits(KEY_LIGHT2_GPIO_PORT,KEY_LIGHT2_PIN);	
}
	
//���ADCֵ
//ch:ͨ��ֵ 0~16
//����ֵ:ת�����
u16 Get_Adc(u8 ch)   
{ 
	return ADC_ConvertedValue[ch];
}

//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
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

