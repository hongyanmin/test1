/*
 *******************************************************************************
 *                                  INCLUDE                                  
 *******************************************************************************
 */
#include "sht10.h"


/*
 *******************************************************************************
 *                                 CONSTANTS                                     
 *******************************************************************************
 */
#define NOACK        0
#define ACK          1

#define STATUS_REG_W 0x06  //000   0011   0
#define STATUS_REG_R 0x07  //000   0011   1
#define MEASURE_TEMP 0x03  //000   0001   1
#define MEASURE_HUMI 0x05  //000   0010   1
#define RESET        0x1E  //000   1111   0




/*
 *******************************************************************************
 *                                 FUNCTIONS                                 
 *******************************************************************************
 */
void sht10_init(sht10_device_t *device)
{
  
}


/*
 *******************************************************************************
 *                              LOCAL FUNCTIONS                                  
 *******************************************************************************
 */
/*
 *******************************************************************************
  @brief    Command sequence, to initiate a transmission a "Transmission Start" sequence
            has to be issued. It consists of a lowering of the DATA line
            while SCK is high, followed by a low pulse on SCK and
            raising DATA again while SCK is still high.
             
  @params   NONE
 
  @return   NONE
 *******************************************************************************
 */
void sht10_strsmission_start(sht10_device_t *device)
{
  device->data_pin_set(device->parameter, 1);
  device->sck_pin_set(device->parameter, 0); 
  
  device->delay_us(2);   
  device->sck_pin_set(device->parameter, 1);
  device->delay_us(2);  
  device->data_pin_set(device->parameter, 0);
  device->delay_us(2);  
  device->sck_pin_set(device->parameter, 0);  
  device->delay_us(2);  
  device->sck_pin_set(device->parameter, 1);
  device->delay_us(2);  
  device->data_pin_set(device->parameter, 1);   
  device->delay_us(2);  
  device->sck_pin_set(device->parameter, 0);   
}


/*
 *******************************************************************************
  @brief    Connection reset sequence, if communication with the SHT1x is lost 
            the following signal sequence will reset its serial interface:
            While leaving DATA high toggle SCK 9 or more times. This must be 
            followed by a "Transmission Start" sequence preceding the next command.
 
  @params   none
 
  @return   none
 *******************************************************************************
 */
void sht10_connection_reset(sht10_device_t *device)
{
  int i;
  
  
  device->data_pin_set(device->parameter, 1);  
  device->sck_pin_set(device->parameter, 0); 
  
  device->delay_us(2);
  
  for (i = 0; i < 9; i++) 
  { 
    device->sck_pin_set(device->parameter, 1);
    device->delay_us(2);  
    device->sck_pin_set(device->parameter, 0);   
    device->delay_us(2);
  }
}


/*
 *******************************************************************************
  @brief    Send to SHT10 a byte data
 
  @params   value - the data will be send
 
  @return   0 - SUCCESS, 1 - FAILD
 *******************************************************************************
 */
static int sht10_write_byte(sht10_device_t *device, uint8_t value)
{
  int32_t    i;
  int      error;
  
  
  device->data_pin_set_out(device->parameter);

  for (i = 0x80; i > 0; i >>= 1) 
  {  
    device->delay_us(2);
    
    if (i & value) 
      device->data_pin_set(device->parameter, 1); 
    else 
      device->data_pin_set(device->parameter, 0);   
    
    device->delay_us(2);
    device->sck_pin_set(device->parameter, 1); 
    device->delay_us(2);
    device->sck_pin_set(device->parameter, 0);
  }
  
  device->delay_us(4); 
  device->data_pin_set(device->parameter, 1); 
  
  device->data_pin_set_in(device->parameter); 
  device->sck_pin_set(device->parameter, 1);   
  device->delay_us(2);  
  
  error = device->data_pin_get(device->parameter);
  device->sck_pin_set(device->parameter, 0);
  
  return error; 
}


/*
 *******************************************************************************
  @brief    receive a byte data from SHT10
 
  @params   ack - NOACK, ACK
 
  @return   the data from SHT10
 *******************************************************************************
 */
static uint8_t sht10_read_byte(sht10_device_t *device, uint8_t ack)
{  
  uint8_t i, value = 0;
  
  
  device->data_pin_set(device->parameter, 1); 
  device->data_pin_set_in(device->parameter); 
  
  for (i = 0x80; i > 0; i >>= 1)  
  {   
    device->delay_us(2);  
    device->sck_pin_set(device->parameter, 1); 
    device->delay_us(2);  
    
    if (device->data_pin_get(device->parameter)) 
      value |= i; 
    
    device->sck_pin_set(device->parameter, 0);   
  }
  
  device->delay_us(2);  
  
  device->data_pin_set_out(device->parameter);
  
  if (ack) 
    device->data_pin_set(device->parameter, 0);
  else 
    device->data_pin_set(device->parameter, 1);
  
  device->sck_pin_set(device->parameter, 1);
  device->delay_us(2);   
  device->sck_pin_set(device->parameter, 0);   
  device->data_pin_set(device->parameter, 1);
  
  return value;
}


/*
 *******************************************************************************
  @brief    SHT10 sensor start measure temperature or humidity and return the 
            result as interger.
 
  @params   mode - TEMP, HUMI
 
  @return   result of temperature or humidity.
 *******************************************************************************
 */
int sht10_measure(sht10_device_t *device, uint8_t mode, uint8_t *perr)
{
  uint8_t  error = 0;
  uint32_t i;
  uint8_t  a, b;
  
  
  sht10_strsmission_start(device); 
  
  switch (mode)
  { 
  case TEMP: 
    error += sht10_write_byte(device, MEASURE_TEMP); 
    break;
    
  case HUMI: 
    error += sht10_write_byte(device, MEASURE_HUMI); 
    break;
    
  default : 
    break;  
  }
  
  device->data_pin_set(device->parameter, 1); 
  device->data_pin_set_in(device->parameter);  
  
  for (i = 0; i < 300; i++)
  {
    device->delay_ms(10);
    
    /* wait until sensor has finished the measurement */
    if (device->data_pin_get(device->parameter) == 0 ) 
      break;
  }   
  
  /* wait timeout */
  if( device->data_pin_get(device->parameter) ) 
    error += 1; 
  
  if (perr != NULL)
    *perr = error;
  
  if (error != 0)
    return 0;
  
  /* ��ȡ�����ֽڵ����� */
  a = sht10_read_byte(device, ACK);
  b = sht10_read_byte(device, ACK);

  /* read checksum */
  sht10_read_byte(device, NOACK); 
  
  return (((int)a) << 8) + b;
}


void sht10_calc(float *ph, float *pt)
{
  //  const float C1 = -4.0;              /* 12λʪ�Ⱦ��� ������ʽ */
  //  const float C2 = +0.0405;           /* 12λʪ�Ⱦ��� ������ʽ */
  //  const float C3 = -0.0000028;        /* 12λʪ�Ⱦ��� ������ʽ */
  const float C1 = -2.0468;
  const float C2 = +0.0367;
  const float C3 = -0.0000015955;
  const float T1 = +0.01;                /* 14λ�¶Ⱦ��� 5V����  ������ʽ */
  const float T2 = +0.00008;             /* 14λ�¶Ⱦ��� 5V����  ������ʽ */
  
  float rh = *ph;                        /* rh:      12λ ʪ��     */ 
  float t  = *pt;                        /* t:       14λ �¶�     */
  float rh_lin;                          /* rh_lin:  ʪ�� linearֵ */
  float rh_true;                         /* rh_true: ʪ�� tureֵ   */
  float t_C;                             /* t_C:     �¶� ��       */
  
  t_C = t * 0.01 - 39.6;                            /* �����¶�                   */
  rh_lin = C3 * rh * rh + C2 * rh + C1;             /* ���ʪ�ȷ����Բ���         */
  //  rh_true = (t_C-25)*(T1+T2*rh)+rh_lin;         /* ���ʪ�ȶ����¶������Բ��� */
  rh_true = (t_C - 25) * (T1 + T2 * rh) + rh_lin;   /* ���ʪ�ȶ����¶������Բ��� */
  if (rh_true > 100) rh_true = 100;                 /* ʪ���������               */
  if (rh_true < 0.1) rh_true = 0.1;                 /* ʪ����С����               */
  
  *pt = t_C;                  /* �����¶Ƚ�� */
  *ph = rh_true;              /* ����ʪ�Ƚ�� */
}
