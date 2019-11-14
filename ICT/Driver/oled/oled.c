#include "oled.h"
#include "codetab.h"

u8 OLED_GRAM[128][8];	 


//oled ��ʼ��
void OLED_Init(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure; 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	RCC_AHBPeriphClockCmd(OLED_SCL_CLK|OLED_SDA_CLK,ENABLE);

	/*Ӳ��I2C: PB6 -- SCL; PB7 -- SDA */
	GPIO_InitStructure.GPIO_Pin =  OLED_SCL_PIN | OLED_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//I2C���뿪©���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(OLED_SCL_PORT, OLED_SCL_SOURCE, GPIO_AF_I2C1);
	GPIO_PinAFConfig(OLED_SDA_PORT, OLED_SDA_SOURCE, GPIO_AF_I2C1);

	GPIO_SetBits(OLED_SCL_PORT, OLED_SCL_PIN);  
	GPIO_SetBits(OLED_SDA_PORT, OLED_SDA_PIN); 

	I2C_DeInit(I2C1);//ʹ��I2C1
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x30;//������I2C��ַ,���д��
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;//400K

	I2C_Cmd(I2C1, ENABLE);
	I2C_Init(I2C1, &I2C_InitStructure);
	
	
	delay_ms(100); //�������ʱ����Ҫ
	
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //���ȵ��� 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
	

	
}



//IICд�ֽ�
void I2C_WriteByte(uint8_t addr,uint8_t data)
{
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2C1, ENABLE);//����I2C1
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

	I2C_Send7bitAddress(I2C1, OLED_ADDRESS, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C1, addr);//�Ĵ�����ַ
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(I2C1, data);//��������
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTOP(I2C1, ENABLE);//�ر�I2C1����
}


//д����
void WriteCmd(unsigned char I2C_Command)
{
	I2C_WriteByte(0x00, I2C_Command);
}


//д����
void WriteDat(unsigned char I2C_Data)
{
	I2C_WriteByte(0x40, I2C_Data);
}



//������ʼ������
void OLED_SetPos(unsigned char x, unsigned char y) 
{ 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}

//ȫ�����
void OLED_Fill(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				WriteDat(fill_Data);
			}
	}
}


//����
void OLED_CLS(void)
{
	OLED_Fill(0x00);
}


//��OLED�������л���
void OLED_ON(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X14);  //������ɱ�
	WriteCmd(0XAF);  //OLED����
}


//��OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X10);  //�رյ�ɱ�
	WriteCmd(0XAE);  //OLED����
}


// ��ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
//x,y -- ��ʼ������(x:0~127, y:0~7); ch[] -- Ҫ��ʾ���ַ���; TextSize -- �ַ���С(1:6*8 ; 2:8*16)
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}


// x,y -- ��ʼ������(x:0~127, y:0~7); ch[] -- Ҫ��ʾ���ַ���; TextSize -- �ַ���С(1:6*8 ; 2:8*16)
// ��ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char chr)
{
	unsigned char c = 0,i = 0;

				c = chr - 32;
//			c = chr;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
}

// x,y -- ��ʼ������(x:0~127, y:0~7); ch[] -- Ҫ��ʾ���ַ���; TextSize -- �ַ���С(1:6*8 ; 2:8*16)
// ��ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
void OLED_ShowChar_Small(unsigned char x, unsigned char y, unsigned char chr)
{
	unsigned char c = 0,i = 0;

				c = chr - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				x += 6;
}


//�˺���û�е�ͨ
////���� 
////x:0~127
////y:0~63
////t:1 ��� 0,���				   
//void OLED_DrawPoint(u8 x,u8 y,u8 t)
//{
//	u8 pos,bx,temp=0;
//	if(x>127||y>63)return;//������Χ��.
//	pos=7-y/8;
//	bx=y%8;
//	temp=1<<(7-bx);
//	if(t)OLED_GRAM[x][pos]|=temp;
//	else OLED_GRAM[x][pos]&=~temp;	    
//}


////��ָ��λ����ʾһ���ַ�,���������ַ�
////x:0~127
////y:0~63
////mode:0,������ʾ;1,������ʾ				 
////size:ѡ������ 12/16/24
//void OLED_ShowChar_Small(u8 x,u8 y,u8 chr,u8 size,u8 mode)
//{      			    
//	u8 temp,t,t1;
//	u8 y0=y;
//	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
//	chr=chr-' ';//�õ�ƫ�ƺ��ֵ		 
//    for(t=0;t<csize;t++)
//    {   
//		if(size==12)temp=asc2_1206[chr][t]; 	 	//����1206����
//		else if(size==16)temp=asc2_1608[chr][t];	//����1608����
//		else if(size==24)temp=asc2_2412[chr][t];	//����2412����
//		else return;								//û�е��ֿ�
//        for(t1=0;t1<8;t1++)
//		{
//			if(temp&0x80)OLED_DrawPoint(x,y,mode);
//			else OLED_DrawPoint(x,y,!mode);
//			temp<<=1;
//			y++;
//			if((y-y0)==size)
//			{
//				y=y0;
//				x++;
//				break;
//			}
//		}  	 
//    }          
//}


//x,y -- ��ʼ������(x:0~127, y:0~7); N:������codetab.h�е�����
//��ʾcodetab.h�еĺ���,16*16����
//--------------------------------------------------------------
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int  adder=32*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
}


//x0,y0 -- ��ʼ������(x0:0~127, y0:0~7); x1,y1 -- ���Խ���(������)������(x1:1~128,y1:1~8)
//��ʾBMPλͼ
//--------------------------------------------------------------
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			WriteDat(BMP[j++]);
		}
	}
}


//void OLED_Page1(void)
//{
//		OLED_ShowStr(16,0,"01",2);		
//		OLED_ShowStr(16,2,"02",2);		
//		OLED_ShowStr(16,4,"03",2);		
//		OLED_ShowStr(16,6,"04",2);		
//	
//		OLED_ShowCN(32,0,0);  	//��
//		OLED_ShowCN(48,0,1);		//��
//		OLED_ShowCN(64,0,2);		//��
//		OLED_ShowCN(80,0,3);		//��
//		
//		OLED_ShowCN(32,2,4);		//��
//		OLED_ShowCN(48,2,5);		//��
//		OLED_ShowCN(64,2,6);		//��
//		
//		OLED_ShowCN(32,4,7);		//��
//		OLED_ShowCN(48,4,8);		//��
//		OLED_ShowCN(64,4,9);		//��
//		OLED_ShowCN(80,4,10);		//��
//		OLED_ShowCN(96,4,11);		//��
//		OLED_ShowCN(112,4,12);	//Ϣ
//				
//		OLED_ShowCN(32,6,13);		//��
//		OLED_ShowCN(48,6,14);		//��
//		OLED_ShowCN(64,6,15);		//ʱ
//		OLED_ShowCN(80,6,16);		//��
//		OLED_ShowCN(96,6,17);		//��
//		OLED_ShowCN(112,6,18);	//��
//}	


//void OLED_Page2(void)
//{	
//		OLED_ShowStr(16,0,"05",2);		
//		OLED_ShowStr(16,2,"06",2);		
//		OLED_ShowStr(16,4,"07",2);		
//		OLED_ShowStr(16,6,"08",2);		
//	
//		OLED_ShowCN(32,0,19);		//��
//		OLED_ShowCN(48,0,20);		//��
//		OLED_ShowCN(64,0,15);		//ʱ
//		OLED_ShowCN(80,0,16);		//��
//		OLED_ShowCN(96,0,17);		//��
//		OLED_ShowCN(112,0,18);	//��
//	
//	
//		OLED_ShowCN(32,2,0);		//��
//		OLED_ShowCN(48,2,1);		//��
//		OLED_ShowCN(64,2,13);		//��
//		OLED_ShowCN(80,2,14);		//��
//	
//		OLED_ShowCN(32,4,0);		//��
//		OLED_ShowCN(48,4,1);		//��
//		OLED_ShowCN(64,4,21);		//ˮ
//		OLED_ShowCN(80,4,22);		//λ
//		
//		OLED_ShowCN(32,6,0);		//��
//		OLED_ShowCN(48,6,1);		//��
//		OLED_ShowCN(64,6,23);		//ģ
//		OLED_ShowCN(80,6,24);		//��
//		OLED_ShowCN(96,6,2);		//��
//		OLED_ShowCN(112,6,3);		//��
//}	

//void OLED_Page3(void)
//{	
//		OLED_ShowStr(16,0,"09",2);		
//	
//		OLED_ShowCN(32,0,25);		//��
//		OLED_ShowCN(48,0,26);		//��
//		OLED_ShowCN(64,0,11);		//��
//		OLED_ShowCN(80,0,12);		//Ϣ
//}	

void OLED_Line1(unsigned char LINE)
{
		OLED_ShowStr(16,(LINE-1)*2,"01",2);		
		OLED_ShowCN(32,(LINE-1)*2,0);		//��
		OLED_ShowCN(48,(LINE-1)*2,1);		//��
		OLED_ShowCN(64,(LINE-1)*2,49);		//��
		OLED_ShowCN(80,(LINE-1)*2,50);		//��
		OLED_ShowCN(96,(LINE-1)*2,2);		//��
		OLED_ShowCN(112,(LINE-1)*2,3);		//��
}

void OLED_Line2(unsigned char LINE)
{
		OLED_ShowStr(16,(LINE-1)*2,"02",2);
		OLED_ShowCN(32,(LINE-1)*2,4);		//��
		OLED_ShowCN(48,(LINE-1)*2,5);		//��
		OLED_ShowCN(64,(LINE-1)*2,6);		//��
		OLED_ShowCN(80,(LINE-1)*2,11);		//��
		OLED_ShowCN(96,(LINE-1)*2,12);		//Ϣ
}

void OLED_Line3(unsigned char LINE)
{
		OLED_ShowStr(16,(LINE-1)*2,"03",2);		
		OLED_ShowCN(32,(LINE-1)*2,13);		//��
		OLED_ShowCN(48,(LINE-1)*2,14);		//��
		OLED_ShowCN(64,(LINE-1)*2,15);		//ʱ
		OLED_ShowCN(80,(LINE-1)*2,16);		//��
		OLED_ShowCN(96,(LINE-1)*2,17);		//��
		OLED_ShowCN(112,(LINE-1)*2,18);		//��
}
void OLED_Line4(unsigned char LINE)
{
		OLED_ShowStr(16,(LINE-1)*2,"04",2);		
		OLED_ShowCN(32,(LINE-1)*2,19);		//��
		OLED_ShowCN(48,(LINE-1)*2,20);		//��
		OLED_ShowCN(64,(LINE-1)*2,15);		//ʱ
		OLED_ShowCN(80,(LINE-1)*2,16);		//��
		OLED_ShowCN(96,(LINE-1)*2,17);		//��
		OLED_ShowCN(112,(LINE-1)*2,18);		//��
}
void OLED_Line5(unsigned char LINE)
{
		OLED_ShowStr(16,(LINE-1)*2,"05",2);		
//		OLED_ShowStr(16,(LINE-1)*2,"06",2);		

		OLED_ShowCN(32,(LINE-1)*2,13);		//��
		OLED_ShowCN(48,(LINE-1)*2,14);		//��
		OLED_ShowCN(64,(LINE-1)*2,43);		//��
		OLED_ShowCN(80,(LINE-1)*2,51);		//��
	
}
void OLED_Line6(unsigned char LINE)
{
		OLED_ShowStr(16,(LINE-1)*2,"06",2);		
		OLED_ShowCN(32,(LINE-1)*2,52);		//��
		OLED_ShowCN(48,(LINE-1)*2,53);		//��
		OLED_ShowCN(64,(LINE-1)*2,43);		//��
		OLED_ShowCN(80,(LINE-1)*2,44);		//��
		OLED_ShowCN(96,(LINE-1)*2,23);		//ģ
		OLED_ShowCN(112,(LINE-1)*2,24);		//��
}
void OLED_Line7(unsigned char LINE)
{
		OLED_ShowStr(16,(LINE-1)*2,"07",2);		
		OLED_ShowCN(32,(LINE-1)*2,52);		//��
		OLED_ShowCN(48,(LINE-1)*2,53);		//��
		OLED_ShowCN(64,(LINE-1)*2,21);		//��
		OLED_ShowCN(80,(LINE-1)*2,22);		//��
		OLED_ShowCN(96,(LINE-1)*2,23);		//ģ
		OLED_ShowCN(112,(LINE-1)*2,24);		//��
}
void OLED_Line8(unsigned char LINE)
{
		OLED_ShowStr(16,(LINE-1)*2,"08",2);		
		OLED_ShowCN(32,(LINE-1)*2,25);		//��
		OLED_ShowCN(48,(LINE-1)*2,26);		//��
		OLED_ShowCN(64,(LINE-1)*2,11);		//��
		OLED_ShowCN(80,(LINE-1)*2,12);		//Ϣ
}
void OLED_Line9(unsigned char LINE)
{
		OLED_ShowStr(16,(LINE-1)*2,"09",2);		
		OLED_ShowCN(32,(LINE-1)*2,47);		//״
		OLED_ShowCN(48,(LINE-1)*2,48);		//̬
		OLED_ShowCN(64,(LINE-1)*2,11);		//��
		OLED_ShowCN(80,(LINE-1)*2,12);		//Ϣ
}

