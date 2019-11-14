#include "menu.h"
//#include "protocol.h"


extern u8 key_num;

//�˵��ṹ����
struct Option
{
    u8 KeyLevel;       //�˵�ѡ�������˵����
    u8 EnterIndex;     //ѡ�����������
    u8 CancelIndex;    //ѡ���˳�������
};

//����˵�ѡ��壬����һ���ṹ���� �洢�ڱ���������ʡ�ڴ�RAM
struct Option sOption[OPTIONMETE] =
{
	//�˵���� �������� �˳����� 
	{0, OPTIONMETE - 1, 0,},//0
	{0, OPTIONMETE - 1, 1,},//1
	{0, OPTIONMETE - 1, 2,},//2
	{0, OPTIONMETE - 1, 3,},//3
	{0, OPTIONMETE - 1, 4,},//4
	{0, OPTIONMETE - 1, 5,},//5
	{0, OPTIONMETE - 1, 6,},//6
	{0, OPTIONMETE - 1, 7,},//7
	{0, OPTIONMETE - 1, 8,},//7
};

/*�˵����������*/  //ÿ�����Ӧ�Ŀ�ʼ����  ��������  ѡ����Ŀ*/
u8 Level[3] = {0, 8, 9};

/*�˵���������*/
u8 FirstLineIndex = 0;    		//��Ļ��һ����ʾ��������
u8 SelectLine_L = 1;		    //ԭ��ѡ����
u8 SelectLine = 1;      		//��ǰѡ�����
u8 SelectIndex = 0;     		//��ǰѡ���ж�Ӧ��������
u8 LastIndex = 0;      			//���빦�ܺ���ǰ�������ţ��жϾ��幦��ʹ��

//ģ����
u8 ICT_Num[9]={1,9,1,1,0,0,0,0,1};
//�¶ȴ��������
u8 Tmp_Num[9]={1,9,1,1,1,0,0,0,1};
//λ�ƴ��������
u8 Vib_Num[9]={1,9,1,1,2,0,0,0,1};


u8 Server[]={1,9,2,1,6,8,0,0,3,0,0,1,8,0,8,6};
u8 Clear=1;
u8 UpTime[]={0,0,1};
u8 SampTime[]={0,0,1};
u8 Up;
u8 Water;



u8 LevelNum[3] = {0, 8, 9};
u8 SelectIndexNum = 0;     		//��ǰѡ���ж�Ӧ��������

u8 LevelServer[3]={0,15,16};
u8 SelectIndexServer=0;

u8 LevelUpTime[3]={0,2,3};
u8 SelectIndexUpTime=0;

u8 LevelSampTime[3]={0,2,3};
u8 SelectIndexSampTime=0;




//ˢ�²˵�
void MenuReflash()
{
	u8 a;
	//a=key_num;
	a=Key_Read();
	switch(a)
	{
		case 2:	//�ϼ�
		{
			Run_BEEP();					//������
			OLED_CLS();					//����		
			a=0;						//����ֵ����
			MenuUpOneOption();			//�����ϼ�������
		}break;
		
		case 5:	//�¼�
		{	
			Run_BEEP();
			OLED_CLS();
			a=0;
			MenuDownOneOption();
		}break;
		
		case 1:	//���
		{		
			Run_BEEP();
			OLED_CLS();
			a=0;
			MenuCancelOption();
		}break;
		
		case 3:	//�Ҽ�
		{
			Run_BEEP();
			OLED_CLS();
			a=0;
			MenuEnterOption();
		}break;
		
		
		default:
		break;
	}
}


//ˢ����ʾ
void Display_OLEDMenu (unsigned char index,unsigned char line)
{
	//��ʾ�˵���4��
	u8 i;
	for(i=0;i<4;i++)
	{
		Display_OLEDLine(index+i, i+1);
	}
	
	//��ʾ����λ��
	OLED_ShowStr(8,(line-1)*2,">",2);		
}


//��ʾĳһ��
void Display_OLEDLine (unsigned char index,unsigned char line)
{
	switch(index)
	{
		case 0:			OLED_Line1(line);				break;
		case 1:			OLED_Line2(line);				break;
		case 2:			OLED_Line3(line);				break;
		case 3:			OLED_Line4(line);				break;
		case 4:			OLED_Line5(line);				break;
		case 5:			OLED_Line6(line);				break;
		case 6:			OLED_Line7(line);				break;
		case 7:			OLED_Line8(line);				break;
		case 8:			OLED_Line9(line);				break;
		default:										break;
	}
}


//1  �˵�����һ������
void MenuUpOneOption (void)
{
	//�����ǰΪ����һ��
	if(SelectIndex == Level[0])    					
    {
		FirstLineIndex = Level[1] - 3; 		//��һ����ʾ������Ϊ����������
		SelectIndex = Level[1]; 
		SelectLine_L = SelectLine;			//��¼ԭ���� Ϊ��һ��
		SelectLine = 4;             		//���ѡ����Ϊ������
    }
		//�����ǰ���ǿ�ʼ����
    else        				
    {
		//�����Ѿ�����Ļ���ϱ�һ��
        if(SelectLine == 1)  								
        {
            FirstLineIndex--;				//��ʾ��������
            SelectIndex--;					//ѡ�������Լ�
			SelectLine_L = SelectLine;		//��¼ԭ����
            SelectLine = 1;					//ѡ���л��ǵ�һ��
        }
		//������ǵ�һ��
        else       									
        {
			SelectLine_L = SelectLine;		//��¼ԭ����
            SelectLine--;					//ѡ�����Լ�
            SelectIndex--;					//ѡ�������Լ�
        }
    }
		
	//ˢ����Ļ��ʾ
	Display_OLEDMenu(FirstLineIndex,SelectLine); 		
}


//2  �˵�����һ���
void MenuDownOneOption (void)
{
	//�������������
	if(SelectIndex == Level[1])
    {
        FirstLineIndex = Level[0];			//��һ����ʾ����Ϊ����һ��ѡ��
        SelectIndex = Level[0];				//ѡ������Ϊ����һ��ѡ������
		SelectLine_L = SelectLine;			//��¼ԭ����
		SelectLine = 1;						//ѡ����Ϊ��һ��
    }
	//���������������
    else           
    {
		//�����ǰ������Ļ�����
        if(SelectLine != 4)     						
        {
			SelectIndex++;					//ѡ�������Լ�
            SelectLine_L = SelectLine;		//��¼ԭ����
			SelectLine++;					//ѡ��������
        }
		//�������Ļ�����
        else          											
        {
            FirstLineIndex++;				//��һ����ʾ����
            SelectIndex++;					//ѡ�������Լ�
        }
    }
		
	Display_OLEDMenu(FirstLineIndex,SelectLine);
}


//3  ����ĳ��ܺ���
void MenuEnterOption (void)
{
	//��ǽ���ǰ�������ţ��Ա��жϾ��幦�ܣ�
    LastIndex = SelectIndex;           
		
	//����ѡ������Ϊ֮ǰ�����Ŷ�Ӧ��������
    SelectIndex = sOption[SelectIndex].EnterIndex;     
		
	//���ݽ���ǰ�������жϾ��庯��
	switch (LastIndex)            
    {
		case 0:			Function1();		break;
		case 1:			Function2();		break;
		case 2:			Function3();		break;
		case 3:			Function4();		break;
		case 4:			Function5();		break;
		case 5:			Function6();		break;
		case 6:			Function7();		break;
		case 7:			Function8();		break;
		case 8:			Function9();		break;
		default:			break;      
    }
	
}

//4  �˵��˳����ܺ���
void MenuCancelOption (void)
{
	//������Ǵӹ��ܷ���
	if(SelectIndex != OPTIONMETE - 1)        
	{
		SelectIndex = sOption[SelectIndex].CancelIndex;     //ѡ������Ϊѡ�������
	}
	//����Ǵӹ��ܷ���
	else                
	{
		SelectIndex = LastIndex;         			 		//�������ڽ���ǰ��������
	}

	//���ݷ���ѡ��ȷ����ʾ����
	if(SelectIndex > Level[1] - 3) 
	{
		FirstLineIndex = Level[1] - 3;
		SelectLine_L = SelectLine;
		SelectLine =  4 - (Level[1] - SelectIndex);
	}
	//һ����ʾ��ʽ
	else               
	{
		FirstLineIndex = SelectIndex;       				//��һ����ʾ����
		SelectLine_L = SelectLine;
		SelectLine = 1;            							//ѡ���һ��
	}
	
	Display_OLEDMenu(FirstLineIndex,SelectLine);
}


//����1
void Function1(void)
{
	u8 i,a=0;
	
	//1��ʾ����
	OLED_ShowCN(8,0,25);		//��
	OLED_ShowCN(24,0,26);		//��
	OLED_ShowCN(40,0,2);		//��
	OLED_ShowCN(56,0,3);		//Ϣ

	//2��ʾ����
	for(i=0;i<9;i++)
	{
		OLED_ShowChar(i*8+8,4,ICT_Num[i]+48);		
	}
	
	//3�����ʾ & ��ʾ��ʼλ��
	for(i=0;i<120;i++)		
	{
		OLED_ShowStr(i,6," ",2);		
	}
	OLED_ShowStr((SelectIndexNum*8)+8,6,"^",1);		

	//��ȡ����ֵ
	while(a==0)			
	{
		a=Key_Read();					//��ȡ����
		delay_ms(100);					//����
	}
	
	//�жϰ���ֵΪ���������ֵ
	while(a==2||a==4||a==5||a==6)
	{
		//���
		if(a==4) 
		{
			Run_BEEP();				
			a=0;					
			NumLeftOption();		
			while(a==0)
			{
				a=Key_Read();		
				delay_ms(100);		
			}
		}
		//�Ҽ�
		else if(a==6)
		{
			Run_BEEP();		
			a=0;	
			NumRightOption();
			while(a==0)			
			{
				a=Key_Read();		
				delay_ms(100);		
			}
		}
		//�ϼ�
		else if(a==2)
		{
			Run_BEEP();
			a=0;	
			
			//�޶��������� & ѡ������ֵ
			if(ICT_Num[SelectIndexNum]==9)				ICT_Num[SelectIndexNum]=0;			
			else									ICT_Num[SelectIndexNum]++;
			
			//��ʾ��ֵ
			OLED_ShowChar((SelectIndexNum*8+8),4,ICT_Num[SelectIndexNum]+48);		
			
			while(a==0)			{a=Key_Read();delay_ms(100);}
		}
		//�¼�
		else if(a==5)
		{
			Run_BEEP();		
			a=0;					
			
			if(ICT_Num[SelectIndexNum]==0)				ICT_Num[SelectIndexNum]=9;			
			else									ICT_Num[SelectIndexNum]--;
			
			OLED_ShowChar((SelectIndexNum*8+8),4,ICT_Num[SelectIndexNum]+48);		

			while(a==0)			{a=Key_Read();delay_ms(100);}
		}
	}
	
	//��ok
	if(a==1)  
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
	//������
	else if(a==3)		
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
}


//����1����
void NumLeftOption(void)
{
	u8 i=0;
	
	//���������ߵ����� �Ͷ�λ�����ұ�
	if(SelectIndexNum == LevelNum[0])		SelectIndexNum=LevelNum[1];
	else									SelectIndexNum--;
	
	//�����ʾ
	for(i=0;i<128;i++)			OLED_ShowStr(i,6," ",1);		
	
	//ָʾ����λ��
	OLED_ShowStr((SelectIndexNum*8+8),6,"^",1);		
}


//����1����
void NumRightOption(void)
{
	u8 i=0;
	
	//��������ұߵ����� �Ͷ�λ������� ��֮��Ȼ
	if(SelectIndexNum == LevelNum[1])		SelectIndexNum=LevelNum[0];
	else									SelectIndexNum++;
			
	//ָʾ����λ��
	for(i=0;i<128;i++)			OLED_ShowStr(i,6," ",1);		
			
	//ָʾ����λ��
	OLED_ShowStr((SelectIndexNum*8+8),6,"^",1);		
}


//����2
void Function2(void)
{
	u8 i,k=0;
	u8 a=0;

	//ָʾ����λ��
	OLED_ShowStr(0,0,"IP:",1);		
	OLED_ShowStr(0,4,"PORT:",1);		

	//��ʾIP
	for(i=0;i<3;i++)
	{
		OLED_ShowChar(i*8,1,Server[k]+48);		
		k++;
	}
	OLED_ShowChar(24,1,'.');		
	for(i=4;i<7;i++)
	{
		OLED_ShowChar(i*8,1,Server[k]+48);		
		k++;
	}
	OLED_ShowChar(56,1,'.');		
	for(i=8;i<11;i++)
	{
		OLED_ShowChar(i*8,1,Server[k]+48);		
		k++;
	}
	OLED_ShowChar(88,1,'.');		
	for(i=12;i<15;i++)
	{
		OLED_ShowChar(i*8,1,Server[k]+48);		
		k++;
	}
		
	//��ʾ�˿ں�
	for(i=16;i<20;i++)
	{
		OLED_ShowChar((i-16)*8,5,Server[k]+48);		
		k++;
	}
	
	while(a==0)			{a=Key_Read();delay_ms(100);}

	//��ok
	if(a==1)  
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
	//������
	else if(a==3)		
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
}


////����2 //��������Ͷ˿� ��ʱ����
//void Function2(void)
//{
//	u8 i,k=0;
//	u8 a=0,n;

//	//ָʾ����λ��
//	OLED_ShowStr(0,0,"IP:",1);		
//	OLED_ShowStr(0,4,"PORT:",1);		

//	//��ʾIP
//	for(i=0;i<3;i++)
//	{
//		OLED_ShowChar(i*8,1,Server[k]+48);		
//		k++;
//	}
//	OLED_ShowChar(24,1,'.');		
//	for(i=4;i<7;i++)
//	{
//		OLED_ShowChar(i*8,1,Server[k]+48);		
//		k++;
//	}
//	OLED_ShowChar(56,1,'.');		
//	for(i=8;i<11;i++)
//	{
//		OLED_ShowChar(i*8,1,Server[k]+48);		
//		k++;
//	}
//	OLED_ShowChar(88,1,'.');		
//	for(i=12;i<15;i++)
//	{
//		OLED_ShowChar(i*8,1,Server[k]+48);		
//		k++;
//	}
//		
//	//��ʾ�˿ں�
//	for(i=16;i<20;i++)
//	{
//		OLED_ShowChar((i-16)*8,5,Server[k]+48);		
//		k++;
//	}
//	
//	//�����ʾ & ��ʾ��ʼλ��
//	for(i=0;i<120;i++)		
//	{
//		OLED_ShowStr(i,3," ",1);		
//	}
//	OLED_ShowStr((SelectIndexServer*8),3,"^",1);		
//		
//	while(a==0)			{a=Key_Read();delay_ms(100);}
//		
//	while(a==2||a==4||a==5||a==6)
//	{
//		if(a==4) 
//		{
//			Run_BEEP();		a=0;		
//			ServerLeftOption();		
//			while(a==0)			{a=Key_Read();delay_ms(100);}
//		}
//		else if(a==6)
//		{
//			Run_BEEP();		a=0;		
//			ServerRightOption();
//			while(a==0)			{a=Key_Read();delay_ms(100);}
//		}
//		else if(a==2)
//		{
//			Run_BEEP();		a=0;	
//			
//			//�޶���������
//			if(Server[SelectIndexServer]==9)			Server[SelectIndexServer]=0;			
//			else										Server[SelectIndexServer]++;
//		
//			//��ʾIP&Port
//			if(SelectIndexServer==0||SelectIndexServer==1||SelectIndexServer==2)
//					OLED_ShowChar((SelectIndexServer*8),1,Server[SelectIndexServer]+48);		
//			else if(SelectIndexServer==3||SelectIndexServer==4||SelectIndexServer==5)
//					OLED_ShowChar(((SelectIndexServer*8)+8),1,Server[SelectIndexServer]+48);		
//			else if(SelectIndexServer==6||SelectIndexServer==7||SelectIndexServer==8)
//					OLED_ShowChar(((SelectIndexServer*8)+16),1,Server[SelectIndexServer]+48);		
//			else if(SelectIndexServer==9||SelectIndexServer==10||SelectIndexServer==11)
//					OLED_ShowChar(((SelectIndexServer*8)+24),1,Server[SelectIndexServer]+48);		
//			else
//					OLED_ShowChar((((SelectIndexServer-12)*8)),5,Server[SelectIndexServer]+48);		
//			
//			while(a==0)			{a=Key_Read();delay_ms(100);}

//		}
//		else if(a==5)
//		{
//			Run_BEEP();		a=0;	
//				
//			//�޶���������		
//			if(Server[SelectIndexServer]==0)		Server[SelectIndexServer]=9;			
//			else									Server[SelectIndexServer]--;
//			
//			//��ʾIP&Port
//			if(SelectIndexServer==0||SelectIndexServer==1||SelectIndexServer==2)
//					OLED_ShowChar((SelectIndexServer*8),1,Server[SelectIndexServer]+48);		
//			else if(SelectIndexServer==3||SelectIndexServer==4||SelectIndexServer==5)
//					OLED_ShowChar(((SelectIndexServer*8)+8),1,Server[SelectIndexServer]+48);		
//			else if(SelectIndexServer==6||SelectIndexServer==7||SelectIndexServer==8)
//					OLED_ShowChar(((SelectIndexServer*8)+16),1,Server[SelectIndexServer]+48);	
//			else if(SelectIndexServer==9||SelectIndexServer==10||SelectIndexServer==11)
//					OLED_ShowChar(((SelectIndexServer*8)+24),1,Server[SelectIndexServer]+48);		
//			else
//					OLED_ShowChar((((SelectIndexServer-12)*8)),5,Server[SelectIndexServer]+48);		
//			
//			while(a==0)			{a=Key_Read();delay_ms(100);}
//		}
//	}
//		
//	//��ok
//	if(a==1)  
//	{
//		Run_BEEP();		OLED_CLS();		a=0;
//		MenuCancelOption();
//	}
//	//������
//	else if(a==3)		
//	{
//		Run_BEEP();		OLED_CLS();		a=0;
//		MenuCancelOption();
//	}
//}


////����2����
//void ServerLeftOption(void)
//{
//	u8 i=0;
//	//���������ߵ����� �Ͷ�λ�����ұ�
//	if(SelectIndexServer == LevelServer[0])		SelectIndexServer=LevelServer[1];
//	else										SelectIndexServer--;
//		
//	//����Ϊ120 ��ֹ��� ����������ط�
//	for(i=0;i<120;i++)		OLED_ShowStr(i,3," ",1);		
//	for(i=0;i<120;i++)		OLED_ShowStr(i,7," ",1);		
//		
//		
//	if(SelectIndexServer==0||SelectIndexServer==1||SelectIndexServer==2)
//		OLED_ShowStr((SelectIndexServer*8),3,"^",1);		
//	else if(SelectIndexServer==3||SelectIndexServer==4||SelectIndexServer==5)
//		OLED_ShowStr((SelectIndexServer*8+8),3,"^",1);		
//	else if(SelectIndexServer==6||SelectIndexServer==7||SelectIndexServer==8)
//		OLED_ShowStr((SelectIndexServer*8+16),3,"^",1);		
//	else if(SelectIndexServer==9||SelectIndexServer==10||SelectIndexServer==11)
//		OLED_ShowStr((SelectIndexServer*8+24),3,"^",1);		
//	else
//		OLED_ShowStr(((SelectIndexServer-12)*8),7,"^",1);		
//}


////����2����
//void ServerRightOption(void)
//{			
//	u8 i=0;
//	
//	//��������ұߵ����� �Ͷ�λ�������
//	if(SelectIndexServer == LevelServer[1])		SelectIndexServer=LevelServer[0];
//	else										SelectIndexServer++;
//			
//	for(i=0;i<120;i++)		OLED_ShowStr(i,3," ",1);		
//	for(i=0;i<120;i++)		OLED_ShowStr(i,7," ",1);		
//		
//	if(SelectIndexServer==0||SelectIndexServer==1||SelectIndexServer==2)
//		OLED_ShowStr((SelectIndexServer*8),3,"^",1);		
//	else if(SelectIndexServer==3||SelectIndexServer==4||SelectIndexServer==5)
//		OLED_ShowStr((SelectIndexServer*8+8),3,"^",1);		
//	else if(SelectIndexServer==6||SelectIndexServer==7||SelectIndexServer==8)
//		OLED_ShowStr((SelectIndexServer*8+16),3,"^",1);		
//	else if(SelectIndexServer==9||SelectIndexServer==10||SelectIndexServer==11)
//		OLED_ShowStr((SelectIndexServer*8+24),3,"^",1);		
//	else
//		OLED_ShowStr(((SelectIndexServer-12)*8),7,"^",1);		
//}


//����3
void Function3(void)
{
	u8 a;
	
	OLED_ShowCN(0,0,13);	//��0
	OLED_ShowCN(16,0,14);	//��16
	OLED_ShowCN(32,0,15);	//ʱ32
	OLED_ShowCN(48,0,16);	//��48
	OLED_ShowCN(64,0,17);	//��
	OLED_ShowCN(80,0,18);	//��

	OLED_ShowStr(0,3,"15 Minute",2);

	while(a==0)			{a=Key_Read();delay_ms(100);}
		
	//��ok
	if(a==1)  
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
	//������
	else if(a==3)		
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
}


//��ʱ����
////����3
//void Function3(void)
//{
//	u8 a;
//	
//	OLED_ShowCN(0,0,13);	//��0
//	OLED_ShowCN(16,0,14);	//��16
//	OLED_ShowCN(32,0,15);	//ʱ32
//	OLED_ShowCN(48,0,16);	//��48
//	OLED_ShowCN(64,0,17);	//��
//	OLED_ShowCN(80,0,18);	//��

//	OLED_ShowCN(8,4,7);		//��
//	OLED_ShowCN(24,4,8);	//��

//	OLED_ShowCN(56,4,11);	//��
//	OLED_ShowCN(72,4,12);	//Ϣ
//	
//	
//	if(Clear==0) 
//	{
//		OLED_ShowStr(0,4,">",2);		
//		OLED_ShowStr(48,4," ",2);		
//	}
//	else if(Clear==1)
//	{
//		OLED_ShowStr(48,4,">",2);		
//		OLED_ShowStr(0,4," ",2);		
//	}
//	
//	while(a==0)			{a=Key_Read();delay_ms(100);}
//					
//	while(a==4||a==6)
//	{
//		if(a==4) 
//		{
//			Run_BEEP();		a=0;	
//		
//			if(Clear==0) 
//			{
//				Clear=1;
//				OLED_ShowStr(48,4,">",2);		
//				OLED_ShowStr(0,4," ",2);		
//			}
//			else if(Clear==1) 				
//			{
//				Clear=0;
//				OLED_ShowStr(0,4,">",2);		
//				OLED_ShowStr(48,4," ",2);		
//			}
//			
//			while(a==0)			{a=Key_Read();delay_ms(100);}
//		}
//		else if(a==6)
//		{
//			Run_BEEP();		a=0;	
//			
//			if(Clear==0) 
//			{
//				Clear=1;
//				OLED_ShowStr(48,4,">",2);		
//				OLED_ShowStr(0,4," ",2);		
//			}
//			else if(Clear==1) 			
//			{
//				Clear=0;
//				OLED_ShowStr(0,4,">",2);		
//				OLED_ShowStr(48,4," ",2);		
//			}
//				
//			while(a==0)			{a=Key_Read();delay_ms(100);}
//		}
//	}
//		
//	//��ok
//	if(a==1)  
//	{
//		Run_BEEP();		OLED_CLS();		a=0;
//		MenuCancelOption();
//	}
//	//������
//	else if(a==3)		
//	{
//		Run_BEEP();		OLED_CLS();		a=0;
//		MenuCancelOption();
//	}
//}


//����4
void Function4(void)
{
	u8 a;
	OLED_ShowCN(0,0,19);		//��
	OLED_ShowCN(16,0,20);		//��
	OLED_ShowCN(32,0,15);		//ʱ
	OLED_ShowCN(48,0,16);		//��
	OLED_ShowCN(64,0,17);		//��
	OLED_ShowCN(80,0,18);		//��
	
	OLED_ShowStr(0,3,"1 Minute",2);
		
	while(a==0)			{a=Key_Read();delay_ms(100);}
	
	//��ok
	if(a==1)  
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
	//������
	else if(a==3)		
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
}



//����5
void Function5(void)
{
	u8 a;
	
	OLED_ShowCN(0,0,13);		//��
	OLED_ShowCN(16,0,14);		//��
	OLED_ShowCN(32,0,43);		//��
	OLED_ShowCN(48,0,51);		//��
	

	
	while(a==0)			{a=Key_Read();delay_ms(100);}
	
	//��ok
	if(a==3)  
	{
		Run_BEEP();		OLED_CLS();		a=0;
		
		Show_SendingPage();
		
		//���в��Ժ���
		Data_Call();
		
		Show_OkPage();
		
		MenuCancelOption();
	}
	//������
	else if(a==1)		
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
}


//����6 ��Ӳ���ģ��
void Function6(void)
{
	u8 i=0,a;
	
	u32 Tmp_Num_t; //��ʱ���� �洢�¶ȱ���
	
	OLED_ShowCN(0,0,52);		//��
	OLED_ShowCN(16,0,53);		//��
	OLED_ShowCN(32,0,43);		//��
	OLED_ShowCN(48,0,44);		//��
	OLED_ShowCN(64,0,23);		//ģ
	OLED_ShowCN(80,0,24);		//��
	
	//2��ʾ����
	for(i=0;i<9;i++)
	{
		OLED_ShowChar(i*8+8,4,Tmp_Num[i]+48);		
	}
	
	//3�����ʾ & ��ʾ��ʼλ��
	for(i=0;i<120;i++)		
	{
		OLED_ShowStr(i,6," ",2);		
	}
	OLED_ShowStr((SelectIndexNum*8)+8,6,"^",1);		
	
	while(a==0)			{a=Key_Read();delay_ms(100);}

	//�жϰ���ֵΪ���������ֵ
	while(a==2||a==4||a==5||a==6)
	{
		//���
		if(a==4) 
		{
			Run_BEEP();				
			a=0;					
			Tmp_NumLeftOption();		
			while(a==0)
			{
				a=Key_Read();		
				delay_ms(100);		
			}
		}
		//�Ҽ�
		else if(a==6)
		{
			Run_BEEP();		
			a=0;	
			Tmp_NumRightOption();
			while(a==0)			
			{
				a=Key_Read();		
				delay_ms(100);		
			}
		}
		//�ϼ�
		else if(a==2)
		{
			Run_BEEP();
			a=0;	
			
			//�޶��������� & ѡ������ֵ
			if(Tmp_Num[SelectIndexNum]==9)				Tmp_Num[SelectIndexNum]=0;			
			else										Tmp_Num[SelectIndexNum]++;
			
			//��ʾ��ֵ
			OLED_ShowChar((SelectIndexNum*8+8),4,Tmp_Num[SelectIndexNum]+48);		
			
			while(a==0)			{a=Key_Read();delay_ms(100);}
		}
		//�¼�
		else if(a==5)
		{
			Run_BEEP();		
			a=0;					
			
			if(Tmp_Num[SelectIndexNum]==0)				Tmp_Num[SelectIndexNum]=9;			
			else										Tmp_Num[SelectIndexNum]--;
			
			OLED_ShowChar((SelectIndexNum*8+8),4,Tmp_Num[SelectIndexNum]+48);		

			while(a==0)			{a=Key_Read();delay_ms(100);}
		}
	}
	
	//��ok
	if(a==3)  
	{
		Run_BEEP();		OLED_CLS();		a=0;
		
		Tmp_Num_t=Tmp_Num[0]*100000000+Tmp_Num[1]*10000000+Tmp_Num[2]*1000000+Tmp_Num[3]*100000+Tmp_Num[4]*10000+Tmp_Num[5]*1000+Tmp_Num[6]*100+Tmp_Num[7]*10+Tmp_Num[8];
		
		Sensor_ID_Set(1,Tmp_Num_t);
		
		Show_OkPage();

		MenuCancelOption();
	}
	//������
	else if(a==1)		
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
}


//����6����
void Tmp_NumLeftOption(void)
{
	u8 i=0;
	
	//���������ߵ����� �Ͷ�λ�����ұ�
	if(SelectIndexNum == LevelNum[0])		SelectIndexNum=LevelNum[1];
	else									SelectIndexNum--;
	
	//�����ʾ
	for(i=0;i<128;i++)			OLED_ShowStr(i,6," ",1);		
	
	//ָʾ����λ��
	OLED_ShowStr((SelectIndexNum*8+8),6,"^",1);		
}


//����6����
void Tmp_NumRightOption(void)
{
	u8 i=0;
	
	//��������ұߵ����� �Ͷ�λ������� ��֮��Ȼ
	if(SelectIndexNum == LevelNum[1])		SelectIndexNum=LevelNum[0];
	else									SelectIndexNum++;
			
	//ָʾ����λ��
	for(i=0;i<128;i++)			OLED_ShowStr(i,6," ",1);		
			
	//ָʾ����λ��
	OLED_ShowStr((SelectIndexNum*8+8),6,"^",1);		
}



////����6
//void Function6(void)
//{
//	u8 a;
//	
//	OLED_ShowCN(0,0,0);		//��
//	OLED_ShowCN(16,0,1);	//��

//	OLED_ShowCN(8,4,7);		//��
//	OLED_ShowCN(24,4,8);	//��

//	OLED_ShowCN(56,4,11);	//��
//	OLED_ShowCN(72,4,12);	//Ϣ
//	
//	if(Up==0) 
//	{
//		OLED_ShowStr(0,4,">",2);		
//		OLED_ShowStr(48,4," ",2);	
//	}
//	else if(Up==1)
//	{
//		OLED_ShowStr(48,4,">",2);		
//		OLED_ShowStr(0,4," ",2);		
//	}
//	
//	while(a==0)			{a=Key_Read();delay_ms(100);}

//	while(a==4||a==6)
//	{
//		if(a==4) 
//		{
//			Run_BEEP();		a=0;	
//	
//			if(Up==0) 
//			{
//				Up=1;
//				OLED_ShowStr(48,4,">",2);		
//				OLED_ShowStr(0,4," ",2);		
//			}
//			else if(Up==1) 				
//			{
//				Up=0;
//				OLED_ShowStr(0,4,">",2);	
//				OLED_ShowStr(48,4," ",2);	
//			}
//		
//			while(a==0)			{a=Key_Read();delay_ms(100);}
//		}
//		else if(a==6)
//		{
//			Run_BEEP();		a=0;	//������λ
//		
//			if(Up==0) 
//			{
//				Up=1;
//				OLED_ShowStr(48,4,">",2);		
//				OLED_ShowStr(0,4," ",2);	
//			}
//			
//			else if(Up==1) 			
//			{
//				Up=0;
//				OLED_ShowStr(0,4,">",2);		
//				OLED_ShowStr(48,4," ",2);		
//			}
//				
//			while(a==0)			{a=Key_Read();delay_ms(100);}
//		}
//	}
//		
//	//��ok
//	if(a==1)  
//	{
//		Run_BEEP();		OLED_CLS();		a=0;
//		MenuCancelOption();
//	}
//	//������
//	else if(a==3)		
//	{
//		Run_BEEP();		OLED_CLS();		a=0;
//		MenuCancelOption();
//	}
//}


//����7
void Function7(void)
{
	u8 i, a=0;
	
	u32 Vib_Num_t; //��ʱ���� �洢�¶ȱ���

	OLED_ShowCN(0,0,52);		//��
	OLED_ShowCN(16,0,53);		//��
	OLED_ShowCN(32,0,21);		//ˮ
	OLED_ShowCN(48,0,22);		//λ
	OLED_ShowCN(64,0,23);		//ģ
	OLED_ShowCN(80,0,24);		//��
	

	//2��ʾ����
	for(i=0;i<9;i++)
	{
		OLED_ShowChar(i*8+8,4,Vib_Num[i]+48);		
	}
	
	//3�����ʾ & ��ʾ��ʼλ��
	for(i=0;i<120;i++)		
	{
		OLED_ShowStr(i,6," ",2);		
	}
	OLED_ShowStr((SelectIndexNum*8)+8,6,"^",1);		
	
	while(a==0)			{a=Key_Read();delay_ms(100);}
	
	//�жϰ���ֵΪ���������ֵ
	while(a==2||a==4||a==5||a==6)
	{
		//���
		if(a==4) 
		{
			Run_BEEP();				
			a=0;					
			Vib_NumLeftOption();		
			while(a==0)
			{
				a=Key_Read();		
				delay_ms(100);		
			}
		}
		//�Ҽ�
		else if(a==6)
		{
			Run_BEEP();		
			a=0;	
			Vib_NumRightOption();
			while(a==0)			
			{
				a=Key_Read();		
				delay_ms(100);		
			}
		}
		//�ϼ�
		else if(a==2)
		{
			Run_BEEP();
			a=0;	
			
			//�޶��������� & ѡ������ֵ
			if(Vib_Num[SelectIndexNum]==9)				Vib_Num[SelectIndexNum]=0;			
			else									Vib_Num[SelectIndexNum]++;
			
			//��ʾ��ֵ
			OLED_ShowChar((SelectIndexNum*8+8),4,Vib_Num[SelectIndexNum]+48);		
			
			while(a==0)			{a=Key_Read();delay_ms(100);}
		}
		//�¼�
		else if(a==5)
		{
			Run_BEEP();		
			a=0;					
			
			if(Vib_Num[SelectIndexNum]==0)				Vib_Num[SelectIndexNum]=9;			
			else									Vib_Num[SelectIndexNum]--;
			
			OLED_ShowChar((SelectIndexNum*8+8),4,Vib_Num[SelectIndexNum]+48);		

			while(a==0)			{a=Key_Read();delay_ms(100);}
		}
	}

	//��ok
	if(a==3)  
	{
		Run_BEEP();		OLED_CLS();		a=0;
		
		Vib_Num_t=Vib_Num[0]*100000000+Vib_Num[1]*10000000+Vib_Num[2]*1000000+Vib_Num[3]*100000+Vib_Num[4]*10000+Vib_Num[5]*1000+Vib_Num[6]*100+Vib_Num[7]*10+Vib_Num[8];

		Sensor_ID_Set(2,Vib_Num_t);
		
		Show_OkPage();
		
		MenuCancelOption();
	}
	
	//������
	else if(a==1)		
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
}


//����7����
void Vib_NumLeftOption(void)
{
	u8 i=0;
	
	//���������ߵ����� �Ͷ�λ�����ұ�
	if(SelectIndexNum == LevelNum[0])		SelectIndexNum=LevelNum[1];
	else									SelectIndexNum--;
	
	//�����ʾ
	for(i=0;i<128;i++)			OLED_ShowStr(i,6," ",1);		
	
	//ָʾ����λ��
	OLED_ShowStr((SelectIndexNum*8+8),6,"^",1);		
}


//����7����
void Vib_NumRightOption(void)
{
	u8 i=0;
	
	//��������ұߵ����� �Ͷ�λ������� ��֮��Ȼ
	if(SelectIndexNum == LevelNum[1])		SelectIndexNum=LevelNum[0];
	else									SelectIndexNum++;
			
	//ָʾ����λ��
	for(i=0;i<128;i++)			OLED_ShowStr(i,6," ",1);		
			
	//ָʾ����λ��
	OLED_ShowStr((SelectIndexNum*8+8),6,"^",1);		
}


//����8
void Function8(void)
{
	u8 a=0;
	
	OLED_ShowCN(0,0,25);		//��
	OLED_ShowCN(16,0,26);		//��
	OLED_ShowCN(32,0,11);		//��
	OLED_ShowCN(48,0,12);		//Ϣ

	OLED_ShowStr(0,3,"NUMBER: V1.0.0",1);//����6*8�ַ�,����ʾ7��,ÿ�в�����20�ַ�
	OLED_ShowStr(0,5,"DATA:   20190910",1);//����6*8�ַ�,����ʾ7��,ÿ�в�����20�ַ�
	
	while(a==0)			{a=Key_Read();delay_ms(100);}
	
	//��ok
	if(a!=0)  
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
}


//void Function8(void)
//{
//	u8 a=0;
//	OLED_ShowCN(0,0,0);			//��
//	OLED_ShowCN(16,0,1);		//��
//	OLED_ShowCN(32,0,23);		//ģ
//	OLED_ShowCN(48,0,24);		//��
//	OLED_ShowCN(64,0,2);		//��
//	OLED_ShowCN(80,0,3);		//��
//	while(a==0)			{a=Key_Read();delay_ms(100);}
//	//��ok
//	if(a!=0)  
//	{
//		Run_BEEP();		OLED_CLS();		a=0;
//		MenuCancelOption();
//	}
//}


//����9
void Function9(void)
{
	u8 a=0;
	
	OLED_ShowCN(0,0,47);		//״
	OLED_ShowCN(16,0,48);		//̬
	OLED_ShowCN(32,0,11);		//��
	OLED_ShowCN(48,0,12);		//Ϣ

	OLED_ShowStr(0,3,"3 online",2);//����6*8�ַ�,����ʾ7��,ÿ�в�����20�ַ�
	
//	//���в��Ժ���
//	void fun(void);
	
	while(a==0)			{a=Key_Read();delay_ms(100);}
	
	//��ok
	if(a!=0)  
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
}















