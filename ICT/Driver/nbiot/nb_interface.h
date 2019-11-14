
#ifndef __NB_INTERFACE_H
#define __NB_INTERFACE_H

#include "nb_at.h"
#include "stm32l1xx.h"
#include "string.h"
#include "stdio.h"

typedef struct
{
    uint8_t adr0;
    uint8_t adr1;
    uint8_t adr2;
    uint8_t adr3;
} SERVER_INFO;

typedef enum
{
    kSOCKETType_INIT	= 0x00,
    kSOCKETType_TCP		= 0x01,
    kSOCKETType_UDP		= 0x02,
    kSOCKETType_END		= 0x03,
} BC95_SOCKETTypeDef;

uint8_t NBBCxx_getACK( void );
uint8_t NBBCxx_reboot( void );
uint8_t NBBCxx_checkModel( void );
uint8_t NBBCxx_getACK( void );
uint8_t NBBCxx_setCMEE( void );
uint8_t NBBCxx_getNBAND( void );
uint8_t NBBCxx_getCIMI( void );
uint8_t NBBCxx_setCGATT( void );
uint8_t NBBCxx_getCSQ( u8* csq );
uint8_t NBBCxx_getCEREG( void );
uint8_t NBBCxx_setCEREG( void );
uint8_t NBBCxx_getCOPS( void );
uint8_t NBBCxx_setCGDCONT( void );
uint8_t NBBCxx_getCGATT( void );
uint8_t NBBCxx_getCSCON( void );
uint8_t NBBCxx_setNCDP( char *ipAdr ,uint16_t port);
uint8_t NBBCxx_setNMGS_char( char *inData );
uint8_t NBBCxx_setNSOCR( BC95_SOCKETTypeDef type, uint16_t listePort, uint8_t *socketId );
uint8_t NBBCxx_setNSOCOTcp( uint8_t socketId, char *ipAdr, uint16_t port );
uint8_t NBBCxx_setNSOTSUdp( uint8_t socketId, char *ipAdr, uint16_t port, char *inData );
uint8_t NBBCxx_setNSOSDTcp( uint8_t socketId, char *inData );
uint8_t NBBCxx_getUrc( at_client_t client );
void NBBCxx_setUrcTable( at_client_t client, const struct at_urc *urc_table, uint16_t table_sz );
uint8_t NBBCxx_setNSORFTcp( uint8_t socketId, uint16_t readLen, char *outData );
uint8_t NB_LowPower_Mode(void);		
#endif
