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
   while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//等待发送数据完毕
   return ch;
}

void USART1_Configuration(u32 baund)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//使能USART1
    RCC_AHBPeriphClockCmd(USART_TX_CLK|USART_RX_CLK, ENABLE);	//GPIOA时钟
	
 	  USART_DeInit(USART1);  //复位串口1
    
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
		
		USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//开启USART1的接收中断
		//Usart1 NVIC 配置
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
		
		USART_ClearFlag(USART1,USART_FLAG_TC); //清除发送完成标志位 	
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

