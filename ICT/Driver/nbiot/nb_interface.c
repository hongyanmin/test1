
#include "nb_interface.h"

static uint8_t	atRxBuf[512];
static char		atTxBuf[512];
char			tmpData[512];


void (*urcFunc)( const char *data, uint16_t size );

#define PP_HTONS( x ) ( ( ( (x) & 0xff) << 8) | ( ( (x) & 0xff00) >> 8) )

static uint8_t NBBCxx_checkHexStr( char* inData )
{
		int i;
    if ( (strlen( inData ) % 2) != 0 )
        return (1);
    for ( i = 0; i < strlen( inData ); i++ )
    {
        if ( (inData[i] >= '0' && inData[i] <= '9')
             || (inData[i] >= 'a' && inData[i] <= 'f')
             || (inData[i] >= 'A' && inData[i] <= 'F') )
        {
            ;
        }
        else
        {
            return (1);
        }
    }
    return (0);
}



uint8_t NBBCxx_checkModel( void )
{
    uint8_t res = 0;
    res = NBAT_sendCmd( "AT\r\n", "OK", 100, atRxBuf );
    return (res);
}


uint8_t NBBCxx_reboot( void )
{
    uint8_t res = 0;
    res = NBAT_sendCmd( "AT+NRB\r\n", "REBOOTING", 100, atRxBuf );
    return (res);
}


/*/////////////////////////////////// */
uint8_t NBBCxx_getACK( void )
{
    uint8_t res = 0;
    res = NBAT_sendCmd( "AT\r\n", "OK", 100, atRxBuf );
    return (res);
}


uint8_t NBBCxx_setCMEE( void )
{
    uint8_t res = 0;
    res = NBAT_sendCmd( "AT+CMEE=1\r\n", "OK", 100, atRxBuf );
    return (res);
}


uint8_t NBBCxx_getNBAND( void )
{
    uint8_t res = 0;
    res = NBAT_sendCmd( "AT+NBAND?\r\n", "OK", 100, atRxBuf );
		return (res);
}


uint8_t NBBCxx_getCIMI( void )
{
    uint8_t res = 0;
    res = NBAT_sendCmd( "AT+CIMI\r\n", "46", 100, atRxBuf );
    return (res);
}


uint8_t NBBCxx_setCGATT( void )
{
    uint8_t res = 0;
    res = NBAT_sendCmd( "AT+CGATT=1\r\n", "OK", 100, atRxBuf );
    return (res);
}


uint8_t NBBCxx_getCSQ( u8* csq )
{
    uint8_t res = 0;
    res = NBAT_sendCmd( "AT+CSQ\r\n", "OK", 100, atRxBuf );
    if ( res == 0 )
    {
        /*获取CSQ */
        uint8_t csqTmp = (atRxBuf[7] - 0x30) * 10 + (atRxBuf[8] - 0x30);
        if ( csqTmp == 99 )
        {
            return (3);
        }
        if ( csq != NULL )
            *csq = csqTmp;

        /*
         * NBBCxx_parser(atRxBuf);
         * printf( "[INFO]GETCSQ RESPONSE:\r\n%s %s\r\n", atRespLine[0],atRespLine[1]);
         */
    }
    return (res);
}


uint8_t NBBCxx_getCEREG( void )
{
    uint8_t res			= 0;
    char	*str0Res	= NULL;
    char	*str1Res	= NULL;
    res = NBAT_sendCmd( "AT+CEREG?\r\n", "OK", 100, atRxBuf );
    if ( res == 0 )
    {
        str0Res = (char *) strstr( (const char *) atRxBuf, (const char *) "+CEREG:0,1" );
        str1Res = (char *) strstr( (const char *) atRxBuf, (const char *) "+CEREG:1,1" );
        if ( str0Res == NULL && str1Res == NULL )
        {
            res = 1;
        }
    }
    return (res);
}


uint8_t NBBCxx_setCEREG( void )
{
    uint8_t res = 0;
    res = NBAT_sendCmd( "AT+CEREG=1\r\n", "OK", 100, atRxBuf );
    return (res);
}


uint8_t NBBCxx_getCOPS( void )
{
    uint8_t res			= 0;
    char	*str0Res	= NULL;
    char	*str1Res	= NULL;
    res = NBAT_sendCmd( "AT+COPS?\r\n", "OK", 100, atRxBuf );
    if ( res == 0 )
    {
        str0Res = (char *) strstr( (const char *) atRxBuf, (const char *) "46011" );
        str1Res = (char *) strstr( (const char *) atRxBuf, (const char *) "46000" );
        if ( (str0Res == NULL) && (str1Res == NULL) )
        {
            res = 1;
        }
    }
    return (res);
}


uint8_t NBBCxx_setNCDP( char *ipAdr,uint16_t port)
{
    uint8_t res = 0;
    sprintf( atTxBuf, "AT+NCDP=%s,%d\r\n", ipAdr ,port );
    res = NBAT_sendCmd( atTxBuf, "OK", 100, atRxBuf );
    return (res);
}


/*通过COAP协议发送数据，最大512字节*/
uint8_t NBBCxx_setNMGS_char( char *inData )
{
    uint8_t res = 0;
    sprintf( atTxBuf, "AT+NMGS=%d,%s\r\n", strlen( inData ) / 2, inData );
    res = NBAT_sendCmd( atTxBuf, "OK", 200, atRxBuf );
    return (res);
}


uint8_t NBBCxx_setCGDCONT( void )
{
    uint8_t res = 0;
    res = NBAT_sendCmd( "AT+CGDCONT=1,\042IP\042,\042HUAWEI.COM\042\r\n", "OK", 100, atRxBuf );
    return (res);
}


uint8_t NBBCxx_getCGATT( void )
{
    uint8_t res			= 0;
    char	*str0Res	= NULL;
    res = NBAT_sendCmd( "AT+CGATT?\r\n", "OK", 100, atRxBuf );
    if ( res == 0 )
    {
        str0Res = (char *) strstr( (const char *) atRxBuf, (const char *) "+CGATT:1" );
        if ( str0Res == NULL )
        {
            res = 1;
        }
    }
    return (res);
}


uint8_t NBBCxx_getCSCON( void )
{
    uint8_t res			= 0;
    char	*str0Res	= NULL;
    char	*str1Res	= NULL;
    res = NBAT_sendCmd( "AT+CSCON?\r\n", "OK", 100, atRxBuf );
    if ( res == 0 )
    {
        str0Res = (char *) strstr( (const char *) atRxBuf, (const char *) "+CSCON:0,1" );
        str1Res = (char *) strstr( (const char *) atRxBuf, (const char *) "+CSCON:0,0" );
        if ( str0Res == NULL && str1Res == NULL )
        {
            res = 1;
        }
    }
    return (res);
}


/*
 * ///////////////////////////////////////////
 * TCP/UDP
 */


/* brief :创建socket
 * return:0:成功  非0失败
 */
uint8_t NBBCxx_setNSOCR( BC95_SOCKETTypeDef type, uint16_t listePort, uint8_t *socketId )
{
    uint8_t res = 0;

    if ( type == kSOCKETType_TCP )
    {
        sprintf( atTxBuf, "AT+NSOCR=STREAM,6,%d,1\r\n", listePort );
    }
    else if ( type == kSOCKETType_UDP )
    {
        sprintf( atTxBuf, "AT+NSOCR=DGRAM,17,%d,1\r\n", listePort );
    }
    else
    {
        return (0xFF);
    }

    res = NBAT_sendCmd( atTxBuf, "OK", 100, atRxBuf );
    if ( res == 0 )
    {
        if ( atRxBuf[2] >= '0' && atRxBuf[2] <= '6' )
        {
            *socketId = atRxBuf[2] - '0';
            return (0);
        }
    }
    return (1);
}


/* brief :连接服务器(TCP)
 * return:0:成功  非0失败
 */
uint8_t NBBCxx_setNSOCOTcp( uint8_t socketId, char *ipAdr, uint16_t port )
{
    uint8_t res = 0;
    if ( socketId > 7 )
        return (1);
    sprintf( atTxBuf, "AT+NSOCO=%d,%s,%d\r\n", socketId, ipAdr, port );
    res = NBAT_sendCmd( atTxBuf, "OK", 3000, atRxBuf );
    return (res);
}


/* brief :UDP发送数据
 * return:0:成功  非0失败
 */
uint8_t NBBCxx_setNSOTSUdp( uint8_t socketId, char *ipAdr,
                            uint16_t port,
                            char *inData )
{
    uint8_t res = 0;
    if ( socketId > 7 )
        return (1);
    if ( NBBCxx_checkHexStr( inData ) )
        return (1);
    sprintf( atTxBuf, "AT+NSOTS=%d,%s,%d,%d,%s\r\n",
             socketId, ipAdr, port,
             strlen( inData ) / 2, inData );
    res = NBAT_sendCmd( atTxBuf, "OK", 30, atRxBuf );
    return (res);
}


/* brief :TCP发送数据
 * return:0:成功  非0失败
 */
uint8_t NBBCxx_setNSOSDTcp( uint8_t socketId,
                            char *inData )
{
    uint8_t res = 0;
    if ( socketId > 7 )
        return (1);
    if ( NBBCxx_checkHexStr( inData ) )
        return (2);
    sprintf( atTxBuf, "AT+NSOSD=%d,%d,%s\r\n",
             socketId, strlen( inData ) / 2, inData );
    res = NBAT_sendCmd( atTxBuf, "OK", 30, atRxBuf );
    return (res);
}


uint8_t NBBCxx_setNSORFTcp( uint8_t socketId, uint16_t readLen, char *outData )
{
    uint8_t res = 0;
    if ( socketId > 7 )
        return (1);
    sprintf( atTxBuf, "AT+NSORF=%d,%d\r\n",
             socketId, readLen );
    res = NBAT_sendCmd( atTxBuf, "OK", 300, (uint8_t*)outData );
    return (res);
}


void NBBCxx_setUrcTable( at_client_t client, const struct at_urc *urc_table, uint16_t table_sz )
{
    client->urc_table		= urc_table;
    client->urc_table_size	= table_sz;
}


static const struct at_urc *get_urc_obj( at_client_t client )
{
    uint16_t	i, prefix_len, suffix_len;
    uint16_t	buf_sz;
    char		*buffer = NULL;

    buffer	= (char*)client->recvBuf;
    buf_sz	= client->cur_recv_len;

    for ( i = 0; i < client->urc_table_size; i++ )
    {
        prefix_len	= strlen( client->urc_table[i].cmd_prefix );
        suffix_len	= strlen( client->urc_table[i].cmd_suffix );
        if ( buf_sz < prefix_len + suffix_len )
        {
            continue;
        }
        if ( (prefix_len ? !strncmp( buffer, client->urc_table[i].cmd_prefix, prefix_len ) : 1)
             && (suffix_len ? !strncmp( buffer + buf_sz - suffix_len, client->urc_table[i].cmd_suffix, suffix_len ) : 1) )
        {
            return (&client->urc_table[i]);
        }
    }
    return (NULL);
}


/* brief:非阻塞式接收数据
 * return:0:成功  非0失败
 */
uint8_t NBBCxx_getUrc( at_client_t client )
{
    uint8_t res = 0;
    res = NBAT_getUrc( client );
    if ( res == 0 )
    {
        const struct at_urc *urc;
        if ( (urc = get_urc_obj( client ) ) != NULL )
        {
            urc->func( (char*)client->recvBuf, client->cur_recv_len );
        }
    }
    return (res);
}

//低功耗模式,AT+CPSMS=1,,,01000011,01000011
uint8_t NB_LowPower_Mode(void)
{    
    uint8_t res = 0;
    res = NBAT_sendCmd( "AT+CPSMS=1,,,01000011,01000011\r\n", "OK", 100, atRxBuf );
//		res = NBAT_sendCmd( "AT+CEDRXS=1,5,\"0101\"\r\n", "OK", 100, atRxBuf );
    return (res);  
}
