#ifndef _usart_H
#define _usart_H

#include "pbdata.h"
#include "general_type.h"
#include "stm32l1xx_usart.h"

#define USART_TX_PORT				GPIOA
#define USART_TX_PIN 				GPIO_Pin_9
#define USART_TX_CLK    		RCC_AHBPeriph_GPIOA
#define USART_TX_PIN_SOURCE	GPIO_PinSource9

#define USART_RX_PORT 			GPIOA
#define USART_RX_PIN 				GPIO_Pin_10
#define USART_RX_CLK    		RCC_AHBPeriph_GPIOA
#define USART_RX_PIN_SOURCE	GPIO_PinSource10

void USART1_Configuration(u32 baund);
void USART1_OUT(USART_TypeDef* USARTx, uint8_t *Data,uint16_t Len);
int fputc(int ch,FILE *f);
void USART1_Send(uint8_t *buf,uint16_t len);
#endif
