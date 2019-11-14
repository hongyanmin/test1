#ifndef _rtc_H
#define _rtc_H

#include "stm32l1xx_rtc.h"
#include "stm32l1xx_pwr.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_exti.h"
#include "misc.h"

#define DATA_PROCESS_IDLE										0x00//空闲，低功耗模式
#define	DATA_CALL_START											0x01//召测开始
#define	DATA_REPORT_START										0x02//主动上报开始
#define	DATA_CALL_OVER											0x04//召测完成
#define	DATA_EXTERNAL											0x08//外部中断
#define DATA_RECEIVE_UP											0x10//接收到NB模块发的设置查询命令

typedef struct time
{
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
}sys_time;


void RTC_Configuration(void);
void RTC_Time_Set(void);
void RTC_Alarm_Configuration(void);
uint32_t iso_date(uint8_t *p);
#endif

