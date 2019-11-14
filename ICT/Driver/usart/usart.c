#include "usart.h"
u8 flag;

void USART1_OUT(USART_TypeDef* USARTx, uint8_t *Data,uint16_t Len)
{ 
	uint16_t i;
	for(i=0; i<Len; i++){
		USART_SendData(USARTx, Data[i]);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
	}
}
int fputc(int ch,FILE *f)
{
   USART_SendData(USART1,(u8)ch);
   while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//�ȴ������������
   return ch;
}

void USART1_Configuration(u32 baund)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//ʹ��USART1
    RCC_AHBPeriphClockCmd(USART_TX_CLK|USART_RX_CLK, ENABLE);	//GPIOAʱ��
	
 	  USART_DeInit(USART1);  //��λ����1
    
	  //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = USART_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	
    GPIO_Init(USART_TX_PORT, &GPIO_InitStructure); 
    
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = USART_RX_PIN;
    GPIO_Init(USART_RX_PORT, &GPIO_InitStructure);

	  GPIO_PinAFConfig(USART_TX_PORT, USART_TX_PIN_SOURCE, GPIO_AF_USART1);
	  GPIO_PinAFConfig(USART_RX_PORT, USART_RX_PIN_SOURCE, GPIO_AF_USART1); 
    
    USART_InitStructure.USART_BaudRate = baund;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1,&USART_InitStructure);
		
    USART_Cmd(USART1,ENABLE);
		
		USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//����USART1�Ľ����ж�
		//Usart1 NVIC ����
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
		
		USART_ClearFlag(USART1,USART_FLAG_TC); //���������ɱ�־λ 	
}

void USART1_Send(uint8_t *buf,uint16_t len)
{
  while(len--)
  {  
    USART_SendData(USART1, (uint16_t)*buf);
    buf++;
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);    
  }
}

