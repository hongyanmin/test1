
#include "nb_process.h"

/*COAP服务器配置 */
#define NBIOT_COAP_SERVICE_IP 	"180.101.147.115" //"49.4.85.232"
#define NBIOT_COAP_SERVICE_PORT 5683

uint32_t			gtime3Count;
uint8_t sendDataCount = 0;   /*静态全局变量 */

int8_t nb_csq = 0;//信号强度

const char	hexStrTable[] = { 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46 };
uint8_t		g_socketId = 0;
char	tmpBuf1[512];
char	tmpBuf2[512];

void hexToStr( char *inBuf, char *outBuf,uint16_t len)
{
    uint16_t i = 0;
    for (; i < len; i++ )
    {
        outBuf[2 * i + 0]	= hexStrTable[(inBuf[i] >> 4)];
        outBuf[2 * i + 1]	= hexStrTable[(inBuf[i] & 0x0F)];
    }
    outBuf[2 * i + 0] = 0;
}

void hexToStr1( char *inBuf, char *outBuf )
{
    uint16_t i = 0;
    for (; i < 5; i++ )
    {
        outBuf[2 * i + 0]	= hexStrTable[(inBuf[i] >> 4)];
        outBuf[2 * i + 1]	= hexStrTable[(inBuf[i] & 0x0F)];
    }
    outBuf[2 * i + 0] = 0;
}

uint8_t strToHex( char *inBuf, char *outBuf, uint32_t *outLen )
{
    uint16_t	i	= 0;
    uint8_t		tmp = 0;
    if ( (strlen( inBuf ) % 2) != 0 )
        return (1);
    for (; i < (uint16_t) strlen( inBuf ); i++ )
    {
        tmp = 0;
        if ( inBuf[i] >= '0' && inBuf[i] <= '9' )
        {
            tmp = inBuf[i] - '0';
        }
        else if ( inBuf[i] >= 'A' && inBuf[i] <= 'F' )
        {
            tmp = inBuf[i] - 'A' + 0x0A;
        }
        else if ( inBuf[i] >= 'a' && inBuf[i] <= 'f' )
        {
            tmp = inBuf[i] - 'A' + 0x0A;
        }
        else
        {
            return (2);
        }
        if ( (i % 2) == 0 )
            outBuf[i / 2] = tmp << 4;
        else
            outBuf[i / 2] |= tmp;
    }
    *outLen = strlen( inBuf ) / 2;
    return (0);
}


/*1ms定时器 */
void TIM3_init(void)
{	
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  
		TIM_TimeBaseStructure.TIM_Period = 1000-1;//Tout= ((arr+1)*(psc+1))/Tclk；
		TIM_TimeBaseStructure.TIM_Prescaler = 32-1;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		/* TIM IT enable */
//		TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);
	
		/* TIM3 enable counter */
//		TIM_Cmd(TIM3, ENABLE);			

		/* Enable the TIM3 Interrupt */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);	
}


/*TIME3中断处理函数 */
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		gtime3Count++;
 //   TIM_Cmd(TIM3, DISABLE);
  }
}


static void urc_connect_func( const char *data, uint16_t size )
{
//    printf( "[INFO]urc_connect_func\r\n" );
}


/*收到数据的回调函数 */
static void urc_recvdata_func( const char *data, uint16_t size )
{
    char outData[96];
    memset( outData, 0x00, sizeof(outData) );
//    printf( "[INFO]urc_recvdata_func\r\n" );
    NBBCxx_setNSORFTcp( g_socketId, 17, outData );
//    printf( "[INFO]recv data:%s\r\n", outData );
}


/*URC设置 */
static const struct at_urc urc_table[] = {
    { "\r\nCONNECT",  "\r\n", urc_connect_func	},
    { "\r\n+NSONMI:", "\r\n", urc_recvdata_func },
};

void NBBCxx_init(void)
{
//	  TIM3_init();
    TIM4_init();
}

int NB_Connect(void)
{
    uint8_t stup	= 0;
    uint8_t res		= 0;
    uint8_t cnt		= 0;
    uint8_t csq		= 0;

    uint8_t mainStup	= 0;    /*主流程 */
    uint8_t subStup		= 0;    /*子流程 */

    struct at_client	clientObj;
    at_client_t			client = &clientObj;
    NBBCxx_setUrcTable( client, urc_table, sizeof(urc_table) / sizeof(urc_table[0]) );

    mainStup	= NBState_INIT;
    subStup		= NBInit_CHECKMODEL;

//		sendDataCount = 0;
	
		while(1)
		{
        /*NBIOT start */
        if ( !( (mainStup == NBState_INIT) && (subStup == NBInit_CHECKMODEL) ) )
        {
            NBBCxx_getUrc( client );
        }
        switch ( mainStup )
        {
        case NBState_INIT:
            if ( subStup == NBInit_CHECKMODEL )
            {
                cnt = 10;
                do
                {
                    res = NBBCxx_checkModel();
                    delay_ms( 500 );
                }
                while ( (cnt--) != 0 && (res != 0) );
                REQUIRE_noErr( res != 0x00, error, stup = 1 );
                subStup = NBInit_REBOOT;
            }
            else if ( subStup == NBInit_REBOOT )
            {
                NBBCxx_reboot();
                cnt = 10;
                do
                {
                    res = NBBCxx_checkModel();
                    delay_ms( 1000 );
                }
                while ( (cnt--) != 0 && (res != 0) );
                REQUIRE_noErr( res != 0x00, error, stup = 2 );
                subStup = NBInit_GETCIMI;
            }
            else if ( subStup == NBInit_GETCIMI )
            {
                cnt = 10;
                do
                {
                    res = NBBCxx_getCIMI();
                    delay_ms( 1000 );
                }
                while ( (cnt--) != 0 && (res != 0) );
                REQUIRE_noErr( res != 0x00, error, stup = 5 );
                subStup = NBInit_GETCSQ;
            }
            else if ( subStup == NBInit_GETCSQ )
            {
                cnt = 50;
                do
                {
                    res = NBBCxx_getCSQ( &csq  );
										nb_csq = csq ;
                    delay_ms( 1000 );
                }
                while ( (cnt--) != 0 && (res != 0) );
                REQUIRE_noErr( res != 0x00, error, stup = 7 );
                subStup = NBInit_GETCEREG;
            }
            else if ( subStup == NBInit_GETCEREG )
            {
                cnt = 50;
                do
                {
                    res = NBBCxx_getCEREG();
                    delay_ms( 1000 );
                }
                while ( (cnt--) != 0 && (res != 0) );
                REQUIRE_noErr( res != 0x00, error, stup = 8 );

                mainStup	= NBState_COAPCONNET;
                subStup		= NBCoap_CONNECTNET;
            }
            break;

        case NBState_COAPCONNET:
        {
            if ( subStup == NBCoap_CONNECTNET )
            {
                /*设置COAP服务器 */
                res = NBBCxx_setNCDP( NBIOT_COAP_SERVICE_IP ,NBIOT_COAP_SERVICE_PORT);
                REQUIRE_noErr( res != 0x00, error, stup = 9 );
                mainStup	= NBState_END;
            }
        }
        break;   
        case NBState_END:
						//TIM_Cmd(TIM3, DISABLE);
        goto success;        
       }
		 }
success:
    return (0);
error:
    while ( 1 );
}

int NB_DataSend(char* pdata,uint8_t len)
{
	uint8_t stup	= 0;
	uint8_t res		= 0;
	
	
	while(1)
	{
#if 0
		if ( sendDataCount == 0 )
		{                                   /*第一次发送数据，启动time3 */
			TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);
			TIM_Cmd(TIM3, ENABLE);									
		}
		else
		{
			if ( gtime3Count <= 2000 )
			{
				/*通过中断函数，延时2秒来处理，时间未到时,只做查询 */
				continue;
			}
		}
#endif		
		/*COAP发送数据 */
		memcpy(tmpBuf1,pdata,len);
									
		/*进行格式转换 */
		memset(tmpBuf2, 0x00, sizeof(tmpBuf2));
		hexToStr(tmpBuf1, tmpBuf2,len*2);
				
		NBBCxx_setNMGS_char(tmpBuf2);
		REQUIRE_noErr(res != 0x00, error, stup = 12);
		
//		sendDataCount++;	
//		gtime3Count = 0;
		goto success;	
}
success:
    return (0);
error:
    while ( 1 );
}

void NB_Close(void)
{
//	TIM_Cmd(TIM3, DISABLE);
//	sendDataCount = 0;
}


