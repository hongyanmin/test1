#ifndef __TEMP_H__
#define __TEMP_H__
#ifdef __cplusplus
extern "C" {
#endif
/*
 *******************************************************************************
 *                                  INCLUDE                                  
 *******************************************************************************
 */
#include "stm32l1xx.h"
#include "sht10.h"
#include "delay.h"
#include <stdio.h>
#include <string.h>
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_rcc.h"

#define TEMP_DAT_PORT		GPIOA
#define TEMP_DAT_PIN		GPIO_Pin_11
#define TEMP_DAT_CLK		RCC_AHBPeriph_GPIOA

#define TEMP_CLK_PORT		GPIOD
#define TEMP_CLK_PIN		GPIO_Pin_2
#define TEMP_CLK_CLK		RCC_AHBPeriph_GPIOD
/*
 *******************************************************************************
 *                                  TYPEDEFS                                  
 *******************************************************************************
 */
typedef enum
{
  HW_TEMP_1, 
  __HW_TEMP_END__,
}hw_temp_t;
  
/*
 *******************************************************************************
 *                                 FUNCTIONS                                 
 *******************************************************************************
 */

void sht10_temp_init(void);
float sht10_temp_read(hw_temp_t temp);

#ifdef __cplusplus
}
#endif  
#endif

