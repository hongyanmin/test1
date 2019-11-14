#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#include "stm32l1xx.h"


/*------------------------宏定义---------------------*/

#define FLASH_SAVE_ADDR         						0X08080000//数据保存地址

#define MAX_SENSOR_NUM											15//总传感器个数

#define WTM_SENSOR_NUM											5//温度传感器个数，前5个
#define WVS_SENSOR_NUM											5//位移传感器个数，中间5个
#define FEC_SENSOR_NUM											5//灭活防爆盒个数，后5个

#define NRF_DATA_LEN												32//NRF数据长度

#define FRAME_HEAD                          0xFFFF//报文头
#define SENSOR_TYPE_ICT                     0x00//终端类型
#define SENSOR_TYPE_WTM                     0x01//温度传感器类型
#define SENSOR_TYPE_WVS                     0x02//位移传感器类型
#define SENSOR_TYPE_FEC											0x03//灭活防爆盒类型

//召测数据
#define MESSAGE_ID_ICT_DATA                 0x01
#define MESSAGE_ID_WTM_DATA                 0x02
#define MESSAGE_ID_WVS_DATA                 0x03
#define MESSAGE_ID_FEC_DATA                 0x04

//主动上报数据
#define MESSAGE_ID_ICT_REPORT_DATA          0x11
#define MESSAGE_ID_WTM_REPORT_DATA          0x12
#define MESSAGE_ID_WVS_REPORT_DATA          0x13
#define MESSAGE_ID_FEC_REPORT_DATA          0x14

//设置命令
#define MESSAGE_ID_ICT_THRESHOLD_SET_DOWN   0x21
#define MESSAGE_ID_ICT_THRESHOLD_SET_UP     0x31
#define MESSAGE_ID_WTM_THRESHOLD_SET_DOWN   0x22
#define MESSAGE_ID_WTM_THRESHOLD_SET_UP     0x32
#define MESSAGE_ID_WVS_THRESHOLD_SET_DOWN   0x23
#define MESSAGE_ID_WVS_THRESHOLD_SET_UP     0x33
#define MESSAGE_ID_FEC_THRESHOLD_SET_DOWN   0x24
#define MESSAGE_ID_FEC_THRESHOLD_SET_UP     0x34
#define MESSAGE_ID_FREQ_SET_DOWN            0x25
#define MESSAGE_ID_FREQ_SET_UP              0x35
#define MESSAGE_ID_SENSORID_SET_DOWN        0x26
#define MESSAGE_ID_SENSORID_SET_UP          0x36
#define MESSAGE_ID_TIME_SET_DOWN            0x27

//查询命令
#define MESSAGE_ID_ICT_THRESHOLD_QUERY_DOWN 0x41    
#define MESSAGE_ID_ICT_THRESHOLD_QUERY_UP   0x51  
#define MESSAGE_ID_WTM_THRESHOLD_QUERY_DOWN 0x42    
#define MESSAGE_ID_WTM_THRESHOLD_QUERY_UP   0x52  
#define MESSAGE_ID_WVS_THRESHOLD_QUERY_DOWN 0x43   
#define MESSAGE_ID_WVS_THRESHOLD_QUERY_UP   0x53  
#define MESSAGE_ID_FEC_THRESHOLD_QUERY_DOWN 0x44   
#define MESSAGE_ID_FEC_THRESHOLD_QUERY_UP   0x54
#define MESSAGE_ID_FREQ_QUERY_DOWN          0x45
#define MESSAGE_ID_FREQ_QUERY_UP            0x55
#define MESSAGE_ID_VERSION_QUERY_DOWN       0x46
#define MESSAGE_ID_VERSION_QUERY_UP         0x56

//远程升级
#define MESSAGE_ID_UPGRADE_DOWN             0x61

/*------------------------配置信息------------------------*/
typedef struct ICT_DataSave
{
	uint8_t sensorType;
	uint8_t res1[3];
	uint32_t sensorId;
	float tempature;
  float humidity;
  float lightIntensity1;
  float lightIntensity2;	
}ICT_DataSave_t;

typedef struct WTM_DataSave
{
	uint8_t sensorType;
	uint8_t res1[3];
	uint32_t sensorId;
	float tempature;
}WTM_DataSave_t;

typedef struct WVS_DataSave
{
	uint8_t sensorType;
	uint8_t res1[3];
	uint32_t sensorId;
	int16_t acceleration_x;
  int16_t acceleration_y;
  int16_t acceleration_z;
	uint8_t res2[2];
}WVS_DataSave_t;

typedef struct FEC_DataSave
{
	uint8_t sensorType;
	uint8_t res1[3];
	uint32_t sensorId;
	float tempature;
}FEC_DataSave_t;

typedef struct DataSave
{
	ICT_DataSave_t ICT_Data_Save;
	WTM_DataSave_t WTM_Data_Save[WTM_SENSOR_NUM];
	WVS_DataSave_t WVS_Data_Save[WVS_SENSOR_NUM];
	FEC_DataSave_t FEC_Data_Save[FEC_SENSOR_NUM];
  uint16_t ICT_Frequency;
  uint16_t WTM_Frequency;
  uint16_t WVS_Frequency;
  uint16_t FEC_Frequency;
}DataSave_t;

/*------------------------终端和主站通信---------------------*/
//召测数据
//终端本身数据
typedef struct ICT_CallData
{
  uint8_t messageId;
  uint8_t res1[3];
  uint32_t time;
  int32_t batteryVoltage;
  int32_t tempature;
  int32_t humidity;
  int32_t lightIntensity1;
  int32_t lightIntensity2;
  uint8_t waterLevel;
  int8_t signalStrength;
  uint8_t res2[2];
}ICT_CallData_t;

typedef struct WTM_Data
{
	uint32_t sensorId;
	int32_t batteryVoltage;
	int32_t tempature;
}WTM_Data_t;

//温度传感器数据
typedef struct WTM_CallData
{
  uint8_t messageId;
  uint8_t res1[3];
  uint32_t time;
  WTM_Data_t sensorData[WTM_SENSOR_NUM];
}WTM_CallData_t;

typedef struct WVS_Data
{
	uint32_t sensorId;
	int32_t batteryVoltage;
	int16_t acceleration_x;
 	int16_t acceleration_y;
  int16_t acceleration_z;
	uint8_t res[2];
}WVS_Data_t;

//位移传感器数据
typedef struct WVS_CallData
{
  uint8_t messageId;
  uint8_t res1[3];
  uint32_t time;
  WVS_Data_t sensorData[WVS_SENSOR_NUM];
}WVS_CallData_t;


typedef struct FEC_Data
{
	uint32_t sensorId;
	int32_t batteryVoltage;
	int32_t tempature;
	uint8_t fireStatus;
	uint8_t res[3];
}FEC_Data_t;

//灭火防爆盒数据
typedef struct FEC_CallData
{
  uint8_t messageId;
  uint8_t res1[3];
  uint32_t time;
  FEC_Data_t sensorData[FEC_SENSOR_NUM];
}FEC_CallData_t;



//主动上报数据
//终端数据主动上报
typedef struct ICT_ReportData
{
  uint8_t messageId;
  uint8_t res1[3];
  uint32_t time;
  int32_t tempature;
  int32_t humidity;
  int32_t lightIntensity1;
  int32_t lightIntensity2;
  uint8_t waterLevel;
  int8_t signalStrength;
  uint8_t res2[2];
}ICT_ReportData_t;


//温度传感器数据主动上报
typedef struct WTM_ReportData
{
  uint8_t messageId;
  uint8_t res1[3];
  uint32_t time;
  uint32_t sensorId;
  int32_t tempature;
}WTM_ReportData_t;

//位移传感器数据主动上报
typedef struct WVS_ReportData
{
  uint8_t messageId;
  uint8_t res1[3];
  uint32_t time;
  uint32_t sensorId;
  int16_t acceleration_x;
  int16_t acceleration_y;
  int16_t acceleration_z;
  uint8_t res2[2];
}WVS_ReportData_t;

//灭火防爆盒数据主动上报
typedef struct FEC_ReportData
{
  uint8_t messageId;
  uint8_t res1[3];
  uint32_t time;
  uint32_t sensorId;
  int32_t tempature;
  uint8_t fireStatus;
  uint8_t res2[3];
}FEC_ReportData_t;

//设置命令
//终端阈值设置下行报文
typedef struct ICT_ThresholdSetDown
{
  uint8_t messageId;
//  uint16_t mid;
//	  uint8_t res1;
  uint8_t mid_res[3];
  int32_t tempature;
  int32_t humidity;
  int32_t lightIntensity1;
  int32_t lightIntensity2;
}ICT_ThresholdSetDown_t;

//终端阈值设置上行报文
typedef struct ICT_ThresholdSetUp
{
  uint8_t messageId;
//  uint16_t mid;
//  uint8_t errcode;
	uint8_t mid_errcode[3];
  uint8_t setFlag;
  uint8_t res2[3];
}ICT_ThresholdSetUp_t;

typedef struct WTM_SetData
{
	uint32_t sensorId;
	int32_t tempatureThreshold;
}WTM_SetData_t;

//温度传感器阈值设置下行报文
typedef struct WTM_ThresholdSetDown
{
  uint8_t messageId;

//  uint16_t mid;
//  uint8_t res1;
	uint8_t mid_res[3];	
  WTM_SetData_t sensorSetData[WTM_SENSOR_NUM];
}WTM_ThresholdSetDown_t;

typedef struct SetDataUp
{
	uint32_t sensorId;
	uint8_t setFlag;
	uint8_t res[3];
}SetDataUp_t;

//温度传感器阈值设置上行报文
typedef struct WTM_ThresholdSetUp
{
  uint8_t messageId;
//  uint16_t mid;
//  uint8_t errcode;
	uint8_t mid_errcode[3];	
  SetDataUp_t sensorSetDataUp[WTM_SENSOR_NUM];
}WTM_ThresholdSetUp_t;


typedef struct WVS_SetData
{
  uint32_t sensorId;
  int16_t acceleration_x;
  int16_t acceleration_y;
  int16_t acceleration_z;
  uint8_t res2[2];
}WVS_SetData_t;

//位移传感器阈值设置下行报文
typedef struct WVS_ThresholdSetDown
{
  uint8_t messageId;
//  uint16_t mid;
//  uint8_t res1;
	uint8_t mid_res[3];	
  WVS_SetData_t sensorSetData[WVS_SENSOR_NUM];
}WVS_ThresholdSetDown_t;


//位移传感器阈值设置上行报文
typedef struct WVS_ThresholdSetUp
{
  uint8_t messageId;
//  uint16_t mid;
//  uint8_t errcode;
	uint8_t mid_errcode[3];	
  SetDataUp_t sensorSetDataUp[WVS_SENSOR_NUM];
}WVS_ThresholdSetUp_t;

typedef struct FEC_SetData
{
  uint32_t sensorId;
  int32_t tempature;
}FEC_SetData_t;

//灭火防爆盒阈值设置下行报文
typedef struct FEC_ThresholdSetDown
{
  uint8_t messageId;
//  uint16_t mid;
//  uint8_t res1;
	uint8_t mid_res[3];	
  FEC_SetData_t sensorSetData[FEC_SENSOR_NUM];
}FEC_ThresholdSetDown_t;



//灭火防爆盒阈值设置上行报文
typedef struct FEC_ThresholdSetUp
{
  uint8_t messageId;
//  uint16_t mid;
//  uint8_t errcode;
	uint8_t mid_errcode[3];
  SetDataUp_t sensorSetDataUp[FEC_SENSOR_NUM];
}FEC_ThresholdSetUp_t;


//数据上传频率设置下行报文
typedef struct FreqSetDown
{
  uint8_t messageId;
//  uint16_t mid;
//  uint8_t res;
	uint8_t mid_res[3];		
  int16_t ICT_Frequency;
  int16_t WTM_Frequency;
  int16_t WVS_Frequency;
  int16_t FEC_Frequency;
}FreqSetDown_t;

//数据上传频率设置上行报文
typedef struct FreqSetUp
{
  uint8_t messageId;
//  uint16_t mid;  
//  uint8_t errcode;
	uint8_t mid_errcode[3];	
  uint8_t set_flag;
  uint8_t res1[3];
}FreqSetUp_t;

//时间同步下行报文
typedef struct TimeSetDown
{  
  uint8_t messageId;
  uint8_t year;
  uint8_t month;
  uint8_t date;
  uint8_t week;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
}TimeSetDown_t;

//查询命令
//查询下行报文
typedef struct QueryDown
{
  uint8_t messageId;
//  uint16_t mid;  
//  uint8_t res1;
	uint8_t mid_res[3];		
}QueryDown_t;

//终端阈值查询上行报文
typedef struct ICT_ThresholdQueryUp
{  
  uint8_t messageId;
//  uint16_t mid;
//  uint8_t errcode;
	uint8_t mid_errcode[3];	
  int32_t tempature;
  int32_t humidity;
  int32_t lightIntensity1;
  int32_t lightIntensity2;
}ICT_ThresholdQueryUp_t;


//温度传感器阈值查询上行报文
typedef struct WTM_ThresholdQueryUp
{  
  uint8_t messageId;
//  uint16_t mid;
//  uint8_t errcode;
	uint8_t mid_errcode[3];	
  WTM_SetData_t sensorSetData[WTM_SENSOR_NUM];
}WTM_ThresholdQueryUp_t;


//位移传感器阈值查询上行
typedef struct WVS_ThresholdQueryUp
{
  uint8_t messageId;
//  uint16_t mid;
//  uint8_t errcode;
	uint8_t mid_errcode[3];		
  WVS_SetData_t sensorSetData[WVS_SENSOR_NUM];
}WVS_ThresholdQueryUp_t;

//灭火防爆盒阈值查询上行
typedef struct FEC_ThresholdQueryUp
{
  uint8_t messageId;
//  uint16_t mid;
//  uint8_t errcode;
	uint8_t mid_errcode[3];	
  FEC_SetData_t sensorSetData[FEC_SENSOR_NUM];
}FEC_ThresholdQueryUp_t;

//数据上传频率查询上行报文
typedef struct FreqQueryUp
{
  uint8_t messageId;
//  uint16_t mid;
//  uint8_t errcode;
	uint8_t mid_errcode[3];		
  uint16_t ICT_Frequency;
  uint16_t WTM_Frequency;
  uint16_t WVS_Frequency;
  uint16_t FEC_Frequency;
}FreqQueryUp_t;

//程序版本号查询上行报文
typedef struct SoftwareVersion
{
  uint8_t messageId;
//  uint16_t mid;
//  uint8_t errcode;
	uint8_t mid_errcode[3];	
  uint8_t version[8];
  uint8_t time[8];
}SoftwareVersion_t;

//远程升级下行报文
typedef struct SoftwareUpgrade
{
  uint8_t messageId;
//  uint16_t mid;
//  uint8_t res;
	uint8_t mid_res[3];	
}SoftwareUpgrade_t;

/*------------------------终端和传感器通信---------------------*/
//数据召测
//温度传感器数据
typedef struct WTM_CallDataIn
{
	uint16_t frameHead;
  uint8_t messageId;
  uint8_t res1;
	uint32_t sensorId;
	float batteryVoltage;
	float tempature;
}WTM_CallDataIn_t;


//位移传感器数据
typedef struct WVS_CallDataIn
{
	uint16_t frameHead;
  uint8_t messageId;
  uint8_t res1;
  uint32_t sensorId;
	float batteryVoltage;
	int16_t acceleration_x;
 	int16_t acceleration_y;
  int16_t acceleration_z;
	uint8_t res2[2];
}WVS_CallDataIn_t;

//灭火防爆盒数据
typedef struct FEC_CallDataIn
{
	uint16_t frameHead;
  uint8_t messageId;
  uint8_t res1;
	uint32_t sensorId;
	float batteryVoltage;
	float tempature;
	uint8_t fireStatus;
	uint8_t res2[3];
}FEC_CallDataIn_t;

//温度传感器数据主动上报
typedef struct WTM_ReportDataIn
{
	uint16_t frameHead;
  uint8_t messageId;
  uint8_t res1;
	uint32_t time;
	uint32_t sensorId;
	float tempature;
}WTM_ReportDataIn_t;

//位移传感器数据主动上报
typedef struct WVS_ReportDataIn
{
  uint16_t frameHead;
  uint8_t messageId;
  uint8_t res1;
  uint32_t time;
  uint32_t sensorId;
  int16_t acceleration_x;
  int16_t acceleration_y;
  int16_t acceleration_z;
  uint8_t res2[2];
}WVS_ReportDataIn_t;

//灭火防爆盒数据主动上报
typedef struct FEC_ReportDataIn
{
	uint16_t frameHead;
  uint8_t messageId;
  uint8_t res1;
	uint32_t time;
	uint32_t sensorId;
	float tempature;
	uint8_t fireStatus;
	uint8_t res2[3];
}FEC_ReportDataIn_t;

//数据设置
//温度传感器阈值设置下行报文
typedef struct WTM_ThresholdSetDownIn
{
  uint8_t messageId;
  uint8_t res1[3];
  uint32_t sensorId;
	float tempature;
}WTM_ThresholdSetDownIn_t;

//位移传感器阈值设置下行报文
typedef struct WVS_ThresholdSetDownIn
{
  uint8_t messageId;
  uint8_t res1[3];
  uint32_t sensorId;
	int16_t acceleration_x;
 	int16_t acceleration_y;
  int16_t acceleration_z;
	uint8_t res2[2];
}WVS_ThresholdSetDownIn_t;

//灭火防爆盒阈值设置下行报文
typedef struct FEC_ThresholdSetDownIn
{
  uint8_t messageId;
  uint8_t res1[3];
  uint32_t sensorId;
	float tempature;
}FEC_ThresholdSetDownIn_t;

//阈值设置上行报文
typedef struct Sensor_ThresholdSetUpIn
{
	uint16_t frameHead;
  uint8_t messageId;
  uint8_t res1;
	uint32_t sensorId;
	uint8_t setFlag;
	uint8_t res2[3];
}Sensor_ThresholdSetUpIn_t;

//传感器编号设置下行报文
typedef struct SensorIdSetDown
{
  uint8_t messageId;
  uint8_t sensorType;
  uint8_t res1[2];
  uint32_t SensorIdOld;
  uint32_t SensorIdNew;
}SensorIdSetDown_t;

//传感器编号设置上行报文
typedef struct SensorIdSetUp
{
  uint16_t frameHead;  
  uint8_t messageId;
  uint8_t res1;
  uint32_t SensorId;
  uint8_t set_flag;
  uint8_t res2[3];
}SensorIdSetUp_t;

//阈值查询
//阈值查询下行报文
typedef struct Sensor_ThresholdQueryDownIn
{
  uint8_t messageId;
  uint8_t res1[3];
	uint32_t sensorId;
}Sensor_ThresholdQueryDownIn_t;

//温度传感器阈值查询上行报文
typedef struct WTM_ThresholdQueryUpIn
{
	uint16_t frameHead;
  uint8_t messageId;
  uint8_t res1;
  uint32_t sensorId;
	float tempature;
}WTM_ThresholdQueryUpIn_t;

//位移传感器阈值查询上行报文
typedef struct WVS_ThresholdQueryUpIn
{
	uint16_t frameHead;
  uint8_t messageId;
  uint8_t res1;
  uint32_t sensorId;
	int16_t acceleration_x;
 	int16_t acceleration_y;
  int16_t acceleration_z;
	uint8_t res2[2];
}WVS_ThresholdQueryUpIn_t;

//灭火防爆盒阈值查询上行报文
typedef struct FEC_ThresholdQueryUpIn
{
	uint16_t frameHead;
  uint8_t messageId;
  uint8_t res1;
  uint32_t sensorId;
	float tempature;
}FEC_ThresholdQueryUpIn_t;

//大小端转换
typedef union FLOAT_CONV
{
    float f;
    char c[4];
}float_conv;
/*----------------------------函数声明-------------------------*/
void Config_Info_Save(void);
void Config_Info_Read(void);
void Config_Init(void);
void ICT_Data_Call(void);
void WTM_Data_Call(void);
void WVS_Data_Call(void);
void FEC_Data_Call(void);
void Data_Call(void);
void ICT_Data_Report(void);
void Sensor_Data_Report(void);
void Data_Report(void);
void ICT_Threshold_Set(uint8_t* pbuf,uint16_t len);
void WTM_Threshold_Set(uint8_t* pbuf,uint16_t len);
void WVS_Threshold_Set(uint8_t* pbuf,uint16_t len);
void FEC_Threshold_Set(uint8_t* pbuf,uint16_t len);
void Freq_Set(uint8_t* pbuf,uint16_t len);
void Time_Set(uint8_t* pbuf,uint16_t len);
void ICT_Threshold_Query(uint8_t* pbuf,uint16_t len);
void WTM_Threshold_Query(uint8_t* pbuf,uint16_t len);
void WVS_Threshold_Query(uint8_t* pbuf,uint16_t len);
void FEC_Threshold_Query(uint8_t* pbuf,uint16_t len);
void Freq_Query(uint8_t* pbuf,uint16_t len);
void Software_Version_Query(uint8_t* pbuf,uint16_t len);
void Data_Set_Query(void);
void Sensor_ID_Set(uint8_t sensorType,uint32_t sensorID);
void Data_Process(void);//数据处理
uint32_t BLEndianUint32(uint32_t value);
int32_t BLEndianInt32(int32_t value);
int16_t BLEndianInt16(int16_t value);
float BLEndianFloat(float value);

#endif

