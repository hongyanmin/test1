#ifndef __NB_PROCESS_H
#define __NB_PROCESS_H

#include "stm32l1xx.h"
#include "delay.h"
#include "nb_interface.h"
#include "stdio.h"
#include "string.h"
#include "misc.h"
#include "stm32l1xx_tim.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_rcc.h"
#include "nb_at.h"

typedef enum
{
    NBDemoPath_NONE,
    NBDemoPath_TCP,
    NBDemoPath_COAP,
    NBDemoPath_END
}NBAppDemoPath_t;

typedef enum
{
    NBState_NONE,
    NBState_INIT,
    NBState_TCPCONNET,
    NBState_COAPCONNET,
    NBState_PROCESS,
    NBState_END
}NBState_t;

typedef enum
{
    NBInit_NONE = 0x10,
    NBInit_CHECKMODEL,
    NBInit_REBOOT,
    NBInit_GETCIMI,
    NBInit_GETCSQ,
    NBInit_GETCEREG,
    NBInit_END
}NBInitStup_t;

typedef enum
{
    NBTcp_NONE = 0x20,
    NBTcp_CREATERSOCKET,
    NBTcp_CONNECTNET,
    NBTcp_END
}NBTcp_t;

typedef enum
{
    NBCoap_NONE = 0x30,
    NBCoap_CONNECTNET,
    NBCoap_END
}NBCoap_t;

typedef enum
{
    NBProcess_NONE = 0x40,
    NBProcess_TCPGETCSQ,
    NBProcess_TCPSENDDATA,
    NBProcess_COAPSENDDATA,
    NBProcess_END
}NBProcess_t;

typedef struct
{
	uint8_t messsageId;
	uint8_t res1[3];
	uint32_t currentTime;
	float batteryVoltage;
	float temperatureValue;
	float humidityValue;
	float lightIntensity1;
	float lightIntensity2;
	uint8_t waterLevel;	
	int8_t signalStrength;
	uint8_t res2[2];
}DeviceData_t;

typedef struct
{
	uint8_t messsageId;
	uint8_t res1[3];
	uint32_t currentTime;
	float temperatureValue;
	float humidityValue;
	float lightIntensity1;
	float lightIntensity2;
	uint8_t waterLevel;	
	int8_t signalStrength;
	uint8_t res2[2];
}DeviceDataReport_t;

#define REQUIRE_noErr( ERR, LABEL, ACTION ) \
    do                                      \
    {                                       \
        if ( ERR )                          \
        {                                   \
            { ACTION; }                     \
            goto LABEL;                     \
        }                                   \
    } while ( 0 )                           


void NBBCxx_init(void);
int NB_Connect(void);		
int NB_DataSend(char* pdata,uint8_t len);	
void NB_Close(void);

#endif /* __NB_PROCESS_H */

