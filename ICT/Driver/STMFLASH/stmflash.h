#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "sys.h"  
#include "stm32l1xx_flash.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////
//用户根据自己的需要设置
#define STM32_EEPROM_SIZE 2	 		//所选STM32的EEPROM容量大小(单位为K)
#define STM32_EEPROM_WREN 1     //使能EEPROM写入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH起始地址
#define STM32_EEPROM_BASE 0x08080000 	//STM32 EEPROM的起始地址

u16 STMFLASH_ReadHalfWord(u32 faddr);		  //读出半字  
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//从指定地址开始写入指定长度的数据
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//从指定地址开始读出指定长度的数据

//测试写入
void Test_Write(u32 WriteAddr,u16 WriteData);								   
#endif

















