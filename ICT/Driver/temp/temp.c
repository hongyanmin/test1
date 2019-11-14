/*
 *******************************************************************************
 *                                  INCLUDE                                  
 *******************************************************************************
 */
#include "temp.h"
/*
 *******************************************************************************
 *                                  TYPEDEFS                                  
 *******************************************************************************
 */ 
struct _temp
{
  void *parameter;
  
  void  (* init)(void *parameter);
  float (* read)(void *parameter);
  
  float temp;
  float humi;
};
typedef struct _temp _temp_t;

struct _temp_sht10
{
  sht10_device_t sht10;
  
  GPIO_TypeDef*  data_port;
  GPIO_TypeDef*  sck_port;
  uint16_t       data_pin;
  uint16_t       sck_pin;
};
typedef struct _temp_sht10 _temp_sht10_t;


/*
 *******************************************************************************
 *                              LOCAL FUNCTIONS                                  
 *******************************************************************************
 */
/* sht10 interface */ 
static void _sht10_data_pin_set(void *parameter, uint8_t b);
static void _sht10_sck_pin_set(void *parameter, uint8_t b);
static uint8_t _sht10_data_pin_get(void *parameter);
static void _sht10_data_pin_set_out(void *parameter);
static void _sht10_data_pin_set_in(void *parameter);
static void _temp_sht10_init(void *parameter);
static float _temp_sht10_read(void *parameter);


/*
 *******************************************************************************
 *                              LOCAL VARIABLES                                 
 *******************************************************************************
 */
//static _temp_sht10_t _temp_sht10_list[] = 
//{
//  {   
//    .sht10.parameter    = &_temp_sht10_list[0],
//    .sht10.data_pin_set = _sht10_data_pin_set,
//    .sht10.sck_pin_set  = _sht10_sck_pin_set,
//    .sht10.data_pin_get = _sht10_data_pin_get,
//    .sht10.data_pin_set_out = _sht10_data_pin_set_out,
//    .sht10.data_pin_set_in = _sht10_data_pin_set_in,
//    .sht10.delay_ms     = delay_ms,
//    .sht10.delay_us     = delay_us,
//      
//    .data_port = TEMP_DAT_PORT,
//    .sck_port  = TEMP_CLK_PORT,
//    .data_pin  = TEMP_DAT_PIN,
//    .sck_pin   = TEMP_CLK_PIN,
//  }   
//};

static _temp_sht10_t _temp_sht10_list[] = 
{
    &_temp_sht10_list[0],
    _sht10_data_pin_set,
    _sht10_sck_pin_set,
    _sht10_data_pin_get,
    _sht10_data_pin_set_out,
    _sht10_data_pin_set_in,
    delay_ms,
    delay_us,
      
    TEMP_DAT_PORT,
    TEMP_CLK_PORT,
    TEMP_DAT_PIN,
		TEMP_CLK_PIN,
};
//static _temp_t _temp_list[] = 
//{
//  /* sht10 */
//  {
//    .parameter = &_temp_sht10_list[0],
//    .init = _temp_sht10_init,
//    .read = _temp_sht10_read,
//  },
//};

static _temp_t _temp_list[] = 
{
  /* sht10 */
    &_temp_sht10_list[0],
    _temp_sht10_init,
    _temp_sht10_read,
};
/*
 *******************************************************************************
 *                                 CONSTANTS                                     
 *******************************************************************************
 */
static const int _temp_number = sizeof(_temp_list) / sizeof(_temp_t);


/*
 *******************************************************************************
 *                                  MACROS                                  
 *******************************************************************************
 */
#define _TEMP_TO_OBJECT(temp)	&_temp_list[(temp)]


/*
 *******************************************************************************
 *                                 FUNCTIONS                                 
 *******************************************************************************
 */
void sht10_temp_init(void)
{
  int i;
  _temp_t *temp;
  
  
  for (i = 0; i < _temp_number; i++)
  {
    temp = &_temp_list[i];
    
    temp->init(temp->parameter);
  }
}


float sht10_temp_read(hw_temp_t temp)
{
  _temp_t *_temp = _TEMP_TO_OBJECT(temp);
  
  return _temp->read(_temp->parameter);
}


/*
 *******************************************************************************
 *                              LOCAL FUNCTIONS                                  
 *******************************************************************************
 */
/* sht10 interface */ 
static void _sht10_data_pin_set(void *parameter, uint8_t b)
{
  _temp_sht10_t *sht10 = (_temp_sht10_t *)parameter;  
  GPIO_WriteBit(sht10->data_port, sht10->data_pin, (BitAction)b);
}


static void _sht10_sck_pin_set(void *parameter, uint8_t b)
{
  _temp_sht10_t *sht10 = (_temp_sht10_t *)parameter;  
  GPIO_WriteBit(sht10->sck_port, sht10->sck_pin, (BitAction)b);
}


static uint8_t _sht10_data_pin_get(void *parameter)
{
  _temp_sht10_t *sht10 = (_temp_sht10_t *)parameter; 
  return (GPIO_ReadInputDataBit(sht10->data_port, sht10->data_pin) > 0);
}


static void _sht10_data_pin_set_out(void *parameter)
{

}


static void _sht10_data_pin_set_in(void *parameter)
{

}


static void _temp_sht10_init(void *parameter)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  _temp_sht10_t *sht10 = (_temp_sht10_t *)parameter;
 
  RCC_AHBPeriphClockCmd(TEMP_DAT_CLK|TEMP_CLK_CLK, ENABLE);
   
	GPIO_InitStructure.GPIO_Pin = TEMP_DAT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;  
	GPIO_Init(TEMP_DAT_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TEMP_CLK_PIN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(TEMP_CLK_PORT, &GPIO_InitStructure);
}


static float _temp_sht10_read(void *parameter)
{
  volatile uint32_t temperature_data;
  volatile uint32_t humidity_data;
  float    t, h;
  uint8_t error;
  _temp_sht10_t *sht10 = (_temp_sht10_t *)parameter;
  
  
  sht10_connection_reset(&sht10->sht10);
  
  temperature_data = sht10_measure(&sht10->sht10, TEMP, &error);
  //if (error != 0) continue;
  humidity_data    = sht10_measure(&sht10->sht10, HUMI, &error);
  //if (error != 0) continue;
  
  t = (float)temperature_data;
  h = (float)humidity_data;
  
  sht10_calc(&h, &t);
  return h;
}
