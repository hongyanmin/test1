
#include "nb_at.h"
#include "rtc.h"

static uint16_t				uartxRxstate = 0;
static uint8_t				uartxRxBuf[512];
static uint8_t				uartxTxBuf[512];

uint8_t								NB_RxBuf[512];
uint16_t							NB_RxLen = 0;

extern uint8_t data_process_flag;//数据处理标志位

#define    UARTX_RECV_END (0x8000)

uint8_t NBBCxx_parser( ATRespLine_TypeDef *resp, char* inData )
{
		int i;
    char	ch;
    char	last_ch;
    uint8_t lineNum = 0;
    uint8_t pos		= 0;
    for (  i = 0; i < strlen( inData ); i++ )
    {
        ch = inData[i];
        resp->respBuf[lineNum][pos++]	= ch;

        /* is newline or URC data */
        if ( last_ch == '\r' && ch == '\n' )
        {
            if ( pos == 2 )
            {
                /*只有\r\n */
                pos = 0;
                continue;
            }
            resp->respBuf[lineNum][pos - 2] = '\0';
            lineNum++;
            pos = 0;
        }
        last_ch = ch;
    }
    resp->respBuf[lineNum][0] = '\0';
    return (0);
}


void USART1_IRQHandler(void)
{
		uint8_t		res;
   if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET) //判断是否产生接收中断
	 {
			res=USART_ReceiveData(USART1);
      if ( (uartxRxstate & UARTX_RECV_END) == 0 )
      {
          if ( uartxRxstate == 0x00 )
          {
              memset( (uint8_t *) uartxRxstate, 0x00, sizeof(uartxRxstate) );
							TIM_ITConfig(TIM4,TIM_IT_Update, ENABLE);
							TIM_Cmd(TIM4, ENABLE);	
          }
          if ( uartxRxstate < sizeof(uartxRxBuf) )
          {
						   uartxRxBuf[uartxRxstate++] = res;       /*记录接收到的值 */
          }
          else
          {
               uartxRxstate |= UARTX_RECV_END;         /*强制标记接收完成 */
          }
       }
	 } 
}

extern void hexToStr1( char *inBuf, char *outBuf );
extern uint8_t NBBCxx_setNMGS_char( char *inData );
void NBBCxx_recv(void)
{
	char	tmpBuf3[512];
  char	tmpBuf4[512];
	if((uartxRxstate & UARTX_RECV_END) == UARTX_RECV_END)
	{
		if(strncmp((char *)&uartxRxBuf[2],"+NNMI:",6)== 0)//前面两个字节0D0A，后面是长度、逗号、数据内容
    {
			uartxRxstate = 0;

//set test			
			tmpBuf3[0]=0x56;//用strlen数据内容不能为0
			tmpBuf3[1]=0x00;
			tmpBuf3[2]=uartxRxBuf[15]-'0';
			tmpBuf3[3]=0x00;
			tmpBuf3[4]=0x03;
			
			/*进行格式转换 */
			memset(tmpBuf4, 0x00, sizeof(tmpBuf4));
      hexToStr1(tmpBuf3, tmpBuf4);
      NBBCxx_setNMGS_char(tmpBuf4);
    }
	}    
}

/*300ms定时器，用于串口数据接收延时 */
void TIM4_init(void)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  
		TIM_TimeBaseStructure.TIM_Period = 3000-1;//Tout= ((arr+1)*(psc+1))/Tclk；
		TIM_TimeBaseStructure.TIM_Prescaler = 3200-1;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		/* TIM IT enable */
//		TIM_ITConfig(TIM4,TIM_IT_Update, ENABLE);
	
		/* TIM4 enable counter */
//		TIM_Cmd(TIM4, ENABLE);			

		/* Enable the TIM4 Interrupt */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);	
}

void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  
		if ( (uartxRxstate & 0x7FFF) != 0 )
		{
					uartxRxstate |= UARTX_RECV_END;
					TIM_Cmd(TIM4, DISABLE);
		
					if(strncmp((char *)&uartxRxBuf[2],"+NNMI:",6)== 0)//前面两个字节0D0A，后面是长度、逗号、数据内容,最后0D0A
					{
								 NB_RxLen = (uartxRxstate&0x7FFF) - 10;
								 memset(NB_RxBuf,0,512);
								 memcpy(NB_RxBuf,&uartxRxBuf[8],NB_RxLen);
								 data_process_flag |= DATA_RECEIVE_UP;
								 uartxRxstate = 0;
					}
		
		}
	}
}

void UARTx_write( uint8_t *buf, uint16_t length )
{
    uint16_t i;
    for ( i = 0; i < length; i++ )  /*循环发送数据 */
    {
        while ( (USART2->SR & 0x40) == 0 );
        USART2->DR = buf[i];
    }
}


int NBAT_send( void *buf, uint32_t len, uint32_t xTimeOut )
{
    if ( len > 1024 )
    {
        return (0);
    }
    USART1_Send( buf, len );
    return (len);
}


char* NBAT_checkCmd( char *str )
{
    char *strx = 0;
    if ( uartxRxstate & UARTX_RECV_END )
    {                                               /*接收到一次数据了 */
        uartxRxBuf[uartxRxstate & 0x7FFF]	= 0;    /*添加结束符 */
        strx	= (char *) strstr( (const char *) uartxRxBuf, (const char *) str );
    }
    return (strx);
}


uint8_t NBAT_sendCmd( char *cmd, char *ack, uint16_t waittime, uint8_t *outResponse )
{
    uint8_t		res		= 0;
    uint16_t	sendLen = 0;
    uartxRxstate = 0;
    if ( strlen( cmd ) > sizeof(uartxTxBuf) )
    {
        return (2);
    }
    if ( (uint32_t) cmd <= 0xFF )
    {
        while ( (USART2->SR & 0x40) == 0 );                                       /*等待上一次数据发送完成 */
        USART2->DR = (uint32_t) cmd;
    }
    else
    {
        sendLen = sprintf( (char *) uartxTxBuf, "%s", cmd );
        USART1_Send( uartxTxBuf, sendLen );         /*发送命令 */
    }
    if ( ack && waittime )                          /*需要等待应答 */
    {
        while ( --waittime )                        /*等待倒计时 */
        {
            delay_ms(10);
            if ( uartxRxstate & UARTX_RECV_END )    /*接收到期待的应答结果 */
            {
                if ( NBAT_checkUrc( (uint8_t *) ack ) == 0 )
                {
                }
                else if ( NBAT_checkCmd( ack ) )
                {
                    memcpy( outResponse, uartxRxBuf, uartxRxstate & 0x7FFF );
                    outResponse[uartxRxstate & 0x7FFF]	= 0;
                    uartxRxstate	= 0;
                    break;                          /*得到有效数据 */
                }
                else if ( NBAT_checkCmd( "ERROR" ) )
                {
                    memcpy( outResponse, uartxRxBuf, uartxRxstate & 0x7FFF );
                    outResponse[uartxRxstate & 0x7FFF]	= 0;
                    uartxRxstate = 0;
                    res	= 2;
                    break;
                }
            }
        }
        if ( waittime == 0 )
            res = 1;
    }
    return (res);
}


uint8_t NBAT_getUrc( at_client_t client )
{
    if ( (uartxRxstate & UARTX_RECV_END) == UARTX_RECV_END )
    {
        /*NBBCxx_parser( client->resp, uartxRxBuf ); */
        uartxRxstate			&= (~UARTX_RECV_END);
        client->cur_recv_len	= uartxRxstate;
        memcpy( client->recvBuf, uartxRxBuf, uartxRxstate );
        uartxRxstate = 0;
        return (0);
    }

    return (1);
}


uint8_t NBAT_checkUrc( uint8_t *outResponse )
{
    if ( strstr( (char *) outResponse, "+NSOMI" ) )
    {
    }
    return (1);
}


uint8_t NBAT_recvData( char *ack, uint16_t waittime, uint8_t *outResponse )
{
    uint8_t res = 0;
    if ( (uartxRxstate & UARTX_RECV_END) == 0 )
        return (2);
    if ( ack && waittime )                              /*需要等待应答 */
    {
        /*等待倒计时 */
        do
        {
            delay_ms( 10 );
            if ( uartxRxstate & UARTX_RECV_END )        /*接收到期待的应答结果 */
            {
                if ( NBAT_checkUrc( (uint8_t *) ack ) == 0 )
                {

                }
                else if ( NBAT_checkCmd( ack ) )
                {
                    memcpy( outResponse, uartxRxBuf, uartxRxstate & 0x7FFF );
                    outResponse[uartxRxstate & 0x7FFF]	= 0;
                    uartxRxstate						= 0;
                    break;                              /*得到有效数据 */
                }
                else if ( NBAT_checkCmd( "ERROR" ) )
                {

                }
            }
        }
        while ( --waittime );
        if ( waittime == 0 )
            res = 1;
    }
    return (res);
}

