#ifndef __SHT10_H__
#define __SHT10_H__
#ifdef __cplusplus
extern "C" {
#endif
/*
 *******************************************************************************
 *                                  INCLUDE                                  
 *******************************************************************************
 */
#include "stm32l1xx.h"
#include <string.h>
#include "general_type.h"
/*
 *******************************************************************************
 *                                 CONSTANTS                                     
 *******************************************************************************
 */
#define TEMP         0
#define HUMI         1
   
   
/*
 *******************************************************************************
 *                                  TYPEDEFS                                  
 *******************************************************************************
 */
struct sht10_device
{
  /* user parameter */
  void *parameter;
  
  void    (* data_pin_set)(void *parameter, uint8_t b);
  void    (* sck_pin_set)(void *parameter, uint8_t b);
  
  uint8_t (* data_pin_get)(void *parameter);
  
  void    (* data_pin_set_out)(void *parameter);
  void    (* data_pin_set_in)(void *parameter);
  
  void (* delay_ms)(uint16_t ms);
  void (* delay_us)(u32 us);
};
typedef struct sht10_device sht10_device_t;


/*
 *******************************************************************************
 *                                 FUNCTIONS                                 
 *******************************************************************************
 */
void sht10_init(sht10_device_t *device);

void sht10_connection_reset(sht10_device_t *device);

int sht10_measure(sht10_device_t *device, uint8_t mode, uint8_t *perr);

void sht10_calc(float *ph, float *pt);

#ifdef __cplusplus
}
#endif
#endif
