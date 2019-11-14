/*--------------ͨ��Э��----------------------*/
#include "protocol.h"
#include "string.h"
#include "adc.h"
#include "rtc.h"
#include "ds18b20.h"
#include "nb_process.h"
#include "temp.h"
#include "stmflash.h"
#include "24l01.h"
#include "timer.h"
#include "stm32l1xx_tim.h"
#include "exti.h"
#include "show.h"
#include "key.h"
#include "menu.h"


int8_t key_num = 0;


uint8_t data_process_flag = 0;//���ݴ����־λ
uint8_t data_report_flag = 0;//�ն��쳣�ϴ���־λ��Ϊ1��ʾ�Ѿ��ϴ�����ֹ�ظ�����

extern sys_time sys_time_t;//ʱ��ת��
extern RTC_TimeTypeDef RTC_time_current;//��ǰʱ��
extern RTC_DateTypeDef RTC_date_current;//��ǰ����

extern uint8_t timer2_flag;//��ʱ��2��־λ��Ϊ1��ʾ��ʱ�䣬60S��ʱ�������ٲ⣬15min�ٲ�һ��
extern uint8_t timer10_flag;//��ʱ��10��־λ��Ϊ1��ʾ��ʱ�䣬20s��ʱ�����������ϱ���1min���һ�������ϱ�

uint8_t nrf_status = 0; //�����жϽ���/����״̬
uint8_t txbuf[NRF_DATA_LEN];	 //���ͻ���
uint8_t rxbuf[NRF_DATA_LEN];	 //���ջ���

extern int8_t nb_csq;//�ź�ǿ��

DataSave_t Data_Save;//���������

//uint8_t save_query_buf[512][10];//�������ò�ѯ���һ���ٲ����������10��

extern uint8_t	NB_RxBuf[512];//NB�յ������ò�ѯ�����
extern uint16_t	NB_RxLen;//NB�յ������ò�ѯ�����

extern uint8_t strToHex( char *inBuf, char *outBuf, uint32_t *outLen );

//������Ϣ����
void Config_Info_Save(void)
{
	STMFLASH_Write(FLASH_SAVE_ADDR,(uint16_t*)&Data_Save,sizeof(Data_Save)/2);
}
	
//������Ϣ��ȡ
void Config_Info_Read(void)
{
	STMFLASH_Read(FLASH_SAVE_ADDR,(uint16_t*)&Data_Save,sizeof(Data_Save)/2);
}

//������Ϣ��ʼ��
void Config_Init(void)
{
	uint8_t i,j = 0;
	Data_Save.ICT_Data_Save.sensorType = SENSOR_TYPE_ICT;
	Data_Save.ICT_Data_Save.sensorId = 0;
	Data_Save.ICT_Data_Save.tempature = 0xFFFFFFFF;//test//50;
	Data_Save.ICT_Data_Save.humidity = 0;
	Data_Save.ICT_Data_Save.lightIntensity1 = 0.1;
	Data_Save.ICT_Data_Save.lightIntensity2 = 0.1;
	
	for(i = 0;i < WTM_SENSOR_NUM;i++)
	{
		Data_Save.WTM_Data_Save[i].sensorType = SENSOR_TYPE_WTM;
		Data_Save.WTM_Data_Save[i].sensorId = ++j;
		Data_Save.WTM_Data_Save[i].tempature = 50;
	}
//	Data_Save.WTM_Data_Save[0].sensorId = 1;//test
	for(i = 0;i < WVS_SENSOR_NUM;i++)
	{
		Data_Save.WVS_Data_Save[i].sensorType = SENSOR_TYPE_WVS;
		Data_Save.WVS_Data_Save[i].sensorId = ++j;
		Data_Save.WVS_Data_Save[i].acceleration_x = 100;
		Data_Save.WVS_Data_Save[i].acceleration_y = 100;
		Data_Save.WVS_Data_Save[i].acceleration_z = 100;
	}
//	Data_Save.WVS_Data_Save[0].sensorId = 6;//test
	for(i = 0;i < FEC_SENSOR_NUM;i++)
	{
		Data_Save.FEC_Data_Save[i].sensorType = SENSOR_TYPE_FEC;
		Data_Save.FEC_Data_Save[i].sensorId = 0;
		Data_Save.FEC_Data_Save[i].tempature = 50;
	}
	
	Data_Save.ICT_Frequency = 15;
	
	Data_Save.WTM_Frequency = 15;
	Data_Save.WVS_Frequency = 15;
	Data_Save.FEC_Frequency = 15;
	
	Config_Info_Save();
}

//�ն������ٲ�
void ICT_Data_Call(void)
{
		float batteryVoltage,tempature,lightIntensity1,lightIntensity2;
		ICT_CallData_t ICT_Call_Data;
	
		RTC_GetTime(RTC_Format_BIN,&RTC_time_current);//��ȡ��ǰʱ��
		RTC_GetDate(RTC_Format_BIN,&RTC_date_current);//��ȡ��ǰ����
	
		sys_time_t.year = RTC_date_current.RTC_Year;
		sys_time_t.month = RTC_date_current.RTC_Month;
		sys_time_t.day = RTC_date_current.RTC_Date;
		sys_time_t.hour = RTC_time_current.RTC_Hours;
		sys_time_t.minute = RTC_time_current.RTC_Minutes;
		sys_time_t.second = RTC_time_current.RTC_Seconds;
		
		Adc_Key_On();//ADC������ش�
		
		memset(&ICT_Call_Data,0,sizeof(ICT_CallData_t));
		ICT_Call_Data.messageId = MESSAGE_ID_ICT_DATA;
		ICT_Call_Data.time = BLEndianInt32(iso_date((uint8_t*)&sys_time_t));
		batteryVoltage = (float)Get_Adc_Average(0,10)/4096*(float)2.5*2.0;
		ICT_Call_Data.batteryVoltage = BLEndianInt32((int32_t)(batteryVoltage*100.0));
		tempature = DS18B20_Get_Temp();
		ICT_Call_Data.tempature = BLEndianInt32(tempature);
		ICT_Call_Data.humidity = 0;//sht10_temp_read(HW_TEMP_1);
		lightIntensity1 = (float)Get_Adc_Average(1,10)/4096*(float)2.5/batteryVoltage;
		lightIntensity2 = (float)Get_Adc_Average(2,10)/4096*(float)2.5/batteryVoltage;
		ICT_Call_Data.lightIntensity1 = BLEndianInt32((int32_t)(lightIntensity1*10000.0));
		ICT_Call_Data.lightIntensity2 = BLEndianInt32((int32_t)(lightIntensity2*10000.0));
		ICT_Call_Data.waterLevel = 0;
		ICT_Call_Data.signalStrength = nb_csq;
		
		Adc_Key_Off();//ADC������عر�
		
		NB_DataSend((char *)&ICT_Call_Data,sizeof(ICT_CallData_t));
}

//�¶ȴ����������ٲ�
void WTM_Data_Call(void)
{
	WTM_CallData_t WTM_Call_Data;	
	uint8_t i = 0,resend_count = 0;
	uint16_t delay_count = 0,send_count = 0;
	WTM_CallDataIn_t WTM_Call_DataIn;
	
	Config_Info_Read();
	
	memset(&WTM_Call_Data,0,sizeof(WTM_CallData_t));
	WTM_Call_Data.messageId = MESSAGE_ID_WTM_DATA;
	WTM_Call_Data.time = BLEndianInt32(iso_date((uint8_t*)&sys_time_t));
		
	while(i<WTM_SENSOR_NUM)
	{
		if(Data_Save.WTM_Data_Save[i].sensorId == 0) 
		{
			i++;
			continue;
		}
		memset(&WTM_Call_DataIn,0,sizeof(WTM_CallDataIn_t));
		WTM_Call_Data.sensorData[i].sensorId = BLEndianInt32(Data_Save.WTM_Data_Save[i].sensorId);
		
		memset(txbuf,0,NRF_DATA_LEN);
		memset(rxbuf,0,NRF_DATA_LEN);
		txbuf[0] = MESSAGE_ID_WTM_DATA;
		memcpy(&txbuf[4],&Data_Save.WTM_Data_Save[i].sensorId,4);

		NRF_TX_Mode();//����ģʽ          
		nrf_status = NRF_Tx_Dat(txbuf);//��������

		if((send_count < 10) && (nrf_status != TX_DS))//���Ͳ��ɹ������·���
		{
			send_count++;
			delay_ms(1);
			continue;
		}
		send_count = 0;
		
		NRF_RX_Mode();//����ģʽ	
		nrf_status = NRF_Rx_Dat(rxbuf);// �ȴ��������� 
		
		while(delay_count < 1000)//1sû�лظ���ʧ��
		{	
			if(nrf_status == RX_DR)//�жϽ���״̬  
			{
				if((rxbuf[0] == 0xFF) && (rxbuf[1] == 0xFF) && (rxbuf[2] == MESSAGE_ID_WTM_DATA))
				{
					memcpy(&WTM_Call_DataIn,rxbuf,sizeof(WTM_CallDataIn_t));
					if(WTM_Call_DataIn.sensorId == Data_Save.WTM_Data_Save[i].sensorId)
					{
						WTM_Call_Data.sensorData[i].batteryVoltage = BLEndianInt32((int32_t)(WTM_Call_DataIn.batteryVoltage*100.0));
						WTM_Call_Data.sensorData[i].tempature = BLEndianInt32((int32_t)(WTM_Call_DataIn.tempature*10.0));
						resend_count = 0;	
						break;
					}
				}		
			}
			delay_ms(1);
			nrf_status = NRF_Rx_Dat(rxbuf);// �ȴ��������� 
			delay_count++;
		}
		delay_count = 0;
		if(WTM_Call_DataIn.batteryVoltage == 0)//û���յ��������·���
		{
			resend_count++;
		}
		if((WTM_Call_DataIn.batteryVoltage != 0) || (resend_count > 3))//����ط�����
		{
			resend_count = 0;	
			i++;
		}
	}

	NB_DataSend((char *)&WTM_Call_Data,sizeof(WTM_CallData_t));
}

//λ�ƴ����������ٲ�
void WVS_Data_Call(void)
{
	WVS_CallData_t WVS_Call_Data;	
	uint8_t i = 0,resend_count = 0;
	uint16_t delay_count = 0,send_count = 0;
	WVS_CallDataIn_t WVS_Call_DataIn;
	
	Config_Info_Read();
	
	memset(&WVS_Call_Data,0,sizeof(WVS_CallData_t));
	WVS_Call_Data.messageId = MESSAGE_ID_WVS_DATA;
	WVS_Call_Data.time = BLEndianInt32(iso_date((uint8_t*)&sys_time_t));
	
	while(i<WVS_SENSOR_NUM)
	{
		if(Data_Save.WVS_Data_Save[i].sensorId == 0) 
		{
			i++;
			continue;
		}
		memset(&WVS_Call_DataIn,0,sizeof(WVS_CallDataIn_t));
		WVS_Call_Data.sensorData[i].sensorId = BLEndianInt32(Data_Save.WVS_Data_Save[i].sensorId);
		
		memset(txbuf,0,NRF_DATA_LEN);
		memset(rxbuf,0,NRF_DATA_LEN);
		txbuf[0] = MESSAGE_ID_WVS_DATA;
		memcpy(&txbuf[4],&Data_Save.WVS_Data_Save[i].sensorId,4);

		NRF_TX_Mode();//����ģʽ          
		nrf_status = NRF_Tx_Dat(txbuf);//��������
	
		if((send_count < 10) && (nrf_status != TX_DS))//���Ͳ��ɹ������·���
		{
			send_count++;
			delay_ms(1);
			continue;
		}
		send_count = 0;
		
		NRF_RX_Mode();//����ģʽ	
		nrf_status = NRF_Rx_Dat(rxbuf);// �ȴ��������� 
		
		while(delay_count < 1000)//1sû�лظ���ʧ��
		{	
			if(nrf_status == RX_DR)//�жϽ���״̬  
			{
				if((rxbuf[0] == 0xFF) && (rxbuf[1] == 0xFF) && (rxbuf[2] == MESSAGE_ID_WVS_DATA))
				{
					memcpy(&WVS_Call_DataIn,rxbuf,sizeof(WVS_CallDataIn_t));
					if(WVS_Call_DataIn.sensorId == Data_Save.WVS_Data_Save[i].sensorId)
					{
						WVS_Call_Data.sensorData[i].batteryVoltage = BLEndianInt32((int32_t)(WVS_Call_DataIn.batteryVoltage*100.0));
						WVS_Call_Data.sensorData[i].acceleration_x = BLEndianInt16(WVS_Call_DataIn.acceleration_x);
						WVS_Call_Data.sensorData[i].acceleration_y = BLEndianInt16(WVS_Call_DataIn.acceleration_y);
						WVS_Call_Data.sensorData[i].acceleration_z = BLEndianInt16(WVS_Call_DataIn.acceleration_z);
						resend_count = 0;	
						break;
					}
				}		
			}
			delay_ms(1);
			nrf_status = NRF_Rx_Dat(rxbuf);// �ȴ��������� 
			delay_count++;
		}
		delay_count = 0;	
		if(WVS_Call_DataIn.batteryVoltage == 0)//û���յ��������·���
		{
			resend_count++;
		}
		if((WVS_Call_DataIn.batteryVoltage != 0) || (resend_count > 3))//����ط�����
		{
			resend_count = 0;
			i++;
		}		
	}
	NB_DataSend((char *)&WVS_Call_Data,sizeof(WVS_CallData_t));
}

//�������������ٲ�
void FEC_Data_Call(void)
{
	FEC_CallData_t FEC_Call_Data;	
	uint8_t i = 0,resend_count = 0;
	uint16_t delay_count = 0,send_count = 0;
	FEC_CallDataIn_t FEC_Call_DataIn;
	
	Config_Info_Read();
	
	memset(&FEC_Call_Data,0,sizeof(FEC_CallData_t));
	FEC_Call_Data.messageId = MESSAGE_ID_FEC_DATA;
	FEC_Call_Data.time = BLEndianInt32(iso_date((uint8_t*)&sys_time_t));
	
	while(i<FEC_SENSOR_NUM)
	{
		if(Data_Save.FEC_Data_Save[i].sensorId == 0) 
		{
			i++;
			continue;
		}
		memset(&FEC_Call_DataIn,0,sizeof(FEC_CallDataIn_t));
		FEC_Call_Data.sensorData[i].sensorId = BLEndianInt32(Data_Save.FEC_Data_Save[i].sensorId);		
		
		memset(txbuf,0,NRF_DATA_LEN);
		memset(rxbuf,0,NRF_DATA_LEN);
		txbuf[0] = MESSAGE_ID_FEC_DATA;
		memcpy(&txbuf[4],&Data_Save.FEC_Data_Save[i].sensorId,4);

		NRF_TX_Mode();//����ģʽ          
		nrf_status = NRF_Tx_Dat(txbuf);//��������

		if((send_count < 10) && (nrf_status != TX_DS))//���Ͳ��ɹ������·���
		{
			send_count++;
			delay_ms(1);
			continue;
		}
		send_count = 0;
		
		NRF_RX_Mode();//����ģʽ	
		nrf_status = NRF_Rx_Dat(rxbuf);// �ȴ��������� 

		while(delay_count < 1000)//1sû�лظ���ʧ��
		{	
			if(nrf_status == RX_DR)//�жϽ���״̬  
			{
				if((rxbuf[0] == 0xFF) && (rxbuf[1] == 0xFF) && (rxbuf[2] == MESSAGE_ID_FEC_DATA))
				{
					memcpy(&FEC_Call_DataIn,rxbuf,sizeof(FEC_CallDataIn_t));
					if(FEC_Call_DataIn.sensorId == Data_Save.FEC_Data_Save[i].sensorId)
					{
						FEC_Call_Data.sensorData[i].batteryVoltage = BLEndianInt32((int32_t)(FEC_Call_DataIn.batteryVoltage*100.0));
						FEC_Call_Data.sensorData[i].tempature = BLEndianInt32((int32_t)(FEC_Call_DataIn.tempature*10.0));	
						FEC_Call_Data.sensorData[i].fireStatus = FEC_Call_DataIn.fireStatus;
						resend_count = 0;	
						break;
					}
				}		
			}
			delay_ms(1);
			nrf_status = NRF_Rx_Dat(rxbuf);// �ȴ��������� 
			delay_count++;
		}
		delay_count = 0;
		if(FEC_Call_DataIn.batteryVoltage == 0)//û���յ��������·���
		{
			resend_count++;
		}
		if((FEC_Call_DataIn.batteryVoltage != 0) || (resend_count > 3))//����ط�����
		{
			resend_count = 0;	
			i++;
		}	
	}

	NB_DataSend((char *)&FEC_Call_Data,sizeof(FEC_CallData_t));
	Config_Info_Read();//ȫ�ֱ�����ֵ��ı�
}

//�����ٲ�
void Data_Call(void)
{
//	NB_Connect();		
	ICT_Data_Call();
	WTM_Data_Call();	
	WVS_Data_Call();
	FEC_Data_Call();
	NB_Close();
}

//�ն����������ϱ�
void ICT_Data_Report(void)
{
	ICT_ReportData_t ICT_Report_Data;
	float tempature,humidity,lightIntensity1,lightIntensity2,batteryVoltage;
	
	if(data_report_flag == 1) return;//����Ѿ��ϴ����˳�
	
	tempature = DS18B20_Get_Temp();	
	humidity = 0;//sht10_temp_read(HW_TEMP_1);//Ԥ��
	
	Adc_Key_On();//ADC������ش�
	batteryVoltage = (float)Get_Adc_Average(0,10)/4096*(float)2.5*2.0;
	lightIntensity1 = (float)Get_Adc_Average(1,10)/4096*(float)2.5/batteryVoltage;
	lightIntensity2 = (float)Get_Adc_Average(2,10)/4096*(float)2.5/batteryVoltage;
	Adc_Key_Off();//ADC������عر�
	
	//������쳣
	if((tempature > Data_Save.ICT_Data_Save.tempature) || (lightIntensity1 < Data_Save.ICT_Data_Save.lightIntensity1) || (lightIntensity2 < Data_Save.ICT_Data_Save.lightIntensity2) || (humidity > Data_Save.ICT_Data_Save.humidity))
	{
		RTC_GetTime(RTC_Format_BIN,&RTC_time_current);//��ȡ��ǰʱ��
		RTC_GetDate(RTC_Format_BIN,&RTC_date_current);//��ȡ��ǰ����
		
		sys_time_t.year = RTC_date_current.RTC_Year;
		sys_time_t.month = RTC_date_current.RTC_Month;
		sys_time_t.day = RTC_date_current.RTC_Date;
		sys_time_t.hour = RTC_time_current.RTC_Hours;
		sys_time_t.minute = RTC_time_current.RTC_Minutes;
		sys_time_t.second = RTC_time_current.RTC_Seconds;
		
		ICT_Report_Data.messageId = MESSAGE_ID_ICT_REPORT_DATA;
		ICT_Report_Data.time = BLEndianInt32(iso_date((uint8_t*)&sys_time_t));
		ICT_Report_Data.tempature = BLEndianInt32(tempature);		
		ICT_Report_Data.humidity = BLEndianInt32((int32_t)(humidity*100.0));
		ICT_Report_Data.lightIntensity1 = BLEndianInt32((int32_t)(lightIntensity1*10000.0));
		ICT_Report_Data.lightIntensity2 = BLEndianInt32((int32_t)(lightIntensity2*10000.0));
		ICT_Report_Data.waterLevel = 0;
		ICT_Report_Data.signalStrength = nb_csq;
		
		//NB_Connect();	
		NB_DataSend((char *)&ICT_Report_Data,sizeof(ICT_ReportData_t));
		NB_Close();
		data_report_flag = 1;
		Config_Info_Read();
	}
}

//���������������ϱ�
void Sensor_Data_Report(void)
{
	WTM_ReportData_t WTM_Report_Data;
	WVS_ReportData_t WVS_Report_Data;
	FEC_ReportData_t FEC_Report_Data;
	WTM_ReportDataIn_t WTM_Report_DataIn;
	WVS_ReportDataIn_t WVS_Report_DataIn;
	FEC_ReportDataIn_t FEC_Report_DataIn;
	
	uint8_t i,id_flag = 0;
		
	memset(rxbuf,0,NRF_DATA_LEN);
	NRF_RX_Mode();//����ģʽ	
	nrf_status = NRF_Rx_Dat(rxbuf);// �ȴ��������� 
			
	if(nrf_status == RX_DR)//�жϽ���״̬  
	{		
		if((rxbuf[8] == 0) && (rxbuf[9] == 0) && (rxbuf[10] == 0) && (rxbuf[11] == 0)) return;//������IDΪ0���ϱ�
		//������ID��ȷ
		for(i = 0;i< WTM_SENSOR_NUM;i++)
		{
			if(strncmp((char*)&rxbuf[8],(char*)&Data_Save.WTM_Data_Save[i].sensorId,4) == 0)
			{
				id_flag = 1;
				break;
			}
		}
		for(i = 0;i< WVS_SENSOR_NUM;i++)
		{
			if(strncmp((char*)&rxbuf[8],(char*)&Data_Save.WVS_Data_Save[i].sensorId,4) == 0)
			{
				id_flag = 1;
				break;
			}
		}
		for(i = 0;i< FEC_SENSOR_NUM;i++)
		{
			if(strncmp((char*)&rxbuf[8],(char*)&Data_Save.FEC_Data_Save[i].sensorId,4) == 0)
			{
				id_flag = 1;
				break;
			}
		}	
			
		//����ͷ��messageId��ȷ
		if((id_flag == 1)&&(rxbuf[0] == 0xFF) &&(rxbuf[1] == 0xFF))//����ͷ
		{
			if(rxbuf[2] == MESSAGE_ID_WTM_REPORT_DATA)//messageId
			{
				memcpy(&WTM_Report_DataIn,rxbuf,sizeof(WTM_ReportDataIn_t));
				memset(&WTM_Report_Data,0,sizeof(WTM_ReportData_t));		
				WTM_Report_Data.messageId = MESSAGE_ID_WTM_REPORT_DATA;		
				WTM_Report_Data.time = BLEndianInt32(WTM_Report_DataIn.time);
				WTM_Report_Data.sensorId = BLEndianInt32(WTM_Report_DataIn.sensorId);
				WTM_Report_Data.tempature = BLEndianInt32((int32_t)(WTM_Report_DataIn.tempature*10.0));
				//NB_Connect();	
				NB_DataSend((char *)&WTM_Report_Data,sizeof(WTM_ReportData_t));
				Config_Info_Read();
				NB_Close();

			}
			else if(rxbuf[2] == MESSAGE_ID_WVS_REPORT_DATA)
			{
				memcpy(&WVS_Report_DataIn,rxbuf,sizeof(WVS_ReportDataIn_t));
				memset(&WVS_Report_Data,0,sizeof(WVS_ReportData_t));
				WVS_Report_Data.messageId = MESSAGE_ID_WVS_REPORT_DATA;
				
				RTC_GetTime(RTC_Format_BIN,&RTC_time_current);//��ȡ��ǰʱ��
				RTC_GetDate(RTC_Format_BIN,&RTC_date_current);//��ȡ��ǰ����
				sys_time_t.year = RTC_date_current.RTC_Year;
				sys_time_t.month = RTC_date_current.RTC_Month;
				sys_time_t.day = RTC_date_current.RTC_Date;
				sys_time_t.hour = RTC_time_current.RTC_Hours;
				sys_time_t.minute = RTC_time_current.RTC_Minutes;
				sys_time_t.second = RTC_time_current.RTC_Seconds;
				WVS_Report_Data.time = BLEndianInt32(iso_date((uint8_t*)&sys_time_t));//λ�ƴ�����flash��������ICT��ȡʱ��
				WVS_Report_Data.sensorId = BLEndianInt32(WVS_Report_DataIn.sensorId);
				WVS_Report_Data.acceleration_x = BLEndianInt16(WVS_Report_DataIn.acceleration_x);
				WVS_Report_Data.acceleration_y = BLEndianInt16(WVS_Report_DataIn.acceleration_y);
				WVS_Report_Data.acceleration_z = BLEndianInt16(WVS_Report_DataIn.acceleration_z);
				//NB_Connect();	
				NB_DataSend((char *)&WVS_Report_Data,sizeof(WVS_ReportData_t));
				Config_Info_Read();
				NB_Close();
			}
			else if(rxbuf[2] == MESSAGE_ID_FEC_REPORT_DATA)
			{
				memcpy(&FEC_Report_DataIn,rxbuf,sizeof(FEC_ReportDataIn_t));
				memset(&FEC_Report_Data,0,sizeof(FEC_ReportData_t));				
				FEC_Report_Data.messageId = MESSAGE_ID_FEC_REPORT_DATA;
				FEC_Report_Data.time = BLEndianInt32(FEC_Report_DataIn.time);
				FEC_Report_Data.sensorId = BLEndianInt32(FEC_Report_DataIn.sensorId); 
				FEC_Report_Data.tempature = BLEndianInt32((int32_t)(FEC_Report_DataIn.tempature*10.0));
				//NB_Connect();	
				NB_DataSend((char *)&FEC_Report_Data,sizeof(FEC_ReportData_t));
				NB_Close();
				Config_Info_Read();
			}
		}	
	}	
}

//���������ϱ�
void Data_Report(void)
{
		delay_ms(100);//100ms���һ��
		ICT_Data_Report();			
		Sensor_Data_Report();		
}

//�ն���ֵ����
void ICT_Threshold_Set(uint8_t* pbuf,uint16_t len)
{
	ICT_ThresholdSetDown_t ICT_Threshold_SetDown;
	ICT_ThresholdSetUp_t ICT_Threshold_SetUp;
	
	memset(&ICT_Threshold_SetUp,0,sizeof(ICT_ThresholdSetUp_t));
	memcpy(&ICT_Threshold_SetDown,pbuf,len);
	
//	Config_Info_Read();
	Data_Save.ICT_Data_Save.tempature = (float)BLEndianInt32(ICT_Threshold_SetDown.tempature)/10.0;
	Data_Save.ICT_Data_Save.humidity = (float)BLEndianInt32(ICT_Threshold_SetDown.humidity)/100.0;
	Data_Save.ICT_Data_Save.lightIntensity1 = (float)BLEndianInt32(ICT_Threshold_SetDown.lightIntensity1)/10000.0;
	Data_Save.ICT_Data_Save.lightIntensity2 = (float)BLEndianInt32(ICT_Threshold_SetDown.lightIntensity2)/10000.0;
	Config_Info_Save();
	
	ICT_Threshold_SetUp.messageId = MESSAGE_ID_ICT_THRESHOLD_SET_UP;
	ICT_Threshold_SetUp.mid_errcode[0] = ICT_Threshold_SetDown.mid_res[0];
	ICT_Threshold_SetUp.mid_errcode[1] = ICT_Threshold_SetDown.mid_res[1];
	ICT_Threshold_SetUp.mid_errcode[2] = 0;
	ICT_Threshold_SetUp.setFlag = 0;
	NB_DataSend((char*)&ICT_Threshold_SetUp,sizeof(ICT_ThresholdSetUp_t));
}


//�¶ȴ�������ֵ����
void WTM_Threshold_Set(uint8_t* pbuf,uint16_t len)
{
	uint8_t i;
	uint16_t delay_count = 0;
	WTM_ThresholdSetDown_t WTM_Threshold_SetDown;
	WTM_ThresholdSetUp_t WTM_Threshold_SetUp;
	WTM_ThresholdSetDownIn_t WTM_Threshold_SetDownIn;
	Sensor_ThresholdSetUpIn_t Sensor_Threshold_SetUpIn;
	
	memset(&WTM_Threshold_SetUp,0,sizeof(WTM_ThresholdSetUp_t));
	memcpy(&WTM_Threshold_SetDown,pbuf,len);	
	Config_Info_Read();
	
	for(i = 0;i< WTM_SENSOR_NUM;i++)
	{
		if(Data_Save.WTM_Data_Save[i].sensorId != 0)
		{
			if(WTM_Threshold_SetDown.sensorSetData[0].sensorId == 0xFFFF)//���д���������Ϊͳһֵ
			{
				Data_Save.WTM_Data_Save[i].tempature = (float)BLEndianInt32(WTM_Threshold_SetDown.sensorSetData[0].tempatureThreshold)/10.0;
				WTM_Threshold_SetDownIn.messageId = MESSAGE_ID_WTM_THRESHOLD_SET_DOWN;
				WTM_Threshold_SetDownIn.tempature = BLEndianInt32(WTM_Threshold_SetDown.sensorSetData[i].tempatureThreshold);
			}	
			else if(BLEndianInt32(WTM_Threshold_SetDown.sensorSetData[i].sensorId) == Data_Save.WTM_Data_Save[i].sensorId)
			{
				Data_Save.WTM_Data_Save[i].sensorId = BLEndianInt32(WTM_Threshold_SetDown.sensorSetData[i].sensorId);
				Data_Save.WTM_Data_Save[i].tempature = (float)BLEndianInt32(WTM_Threshold_SetDown.sensorSetData[i].tempatureThreshold)/10.0;
					
				WTM_Threshold_SetDownIn.messageId = MESSAGE_ID_WTM_THRESHOLD_SET_DOWN;
				WTM_Threshold_SetDownIn.sensorId = BLEndianInt32(WTM_Threshold_SetDown.sensorSetData[i].sensorId);
				WTM_Threshold_SetDownIn.tempature = (float)BLEndianInt32(WTM_Threshold_SetDown.sensorSetData[i].tempatureThreshold)/10.0;
			}
			if((WTM_Threshold_SetDown.sensorSetData[0].sensorId == 0xFFFF) || (BLEndianInt32(WTM_Threshold_SetDown.sensorSetData[i].sensorId) == Data_Save.WTM_Data_Save[i].sensorId))
			{
				memset(txbuf,0,NRF_DATA_LEN);
				memcpy(txbuf,&WTM_Threshold_SetDownIn,sizeof(WTM_ThresholdSetDownIn_t));

				NRF_TX_Mode();//����ģʽ          
				nrf_status = NRF_Tx_Dat(txbuf);//��������
		
				NRF_RX_Mode();//����ģʽ	
				nrf_status = NRF_Rx_Dat(rxbuf);// �ȴ��������� 
		
				while(nrf_status != RX_DR)//�жϽ���״̬  
				{
					delay_ms(1);
					nrf_status = NRF_Rx_Dat(rxbuf);// �ȴ��������� 
					delay_count++;
					if(delay_count > 1000)//1sû�лظ���ʧ��
						break;
				}	
				delay_count = 0;
				memcpy(&Sensor_Threshold_SetUpIn,rxbuf,sizeof(Sensor_ThresholdSetUpIn_t));
		
				if((Sensor_Threshold_SetUpIn.frameHead == FRAME_HEAD) && (Sensor_Threshold_SetUpIn.sensorId == WTM_Threshold_SetDownIn.sensorId))
				{				
					WTM_Threshold_SetUp.sensorSetDataUp[i].sensorId = BLEndianInt32(Sensor_Threshold_SetUpIn.sensorId);
					WTM_Threshold_SetUp.sensorSetDataUp[i].setFlag = Sensor_Threshold_SetUpIn.setFlag;
				}											
			}
		}	
	}
	Config_Info_Save();
	
	WTM_Threshold_SetUp.messageId = MESSAGE_ID_WTM_THRESHOLD_SET_UP;
	WTM_Threshold_SetUp.mid_errcode[0] = WTM_Threshold_SetDown.mid_res[0];
	WTM_Threshold_SetUp.mid_errcode[1] = WTM_Threshold_SetDown.mid_res[1];	
	WTM_Threshold_SetUp.mid_errcode[2]  = 0;

	NB_DataSend((char*)&WTM_Threshold_SetUp,sizeof(WTM_ThresholdSetUp_t));
}

//λ�ƴ�������ֵ����
void WVS_Threshold_Set(uint8_t* pbuf,uint16_t len)
{
	uint8_t i;
	uint16_t delay_count = 0;
	WVS_ThresholdSetDown_t WVS_Threshold_SetDown;
	WVS_ThresholdSetUp_t WVS_Threshold_SetUp;
	WVS_ThresholdSetDownIn_t WVS_Threshold_SetDownIn;
	Sensor_ThresholdSetUpIn_t Sensor_Threshold_SetUpIn;
	
	memset(&WVS_Threshold_SetUp,0,sizeof(WVS_ThresholdSetUp_t));
	memcpy(&WVS_Threshold_SetDown,pbuf,len);	
	Config_Info_Read();
	
	for(i = 0;i< WVS_SENSOR_NUM;i++)
	{
		if(Data_Save.WVS_Data_Save[i].sensorId != 0)
		{
			if(WVS_Threshold_SetDown.sensorSetData[0].sensorId == 0xFFFF)//���д���������Ϊͳһֵ
			{
				Data_Save.WVS_Data_Save[i].acceleration_x = BLEndianInt16(WVS_Threshold_SetDown.sensorSetData[0].acceleration_x);
				Data_Save.WVS_Data_Save[i].acceleration_y = BLEndianInt16(WVS_Threshold_SetDown.sensorSetData[0].acceleration_y);
				Data_Save.WVS_Data_Save[i].acceleration_z = BLEndianInt16(WVS_Threshold_SetDown.sensorSetData[0].acceleration_z);
				
				WVS_Threshold_SetDownIn.messageId = MESSAGE_ID_WTM_THRESHOLD_SET_DOWN;
				WVS_Threshold_SetDownIn.acceleration_x = BLEndianInt16(WVS_Threshold_SetDown.sensorSetData[0].acceleration_x);
				WVS_Threshold_SetDownIn.acceleration_y = BLEndianInt16(WVS_Threshold_SetDown.sensorSetData[0].acceleration_y);
				WVS_Threshold_SetDownIn.acceleration_z = BLEndianInt16(WVS_Threshold_SetDown.sensorSetData[0].acceleration_z);
			}	
			else if(BLEndianInt32(WVS_Threshold_SetDown.sensorSetData[i].sensorId) == Data_Save.WVS_Data_Save[i].sensorId)
			{
				Data_Save.WVS_Data_Save[i].sensorId = BLEndianInt32(WVS_Threshold_SetDown.sensorSetData[i].sensorId);
				Data_Save.WVS_Data_Save[i].acceleration_x = BLEndianInt16(WVS_Threshold_SetDown.sensorSetData[i].acceleration_x);
				Data_Save.WVS_Data_Save[i].acceleration_y = BLEndianInt16(WVS_Threshold_SetDown.sensorSetData[i].acceleration_y);
				Data_Save.WVS_Data_Save[i].acceleration_z = BLEndianInt16(WVS_Threshold_SetDown.sensorSetData[i].acceleration_z);	
				
				WVS_Threshold_SetDownIn.messageId = MESSAGE_ID_WTM_THRESHOLD_SET_DOWN;
				WVS_Threshold_SetDownIn.sensorId = BLEndianInt32(WVS_Threshold_SetDown.sensorSetData[i].sensorId);
				WVS_Threshold_SetDownIn.acceleration_x = BLEndianInt16(WVS_Threshold_SetDown.sensorSetData[i].acceleration_x);
				WVS_Threshold_SetDownIn.acceleration_y = BLEndianInt16(WVS_Threshold_SetDown.sensorSetData[i].acceleration_y);
				WVS_Threshold_SetDownIn.acceleration_z = BLEndianInt16(WVS_Threshold_SetDown.sensorSetData[i].acceleration_z);
			}
			if((WVS_Threshold_SetDown.sensorSetData[0].sensorId == 0xFFFF) || (BLEndianInt32(WVS_Threshold_SetDown.sensorSetData[i].sensorId) == Data_Save.WVS_Data_Save[i].sensorId))
			{
				memset(txbuf,0,NRF_DATA_LEN);
				memcpy(txbuf,&WVS_Threshold_SetDownIn,sizeof(WVS_ThresholdSetDownIn_t));

				NRF_TX_Mode();//����ģʽ          
				nrf_status = NRF_Tx_Dat(txbuf);//��������
		
				NRF_RX_Mode();//����ģʽ	
				nrf_status = NRF_Rx_Dat(rxbuf);// �ȴ��������� 
		
				while(nrf_status != RX_DR)//�жϽ���״̬  
				{
					delay_ms(1);
					nrf_status = NRF_Rx_Dat(rxbuf);// �ȴ��������� 
					delay_count++;
					if(delay_count > 1000)//1sû�лظ���ʧ��
						break;
				}	
				delay_count = 0;
				memcpy(&Sensor_Threshold_SetUpIn,rxbuf,sizeof(Sensor_ThresholdSetUpIn_t));
		
				if((Sensor_Threshold_SetUpIn.frameHead == FRAME_HEAD) && (Sensor_Threshold_SetUpIn.sensorId == WVS_Threshold_SetDownIn.sensorId))
				{				
					WVS_Threshold_SetUp.sensorSetDataUp[i].sensorId = BLEndianInt32(Sensor_Threshold_SetUpIn.sensorId);
					WVS_Threshold_SetUp.sensorSetDataUp[i].setFlag = Sensor_Threshold_SetUpIn.setFlag;
				}											
			}
		}	
	}
	Config_Info_Save();
	
	WVS_Threshold_SetUp.messageId = MESSAGE_ID_WVS_THRESHOLD_SET_UP;
	WVS_Threshold_SetUp.mid_errcode[0] = WVS_Threshold_SetDown.mid_res[0];
	WVS_Threshold_SetUp.mid_errcode[1] = WVS_Threshold_SetDown.mid_res[0];
	WVS_Threshold_SetUp.mid_errcode[2] = 0;

	NB_DataSend((char*)&WVS_Threshold_SetUp,sizeof(WVS_ThresholdSetUp_t));	
}

//����������ֵ����
void FEC_Threshold_Set(uint8_t* pbuf,uint16_t len)
{
	uint8_t i;
	uint16_t delay_count = 0;
	FEC_ThresholdSetDown_t FEC_Threshold_SetDown;
	FEC_ThresholdSetUp_t FEC_Threshold_SetUp;
	FEC_ThresholdSetDownIn_t FEC_Threshold_SetDownIn;
	Sensor_ThresholdSetUpIn_t Sensor_Threshold_SetUpIn;
	
	memset(&FEC_Threshold_SetUp,0,sizeof(FEC_ThresholdSetUp_t));
	memcpy(&FEC_Threshold_SetDown,pbuf,len);	
	Config_Info_Read();
	
	for(i = 0;i< FEC_SENSOR_NUM;i++)
	{
		if(Data_Save.FEC_Data_Save[i].sensorId != 0)
		{
			if(FEC_Threshold_SetDown.sensorSetData[0].sensorId == 0xFFFF)//���д���������Ϊͳһֵ
			{
				Data_Save.FEC_Data_Save[i].tempature = BLEndianInt32(FEC_Threshold_SetDown.sensorSetData[0].sensorId);
					
				FEC_Threshold_SetDownIn.messageId = MESSAGE_ID_WTM_THRESHOLD_SET_DOWN;
				FEC_Threshold_SetDownIn.tempature = (float)BLEndianInt32(FEC_Threshold_SetDown.sensorSetData[0].tempature)/10.0;
			}
			else if(BLEndianInt32(FEC_Threshold_SetDown.sensorSetData[i].sensorId) == Data_Save.FEC_Data_Save[i].sensorId)
			{
				Data_Save.FEC_Data_Save[i].sensorId = BLEndianInt32(FEC_Threshold_SetDown.sensorSetData[i].sensorId);
				Data_Save.FEC_Data_Save[i].tempature = (float)BLEndianInt32(FEC_Threshold_SetDown.sensorSetData[i].sensorId)/10.0;
					
				FEC_Threshold_SetDownIn.messageId = MESSAGE_ID_WTM_THRESHOLD_SET_DOWN;
				FEC_Threshold_SetDownIn.sensorId = BLEndianInt32(FEC_Threshold_SetDown.sensorSetData[i].sensorId);
				FEC_Threshold_SetDownIn.tempature = (float)BLEndianInt32(FEC_Threshold_SetDown.sensorSetData[i].tempature)/10.0;
			}
			if((FEC_Threshold_SetDown.sensorSetData[0].sensorId == 0xFFFF) || (BLEndianInt32(FEC_Threshold_SetDown.sensorSetData[i].sensorId) == Data_Save.FEC_Data_Save[i].sensorId))
			{
				memset(txbuf,0,NRF_DATA_LEN);
				memcpy(txbuf,&FEC_Threshold_SetDownIn,sizeof(FEC_ThresholdSetDownIn_t));

				NRF_TX_Mode();//����ģʽ          
				nrf_status = NRF_Tx_Dat(txbuf);//��������
		
				NRF_RX_Mode();//����ģʽ	
				nrf_status = NRF_Rx_Dat(rxbuf);// �ȴ��������� 
		
				while(nrf_status != RX_DR)//�жϽ���״̬  
				{
					delay_ms(1);
					nrf_status = NRF_Rx_Dat(rxbuf);// �ȴ��������� 
					delay_count++;
					if(delay_count > 1000)//1sû�лظ���ʧ��
						break;
				}	
				delay_count = 0;
				memcpy(&Sensor_Threshold_SetUpIn,rxbuf,sizeof(Sensor_ThresholdSetUpIn_t));
		
				if((Sensor_Threshold_SetUpIn.frameHead == FRAME_HEAD) && (Sensor_Threshold_SetUpIn.sensorId == FEC_Threshold_SetDownIn.sensorId))
				{				
					FEC_Threshold_SetUp.sensorSetDataUp[i].sensorId = BLEndianInt32(Sensor_Threshold_SetUpIn.sensorId);
					FEC_Threshold_SetUp.sensorSetDataUp[i].setFlag = Sensor_Threshold_SetUpIn.setFlag;
				}											
			}
		}	
	}
	Config_Info_Save();
	
	FEC_Threshold_SetUp.messageId = MESSAGE_ID_FEC_THRESHOLD_SET_UP;
	FEC_Threshold_SetUp.mid_errcode[0] = FEC_Threshold_SetDown.mid_res[0];
	FEC_Threshold_SetUp.mid_errcode[1] = FEC_Threshold_SetDown.mid_res[1];
	FEC_Threshold_SetUp.mid_errcode[1] = 0;

	NB_DataSend((char*)&FEC_Threshold_SetUp,sizeof(FEC_ThresholdSetUp_t));
}

//�����ϴ�Ƶ������
void Freq_Set(uint8_t* pbuf,uint16_t len)
{
	FreqSetDown_t Freq_SetDown;
	FreqSetUp_t Freq_SetUp;
	
	memset(&Freq_SetUp,0,sizeof(FreqSetUp_t));
	memcpy(&Freq_SetDown,pbuf,len);
	
	Config_Info_Read();
	Data_Save.ICT_Frequency = BLEndianInt16(Freq_SetDown.ICT_Frequency);
	Data_Save.WTM_Frequency = BLEndianInt16(Freq_SetDown.WTM_Frequency);
	Data_Save.WVS_Frequency = BLEndianInt16(Freq_SetDown.WVS_Frequency);
	Data_Save.FEC_Frequency = BLEndianInt16(Freq_SetDown.FEC_Frequency);
	Config_Info_Save();
	
	Freq_SetUp.messageId = MESSAGE_ID_ICT_THRESHOLD_SET_UP;
	Freq_SetUp.mid_errcode[0] = Freq_SetDown.mid_res[0];
	Freq_SetUp.mid_errcode[1] = Freq_SetDown.mid_res[1];
	Freq_SetUp.mid_errcode[2] = 0;
	Freq_SetUp.set_flag = 0;
	NB_DataSend((char*)&Freq_SetUp,sizeof(FreqSetUp_t));
}

//ʱ��ͬ��
void Time_Set(uint8_t* pbuf,uint16_t len)
{
	TimeSetDown_t Time_Set_Down;
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;
	
	memcpy(&Time_Set_Down,pbuf,len);
	date.RTC_Year = Time_Set_Down.year;
	date.RTC_Month = Time_Set_Down.month;
	date.RTC_Date = Time_Set_Down.date;
//	date.RTC_WeekDay = Time_Set_Down.week;
	RTC_SetDate(RTC_Format_BIN,&date);
	
	time.RTC_Hours = Time_Set_Down.hour;
	time.RTC_Minutes = Time_Set_Down.minute;
	time.RTC_Seconds = Time_Set_Down.second;
	time.RTC_H12 = RTC_H12_AM;
	RTC_SetTime(RTC_Format_BIN,&time);
	
	memset(txbuf,0,NRF_DATA_LEN);
	memcpy(txbuf,&Time_Set_Down,sizeof(TimeSetDown_t));

	NRF_TX_Mode();//����ģʽ          
	nrf_status = NRF_Tx_Dat(txbuf);//��������	

	RTC_GetTime(RTC_Format_BIN,&RTC_time_current);//��ȡRTCʱ��
	RTC_Alarm_Configuration();//RTC���ӳ�ʼ��
}

//�ն���ֵ��ѯ
void ICT_Threshold_Query(uint8_t* pbuf,uint16_t len)
{
	QueryDown_t Query_Down;
	ICT_ThresholdQueryUp_t ICT_Threshold_QueryUp;
	
	memset(&ICT_Threshold_QueryUp,0,sizeof(ICT_ThresholdQueryUp_t));
	memcpy(&Query_Down,pbuf,len);

	Config_Info_Read();
	ICT_Threshold_QueryUp.messageId = MESSAGE_ID_ICT_THRESHOLD_QUERY_UP;
	ICT_Threshold_QueryUp.mid_errcode[0] = Query_Down.mid_res[0];
	ICT_Threshold_QueryUp.mid_errcode[1] = Query_Down.mid_res[1];
	ICT_Threshold_QueryUp.mid_errcode[2] = 0;
	ICT_Threshold_QueryUp.tempature = BLEndianInt32((int32_t)(Data_Save.ICT_Data_Save.tempature*10.0));
	ICT_Threshold_QueryUp.humidity = BLEndianInt32((int32_t)(Data_Save.ICT_Data_Save.humidity*100.0));
	ICT_Threshold_QueryUp.lightIntensity1 = BLEndianInt32((int32_t)(Data_Save.ICT_Data_Save.lightIntensity1*10000.0));
	ICT_Threshold_QueryUp.lightIntensity2 = BLEndianInt32((int32_t)(Data_Save.ICT_Data_Save.lightIntensity2*10000.0));
	
	NB_DataSend((char*)&ICT_Threshold_QueryUp,sizeof(ICT_ThresholdQueryUp_t));
}

//�¶ȴ�������ֵ��ѯ
void WTM_Threshold_Query(uint8_t* pbuf,uint16_t len)
{
	uint8_t i;
	uint16_t delay_count = 0;
	QueryDown_t Query_Down;
	WTM_ThresholdQueryUp_t WTM_Threshold_QueryUp;
	Sensor_ThresholdQueryDownIn_t Sensor_Threshold_QueryDownIn;
	WTM_ThresholdQueryUpIn_t WTM_Threshold_QueryUpIn;
	
	memset(&WTM_Threshold_QueryUp,0,sizeof(WTM_ThresholdQueryUp_t));
	memcpy(&Query_Down,pbuf,len);	
	
	for(i = 0;i< WTM_SENSOR_NUM;i++)
	{
		if(Data_Save.WTM_Data_Save[i].sensorId != 0)
		{			
				Sensor_Threshold_QueryDownIn.messageId = MESSAGE_ID_WTM_THRESHOLD_QUERY_DOWN;
				Sensor_Threshold_QueryDownIn.sensorId = Data_Save.WTM_Data_Save[i].sensorId;

				memset(txbuf,0,NRF_DATA_LEN);
				memcpy(txbuf,&Sensor_Threshold_QueryDownIn,sizeof(Sensor_ThresholdQueryDownIn_t));

				NRF_TX_Mode();//����ģʽ          
				nrf_status = NRF_Tx_Dat(txbuf);//��������
		
				NRF_RX_Mode();//����ģʽ	
				nrf_status = NRF_Rx_Dat(rxbuf);// �ȴ��������� 
		
				while(nrf_status != RX_DR)//�жϽ���״̬  
				{
					delay_ms(1);
					nrf_status = NRF_Rx_Dat(rxbuf);// �ȴ��������� 
					delay_count++;
					if(delay_count > 1000)//1sû�лظ���ʧ��
						break;
				}	
				delay_count = 0;
				memcpy(&WTM_Threshold_QueryUpIn,rxbuf,sizeof(WTM_ThresholdQueryUpIn_t));
		
				if((WTM_Threshold_QueryUpIn.frameHead == FRAME_HEAD) && (WTM_Threshold_QueryUpIn.sensorId == Sensor_Threshold_QueryDownIn.sensorId))
				{				
					WTM_Threshold_QueryUp.sensorSetData[i].sensorId = BLEndianInt32(WTM_Threshold_QueryUpIn.sensorId);
					WTM_Threshold_QueryUp.sensorSetData[i].tempatureThreshold = BLEndianInt32((int32_t)(WTM_Threshold_QueryUpIn.tempature*10.0));
				}											
		}	
	}
	
	WTM_Threshold_QueryUp.messageId = MESSAGE_ID_WTM_THRESHOLD_QUERY_UP;
	WTM_Threshold_QueryUp.mid_errcode[0] = Query_Down.mid_res[0];
	WTM_Threshold_QueryUp.mid_errcode[1] = Query_Down.mid_res[0];
	WTM_Threshold_QueryUp.mid_errcode[2] = 0;

	NB_DataSend((char*)&WTM_Threshold_QueryUp,sizeof(WTM_ThresholdQueryUp_t));	
}

//λ�ƴ�������ֵ��ѯ
void WVS_Threshold_Query(uint8_t* pbuf,uint16_t len)
{
	uint8_t i;
	uint16_t delay_count = 0;
	QueryDown_t Query_Down;
	WVS_ThresholdQueryUp_t WVS_Threshold_QueryUp;
	Sensor_ThresholdQueryDownIn_t Sensor_Threshold_QueryDownIn;
	WVS_ThresholdQueryUpIn_t WVS_Threshold_QueryUpIn;
	
	memset(&WVS_Threshold_QueryUp,0,sizeof(WVS_ThresholdQueryUp_t));	
	memcpy(&Query_Down,pbuf,len);	
	
	for(i = 0;i< WVS_SENSOR_NUM;i++)
	{
		if(Data_Save.WVS_Data_Save[i].sensorId != 0)
		{			
				Sensor_Threshold_QueryDownIn.messageId = MESSAGE_ID_WVS_THRESHOLD_QUERY_DOWN;
				Sensor_Threshold_QueryDownIn.sensorId = Data_Save.WVS_Data_Save[i].sensorId;

				memset(txbuf,0,NRF_DATA_LEN);
				memcpy(txbuf,&Sensor_Threshold_QueryDownIn,sizeof(Sensor_ThresholdQueryDownIn_t));

				NRF_TX_Mode();//����ģʽ          
				nrf_status = NRF_Tx_Dat(txbuf);//��������
		
				NRF_RX_Mode();//����ģʽ	
				nrf_status = NRF_Rx_Dat(rxbuf);// �ȴ��������� 
		
				while(nrf_status != RX_DR)//�жϽ���״̬  
				{
					delay_ms(1);
					nrf_status = NRF_Rx_Dat(rxbuf);// �ȴ��������� 
					delay_count++;
					if(delay_count > 1000)//1sû�лظ���ʧ��
						break;
				}	
				delay_count = 0;
				memcpy(&WVS_Threshold_QueryUpIn,rxbuf,sizeof(WVS_ThresholdQueryUpIn_t));
		
				if((WVS_Threshold_QueryUpIn.frameHead == FRAME_HEAD) && (WVS_Threshold_QueryUpIn.sensorId == Sensor_Threshold_QueryDownIn.sensorId))
				{				
					WVS_Threshold_QueryUp.sensorSetData[i].sensorId = BLEndianInt32(WVS_Threshold_QueryUpIn.sensorId);
					WVS_Threshold_QueryUp.sensorSetData[i].acceleration_x= BLEndianInt16(WVS_Threshold_QueryUpIn.acceleration_x);
					WVS_Threshold_QueryUp.sensorSetData[i].acceleration_y= BLEndianInt16(WVS_Threshold_QueryUpIn.acceleration_y);
					WVS_Threshold_QueryUp.sensorSetData[i].acceleration_z= BLEndianInt16(WVS_Threshold_QueryUpIn.acceleration_z);
				}											
		}	
	}
	
	WVS_Threshold_QueryUp.messageId = MESSAGE_ID_WVS_THRESHOLD_QUERY_UP;
	WVS_Threshold_QueryUp.mid_errcode[0] = Query_Down.mid_res[0];
	WVS_Threshold_QueryUp.mid_errcode[1] = Query_Down.mid_res[1];
	WVS_Threshold_QueryUp.mid_errcode[2] = 0;

	NB_DataSend((char*)&WVS_Threshold_QueryUp,sizeof(WVS_ThresholdQueryUp_t));		
}

//����������ֵ��ѯ
void FEC_Threshold_Query(uint8_t* pbuf,uint16_t len)
{
	uint8_t i;
	uint16_t delay_count = 0;
	QueryDown_t Query_Down;
	FEC_ThresholdQueryUp_t FEC_Threshold_QueryUp;
	Sensor_ThresholdQueryDownIn_t Sensor_Threshold_QueryDownIn;
	FEC_ThresholdQueryUpIn_t FEC_Threshold_QueryUpIn;

	memset(&FEC_Threshold_QueryUp,0,sizeof(FEC_ThresholdQueryUp_t));		
	memcpy(&Query_Down,pbuf,len);	
	
	for(i = 0;i< FEC_SENSOR_NUM;i++)
	{
		if(Data_Save.FEC_Data_Save[i].sensorId != 0)
		{			
				Sensor_Threshold_QueryDownIn.messageId = MESSAGE_ID_FEC_THRESHOLD_QUERY_DOWN;
				Sensor_Threshold_QueryDownIn.sensorId = Data_Save.FEC_Data_Save[i].sensorId;

				memset(txbuf,0,NRF_DATA_LEN);
				memcpy(txbuf,&Sensor_Threshold_QueryDownIn,sizeof(Sensor_ThresholdQueryDownIn_t));

				NRF_TX_Mode();//����ģʽ          
				nrf_status = NRF_Tx_Dat(txbuf);//��������
		
				NRF_RX_Mode();//����ģʽ	
				nrf_status = NRF_Rx_Dat(rxbuf);// �ȴ��������� 
		
				while(nrf_status != RX_DR)//�жϽ���״̬  
				{
					delay_ms(1);
					nrf_status = NRF_Rx_Dat(rxbuf);// �ȴ��������� 
					delay_count++;
					if(delay_count > 1000)//1sû�лظ���ʧ��
						break;
				}	
				delay_count = 0;
				memcpy(&FEC_Threshold_QueryUpIn,rxbuf,sizeof(FEC_ThresholdQueryUpIn_t));
		
				if((FEC_Threshold_QueryUpIn.frameHead == FRAME_HEAD) && (FEC_Threshold_QueryUpIn.sensorId == Sensor_Threshold_QueryDownIn.sensorId))
				{				
					FEC_Threshold_QueryUp.sensorSetData[i].sensorId = BLEndianInt32(FEC_Threshold_QueryUpIn.sensorId);
					FEC_Threshold_QueryUp.sensorSetData[i].tempature= BLEndianInt32((int32_t)(FEC_Threshold_QueryUpIn.tempature)*10.0);
				}											
		}	
	}
	
	FEC_Threshold_QueryUp.messageId = MESSAGE_ID_FEC_THRESHOLD_QUERY_UP;
	FEC_Threshold_QueryUp.mid_errcode[0] = Query_Down.mid_res[0];
	FEC_Threshold_QueryUp.mid_errcode[1] = Query_Down.mid_res[1];
	FEC_Threshold_QueryUp.mid_errcode[2] = 0;

	NB_DataSend((char*)&FEC_Threshold_QueryUp,sizeof(FEC_ThresholdQueryUp_t));	
}

//�����ϴ�Ƶ�ʲ�ѯ
void Freq_Query(uint8_t* pbuf,uint16_t len)
{
	QueryDown_t Query_Down;
	FreqQueryUp_t Freq_QueryUp;

	memset(&Freq_QueryUp,0,sizeof(FreqQueryUp_t));			
	memcpy(&Query_Down,pbuf,len);
	Config_Info_Read();
	
	Freq_QueryUp.messageId = MESSAGE_ID_FREQ_QUERY_UP;
	Freq_QueryUp.mid_errcode[0] = Query_Down.mid_res[0];
	Freq_QueryUp.mid_errcode[1] = Query_Down.mid_res[1];	
	Freq_QueryUp.mid_errcode[2] = 0;
	
	Freq_QueryUp.ICT_Frequency = BLEndianInt16(Data_Save.ICT_Frequency);
	Freq_QueryUp.WTM_Frequency = BLEndianInt16(Data_Save.WTM_Frequency);
	Freq_QueryUp.WVS_Frequency = BLEndianInt16(Data_Save.WVS_Frequency);
	Freq_QueryUp.FEC_Frequency = BLEndianInt16(Data_Save.FEC_Frequency);
	
	NB_DataSend((char*)&Freq_QueryUp,sizeof(FreqQueryUp_t));
}

//�汾�Ų�ѯ
void Software_Version_Query(uint8_t* pbuf,uint16_t len)
{
	SoftwareVersion_t Software_Version;
	uint8_t swVer[8] = {'V', '1', '.', '0', '0', '.', '0', '1'}; //�汾��V1.00.01
	uint8_t swTimVer[8] = {'2', '0', '1', '9', '0', '8', '2', '1'}; //�޸�ʱ��20190821
	
	memset(&Software_Version,0,sizeof(SoftwareVersion_t));	
	
	Software_Version.messageId = MESSAGE_ID_VERSION_QUERY_UP;
	Software_Version.mid_errcode[0] = pbuf[1];
	Software_Version.mid_errcode[1] = pbuf[2];
	Software_Version.mid_errcode[2] = 0;
	
	memcpy(&Software_Version.version,swVer,8);
	memcpy(&Software_Version.time,swTimVer,8);
	NB_DataSend((char*)&Software_Version,sizeof(SoftwareVersion_t));
}


//�������ò�ѯ
void Data_Set_Query(void)
{
	uint8_t	RxBuf[512];
	uint16_t	RxLen = 0;
	uint8_t i = 0;
	uint8_t DataBuf[512];
	uint16_t DataLen = 0;
	
	if(NB_RxLen != 0)
	{
		//ת�����յ������ݣ���ʽ�����ݳ��� ���� ��������
		while(1)
		{
			if(i>3)//���������λ��
				break;
			if(NB_RxBuf[i++] == ',')
				break;
		}
		
		DataLen = NB_RxLen-i;
		NB_RxLen = 0;
		memcpy(DataBuf,&NB_RxBuf[i],DataLen);
		
		strToHex((char*)DataBuf,(char*)RxBuf,(uint32_t*)&RxLen);
		
		switch (RxBuf[0])
		{
			case MESSAGE_ID_ICT_THRESHOLD_SET_DOWN:
				ICT_Threshold_Set(RxBuf,RxLen);
				break;
			
			case MESSAGE_ID_WTM_THRESHOLD_SET_DOWN:
				WTM_Threshold_Set(RxBuf,RxLen);
				break;
			
			case MESSAGE_ID_WVS_THRESHOLD_SET_DOWN:
				WVS_Threshold_Set(RxBuf,RxLen);
				break;
			
			case MESSAGE_ID_FEC_THRESHOLD_SET_DOWN:		
				FEC_Threshold_Set(RxBuf,RxLen);
				break;
			
			case MESSAGE_ID_FREQ_SET_DOWN:
				Freq_Set(RxBuf,RxLen);
				break;
			
			case MESSAGE_ID_TIME_SET_DOWN:
				Time_Set(RxBuf,RxLen);
				break;
			
			case MESSAGE_ID_ICT_THRESHOLD_QUERY_DOWN:
				ICT_Threshold_Query(RxBuf,RxLen);
				break;
			
			case MESSAGE_ID_WTM_THRESHOLD_QUERY_DOWN:
				WTM_Threshold_Query(RxBuf,RxLen);
				break;
			
			case MESSAGE_ID_WVS_THRESHOLD_QUERY_DOWN:
				WVS_Threshold_Query(RxBuf,RxLen);
				break;
			
			case MESSAGE_ID_FEC_THRESHOLD_QUERY_DOWN:
				FEC_Threshold_Query(RxBuf,RxLen);
				break;
			
			case MESSAGE_ID_FREQ_QUERY_DOWN:
				Freq_Query(RxBuf,RxLen);
				break;
			
			case MESSAGE_ID_VERSION_QUERY_DOWN:
				Software_Version_Query(RxBuf,RxLen);
				break;
			
			default:
				break;
		}			
	}
}

//������ID����
//������sensorType:���������ͣ�1:�¶ȴ�������2��λ�ƴ�������3���������У�sensorID:������ID
void Sensor_ID_Set(uint8_t sensorType,uint32_t sensorID)
{
	static uint8_t WTM_Num = 0,WVS_Num = 0,FEC_Num = 0;
	SensorIdSetDown_t SensorId_SetDown;
	SensorIdSetUp_t SensorId_SetUp;
	
	SensorId_SetDown.messageId = MESSAGE_ID_SENSORID_SET_DOWN;
	SensorId_SetDown.sensorType = sensorType;
	SensorId_SetDown.SensorIdOld = 0;
	SensorId_SetDown.SensorIdNew = sensorID;
	
	memset(txbuf,0,NRF_DATA_LEN);
	memcpy(txbuf,&SensorId_SetDown,sizeof(SensorIdSetDown_t));
	
	Config_Info_Read();
	
	NRF_TX_Mode();//����ģʽ          
	nrf_status = NRF_Tx_Dat(txbuf);//��������
		
	NRF_RX_Mode();//����ģʽ	
	nrf_status = NRF_Rx_Dat(rxbuf);// �ȴ��������� 

	if(nrf_status != RX_DR)//�жϽ���״̬  
  {
    delay_ms(1);
  }
  else//���յ�����
  {
		if((rxbuf[0] == 0xFF) && (rxbuf[1] == 0xFF) && (rxbuf[2] == MESSAGE_ID_SENSORID_SET_UP))
		{
			memcpy(&SensorId_SetUp,rxbuf,sizeof(SensorIdSetUp_t));
			if(SensorId_SetUp.set_flag == 0)
			{
				if(SensorId_SetDown.sensorType == SENSOR_TYPE_WTM)
				{
					if(WTM_Num>=5) return;
					Data_Save.WTM_Data_Save[WTM_Num].sensorType = SensorId_SetDown.sensorType;
					Data_Save.WTM_Data_Save[WTM_Num].sensorId = SensorId_SetDown.SensorIdNew;
					WTM_Num++;					
				}
				else if(SensorId_SetDown.sensorType == SENSOR_TYPE_WVS)
				{
					if(WVS_Num>=5) return;					
					Data_Save.WVS_Data_Save[WVS_Num].sensorType = SensorId_SetDown.sensorType;
					Data_Save.WVS_Data_Save[WVS_Num].sensorId = SensorId_SetDown.SensorIdNew;
					WVS_Num++;					
				}
				else if(SensorId_SetDown.sensorType == SENSOR_TYPE_FEC)
				{
					if(FEC_Num>=5) return;					
					Data_Save.FEC_Data_Save[FEC_Num].sensorType = SensorId_SetDown.sensorType;
					Data_Save.FEC_Data_Save[FEC_Num].sensorId = SensorId_SetDown.SensorIdNew;					
					FEC_Num++;
				}								
				Config_Info_Save();
			}
		}
	}
}

//���ݴ���
void Data_Process(void)
{
//	static u8 flag = 0;	
//	
//	if((data_process_flag & DATA_EXTERNAL) == DATA_EXTERNAL)//������ʾ����ʱ�����ٲ⡢�ϱ������ò�ѯ
//	{

//		
//		//POWER_ON(1);
//		//delay_ms(1000);
//		
//		
//		
//		//Run_BEEP();
//		
//		key_num_t = 0;
//		

//			if(flag == 0)
//			{
//				
//				Show_HomePage();
//				key_num_t=Key_Read();
//				if(key_num_t!=0)
//				{
//					flag = 1;
//					Run_BEEP();
//				}
//			}
//			else
//			{
//				MenuReflash();
//			}
//		
//		

//		
//		//data_process_flag = DATA_EXTERNAL;
//		
//		
////		if(first_flag == 0)//��һ�����ϱ�һ���ٲ�����
////		{
////			
////			delay_ms(100);
////			
////			first_flag = 1;
////			
////			
////			//ICT_Data_Call();
////		}
////		else if(first_flag == 1)
////		{
////			
////			
////			delay_ms(500);
////			if(key_num!=0x00)
////			{
////				first_flag = 0;
////				MenuReflash();
////			}
////		}
//		
//		


//		
////		//��ʱ�����ڽ���͹���
////		//Data_Set_Query();		
////		
////		if(1)//��������������ʱ
////		{
////			Sensor_ID_Set(1,1);//������ID����
////		}
//////		else if(0)//�ٲ�����ʱ
//////		{
//////			//�����ϴ����ٲ�һ������
//////			Data_Call();
//////		}
//	}
//	else 
//	{
//		//POWER_ON(0);
//		OLED_Fill(0x00);
//		flag = 0;

//		
//		if((data_process_flag & DATA_CALL_START) == DATA_CALL_START) 
//		{
//			Data_Call();//�����ٲ�
//			data_process_flag |= DATA_CALL_OVER;//�ٲ����
//			data_process_flag &= (~DATA_CALL_START);//����ٲ⿪ʼ��־λ
//		}
//		else if((data_process_flag & DATA_REPORT_START) == DATA_REPORT_START)
//		{
//			Data_Report();//���������ϱ�
//		}
//		else if((data_process_flag & DATA_RECEIVE_UP) == DATA_RECEIVE_UP)//�յ����ò�ѯָ��
//		{
//			data_process_flag &= (~DATA_RECEIVE_UP);//�����־λ
//			//�Ȼظ���ǰ���ݣ����ٲ�����ٴ���
//			if((data_process_flag & DATA_CALL_OVER) == DATA_CALL_OVER)
//			{
//				Data_Set_Query();//�������ò�ѯ
//			}
//		}
//	}
//	
//	if(timer2_flag == 1)//��ʱʱ�䵽������͹���
//	{
//		timer2_flag = 0;	
//		data_process_flag &= (~DATA_CALL_START);
//		data_process_flag &= (~DATA_CALL_OVER);
//		//Stop_Mode();����͹���ģʽ
//	}

//	if(timer10_flag == 1)//��ʱʱ�䵽������͹���
//	{
//		timer10_flag = 0;	
//		data_process_flag &= (~DATA_REPORT_START);
//		//Stop_Mode();����͹���ģʽ
//	}
}



//32λ������С��ת��
int32_t BLEndianInt32(int32_t value)
{
    return ((value & 0x000000FF) << 24) |  ((value & 0x0000FF00) << 8) |  ((value & 0x00FF0000) >> 8) | ((value & 0xFF000000) >> 24);   
}

//16λ������С��ת��
int16_t BLEndianInt16(int16_t value)
{
    return ((value & 0x00FF) << 8) | ((value & 0xFF00) >> 8);     
}

//float�ʹ�С��ת��
float BLEndianFloat(float value)
{
    float_conv d1,d2;
    d1.f = value;
    d2.c[0] = d1.c[3];
    d2.c[1] = d1.c[2];
    d2.c[2] = d1.c[1];
    d2.c[3] = d1.c[0];
    return d2.f;
}
