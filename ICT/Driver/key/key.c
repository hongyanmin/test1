#include "key.h"

uint8_t KEY_Num=0;
//u16 iic_key[17]={0x00,0x00,0x83,0xf3,0x98,0x01,0x08,0x08,0x08,0x08,0x08,0x80,0x80,0x80,0x80,0x80,0x80};
//u16 iic_key17=0;

//key ��ʼ��
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//I2C���뿪©���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);
}


//����Ϊ���
void SDA_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //��©���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;//ʱ������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


//����Ϊ����
void SDA_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;  //����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;//ʱ������
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}


//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  


//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}


//�ȴ�Ӧ���źŵ���//����ֵ��1������Ӧ��ʧ��//0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 


//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}


//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 		


//IIC����һ���ֽ�//���شӻ�����Ӧ��//1����Ӧ��//0����Ӧ��		
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    


//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}


//��ȡ��ֵ
unsigned char Key_Read(void)
{
		u8 a=0,b=0;		
	
		IIC_Start();
		
		IIC_Send_Byte(0xA0);
		IIC_Wait_Ack();

		IIC_Send_Byte(0x08);
		IIC_Wait_Ack();

		IIC_Start();

		IIC_Send_Byte(0xA1);
		IIC_Wait_Ack();

		a=IIC_Read_Byte(1);
		b=IIC_Read_Byte(0);

		IIC_Stop();

		if(a==0x20)						{KEY_Num=1; return 1;}
		else if(a==0x40)				{KEY_Num=2; return 2;}
		else if(a==0x80)				{KEY_Num=3; return 3;}
		else if(a==0x10)				{KEY_Num=4; return 4;}
		else if(a==0x08)				{KEY_Num=5; return 5;}
		else if(a==0x04)				{KEY_Num=6; return 6;}
		else							return 0;
}


////���ð������ж�
//void Key_Sensitivity_Configuration(void)
//{
//		u8 i=0;
//		for(i=0;i<17;i++)
//		{
//			iic_key17 += iic_key[i];
//		}

//		//�ȴ�IIC����
//		while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));

//		//IIC_Start();
//		I2C_GenerateSTART(I2C2, ENABLE);//����I2C2
//		while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/
//		
//		//IIC���͵�ַ
//		I2C_Send7bitAddress(I2C2,0xA0,I2C_Direction_Transmitter);
//		while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

//		//IIC��������
//		I2C_SendData(I2C2,0xB0);
//		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

//		//IIC_Start();
//		I2C_GenerateSTART(I2C2, ENABLE);//����I2C2
//		while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/
//	
//		//IIC��������
//		I2C_SendData(I2C2,0xA1);
//		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));

//		//IIC��������
//		I2C_SendData(I2C2,0x00);
//		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
//		//IIC��������
//		I2C_SendData(I2C2,0x00);
//		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
//		//IIC��������
//		I2C_SendData(I2C2,0x83);
//		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
//		//IIC��������
//		I2C_SendData(I2C2,0xf3);
//		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
//		
//		for(i=0;i<6;i++)
//		{
//				//IIC��������
//				I2C_SendData(I2C2,0x01);
//				while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
//		}
//		
//		for(i=0;i<6;i++)
//		{
//				//IIC��������
//				I2C_SendData(I2C2,0x80);
//				while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
//		}
//		
//		//IIC�������� 18
//		I2C_SendData(I2C2,iic_key17);
//		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));//������  
//	
//		//IIC_Stop();//�ر�I2C2����
//		I2C_GenerateSTOP(I2C2, ENABLE);
//		delay_ms(100);
//}


////�жϺ���
//void EXTI2_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line2)!=RESET)
//	{ 
//		//KEY_STATE=1;
//	}	
//	EXTI_ClearITPendingBit(EXTI_Line2);
//}
