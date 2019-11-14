#include "menu.h"
//#include "protocol.h"


extern u8 key_num;

//菜单结构定义
struct Option
{
    u8 KeyLevel;       //菜单选项所属菜单表号
    u8 EnterIndex;     //选项进入索引号
    u8 CancelIndex;    //选项退出索引号
};

//具体菜单选项定义，定义一个结构数组 存储在编码区，节省内存RAM
struct Option sOption[OPTIONMETE] =
{
	//菜单表号 进入索引 退出索引 
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

/*菜单表分类数组*/  //每层表单对应的开始索引  结束索引  选项数目*/
u8 Level[3] = {0, 8, 9};

/*菜单索引定义*/
u8 FirstLineIndex = 0;    		//屏幕第一行显示的索引号
u8 SelectLine_L = 1;		    //原来选中行
u8 SelectLine = 1;      		//当前选择的行
u8 SelectIndex = 0;     		//当前选定行对应的索引号
u8 LastIndex = 0;      			//进入功能函数前的索引号，判断具体功能使用

//模块编号
u8 ICT_Num[9]={1,9,1,1,0,0,0,0,1};
//温度传感器编号
u8 Tmp_Num[9]={1,9,1,1,1,0,0,0,1};
//位移传感器编号
u8 Vib_Num[9]={1,9,1,1,2,0,0,0,1};


u8 Server[]={1,9,2,1,6,8,0,0,3,0,0,1,8,0,8,6};
u8 Clear=1;
u8 UpTime[]={0,0,1};
u8 SampTime[]={0,0,1};
u8 Up;
u8 Water;



u8 LevelNum[3] = {0, 8, 9};
u8 SelectIndexNum = 0;     		//当前选定行对应的索引号

u8 LevelServer[3]={0,15,16};
u8 SelectIndexServer=0;

u8 LevelUpTime[3]={0,2,3};
u8 SelectIndexUpTime=0;

u8 LevelSampTime[3]={0,2,3};
u8 SelectIndexSampTime=0;




//刷新菜单
void MenuReflash()
{
	u8 a;
	//a=key_num;
	a=Key_Read();
	switch(a)
	{
		case 2:	//上键
		{
			Run_BEEP();					//蜂鸣器
			OLED_CLS();					//清屏		
			a=0;						//按键值清零
			MenuUpOneOption();			//进入上键处理函数
		}break;
		
		case 5:	//下键
		{	
			Run_BEEP();
			OLED_CLS();
			a=0;
			MenuDownOneOption();
		}break;
		
		case 1:	//左键
		{		
			Run_BEEP();
			OLED_CLS();
			a=0;
			MenuCancelOption();
		}break;
		
		case 3:	//右键
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


//刷新显示
void Display_OLEDMenu (unsigned char index,unsigned char line)
{
	//显示菜单的4行
	u8 i;
	for(i=0;i<4;i++)
	{
		Display_OLEDLine(index+i, i+1);
	}
	
	//显示符号位置
	OLED_ShowStr(8,(line-1)*2,">",2);		
}


//显示某一行
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


//1  菜单上移一栏函数
void MenuUpOneOption (void)
{
	//如果当前为表单第一项
	if(SelectIndex == Level[0])    					
    {
		FirstLineIndex = Level[1] - 3; 		//第一行显示索引号为倒数第四项
		SelectIndex = Level[1]; 
		SelectLine_L = SelectLine;			//记录原来行 为第一行
		SelectLine = 4;             		//标记选择行为第四行
    }
		//如果当前不是开始索引
    else        				
    {
		//并且已经在屏幕最上边一行
        if(SelectLine == 1)  								
        {
            FirstLineIndex--;				//显示索引上移
            SelectIndex--;					//选择索引自减
			SelectLine_L = SelectLine;		//记录原来行
            SelectLine = 1;					//选择行还是第一行
        }
		//如果不是第一行
        else       									
        {
			SelectLine_L = SelectLine;		//记录原来行
            SelectLine--;					//选择行自减
            SelectIndex--;					//选择索引自减
        }
    }
		
	//刷新屏幕显示
	Display_OLEDMenu(FirstLineIndex,SelectLine); 		
}


//2  菜单下移一项函数
void MenuDownOneOption (void)
{
	//如果是最后的索引
	if(SelectIndex == Level[1])
    {
        FirstLineIndex = Level[0];			//第一行显示索引为表单第一个选项
        SelectIndex = Level[0];				//选择索引为表单第一个选项索引
		SelectLine_L = SelectLine;			//记录原来行
		SelectLine = 1;						//选择行为第一行
    }
	//如果不是最后的索引
    else           
    {
		//如果当前不是屏幕最底行
        if(SelectLine != 4)     						
        {
			SelectIndex++;					//选择索引自加
            SelectLine_L = SelectLine;		//记录原来行
			SelectLine++;					//选择行下移
        }
		//如果是屏幕最低行
        else          											
        {
            FirstLineIndex++;				//第一行显示下移
            SelectIndex++;					//选择索引自加
        }
    }
		
	Display_OLEDMenu(FirstLineIndex,SelectLine);
}


//3  进入某项功能函数
void MenuEnterOption (void)
{
	//标记进入前的索引号（以便判断具体功能）
    LastIndex = SelectIndex;           
		
	//更新选择索引为之前索引号对应进入索引
    SelectIndex = sOption[SelectIndex].EnterIndex;     
		
	//根据进入前的索引判断具体函数
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

//4  菜单退出功能函数
void MenuCancelOption (void)
{
	//如果不是从功能返回
	if(SelectIndex != OPTIONMETE - 1)        
	{
		SelectIndex = sOption[SelectIndex].CancelIndex;     //选择索引为选项返回索引
	}
	//如果是从功能返回
	else                
	{
		SelectIndex = LastIndex;         			 		//索引等于进入前保存索引
	}

	//根据返回选项确定显示首项
	if(SelectIndex > Level[1] - 3) 
	{
		FirstLineIndex = Level[1] - 3;
		SelectLine_L = SelectLine;
		SelectLine =  4 - (Level[1] - SelectIndex);
	}
	//一般显示方式
	else               
	{
		FirstLineIndex = SelectIndex;       				//第一行显示索引
		SelectLine_L = SelectLine;
		SelectLine = 1;            							//选择第一行
	}
	
	Display_OLEDMenu(FirstLineIndex,SelectLine);
}


//函数1
void Function1(void)
{
	u8 i,a=0;
	
	//1显示中文
	OLED_ShowCN(8,0,25);		//设
	OLED_ShowCN(24,0,26);		//备
	OLED_ShowCN(40,0,2);		//信
	OLED_ShowCN(56,0,3);		//息

	//2显示内容
	for(i=0;i<9;i++)
	{
		OLED_ShowChar(i*8+8,4,ICT_Num[i]+48);		
	}
	
	//3清除显示 & 显示起始位置
	for(i=0;i<120;i++)		
	{
		OLED_ShowStr(i,6," ",2);		
	}
	OLED_ShowStr((SelectIndexNum*8)+8,6,"^",1);		

	//读取按键值
	while(a==0)			
	{
		a=Key_Read();					//读取函数
		delay_ms(100);					//消抖
	}
	
	//判断按键值为具体操作健值
	while(a==2||a==4||a==5||a==6)
	{
		//左键
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
		//右键
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
		//上键
		else if(a==2)
		{
			Run_BEEP();
			a=0;	
			
			//限定数字区间 & 选中数组值
			if(ICT_Num[SelectIndexNum]==9)				ICT_Num[SelectIndexNum]=0;			
			else									ICT_Num[SelectIndexNum]++;
			
			//显示数值
			OLED_ShowChar((SelectIndexNum*8+8),4,ICT_Num[SelectIndexNum]+48);		
			
			while(a==0)			{a=Key_Read();delay_ms(100);}
		}
		//下键
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
	
	//按ok
	if(a==1)  
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
	//按返回
	else if(a==3)		
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
}


//函数1左移
void NumLeftOption(void)
{
	u8 i=0;
	
	//如果是最左边的数字 就定位到最右边
	if(SelectIndexNum == LevelNum[0])		SelectIndexNum=LevelNum[1];
	else									SelectIndexNum--;
	
	//清除显示
	for(i=0;i<128;i++)			OLED_ShowStr(i,6," ",1);		
	
	//指示符号位置
	OLED_ShowStr((SelectIndexNum*8+8),6,"^",1);		
}


//函数1右移
void NumRightOption(void)
{
	u8 i=0;
	
	//如果是最右边的数字 就定位到最左边 反之亦然
	if(SelectIndexNum == LevelNum[1])		SelectIndexNum=LevelNum[0];
	else									SelectIndexNum++;
			
	//指示符号位置
	for(i=0;i<128;i++)			OLED_ShowStr(i,6," ",1);		
			
	//指示符号位置
	OLED_ShowStr((SelectIndexNum*8+8),6,"^",1);		
}


//函数2
void Function2(void)
{
	u8 i,k=0;
	u8 a=0;

	//指示符号位置
	OLED_ShowStr(0,0,"IP:",1);		
	OLED_ShowStr(0,4,"PORT:",1);		

	//显示IP
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
		
	//显示端口号
	for(i=16;i<20;i++)
	{
		OLED_ShowChar((i-16)*8,5,Server[k]+48);		
		k++;
	}
	
	while(a==0)			{a=Key_Read();delay_ms(100);}

	//按ok
	if(a==1)  
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
	//按返回
	else if(a==3)		
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
}


////函数2 //设置网络和端口 暂时不用
//void Function2(void)
//{
//	u8 i,k=0;
//	u8 a=0,n;

//	//指示符号位置
//	OLED_ShowStr(0,0,"IP:",1);		
//	OLED_ShowStr(0,4,"PORT:",1);		

//	//显示IP
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
//	//显示端口号
//	for(i=16;i<20;i++)
//	{
//		OLED_ShowChar((i-16)*8,5,Server[k]+48);		
//		k++;
//	}
//	
//	//清除显示 & 显示起始位置
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
//			//限定数字区间
//			if(Server[SelectIndexServer]==9)			Server[SelectIndexServer]=0;			
//			else										Server[SelectIndexServer]++;
//		
//			//显示IP&Port
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
//			//限定数字区间		
//			if(Server[SelectIndexServer]==0)		Server[SelectIndexServer]=9;			
//			else									Server[SelectIndexServer]--;
//			
//			//显示IP&Port
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
//	//按ok
//	if(a==1)  
//	{
//		Run_BEEP();		OLED_CLS();		a=0;
//		MenuCancelOption();
//	}
//	//按返回
//	else if(a==3)		
//	{
//		Run_BEEP();		OLED_CLS();		a=0;
//		MenuCancelOption();
//	}
//}


////函数2左移
//void ServerLeftOption(void)
//{
//	u8 i=0;
//	//如果是最左边的数字 就定位到最右边
//	if(SelectIndexServer == LevelServer[0])		SelectIndexServer=LevelServer[1];
//	else										SelectIndexServer--;
//		
//	//设置为120 防止溢出 清除了其它地方
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


////函数2右移
//void ServerRightOption(void)
//{			
//	u8 i=0;
//	
//	//如果是最右边的数字 就定位到最左边
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


//函数3
void Function3(void)
{
	u8 a;
	
	OLED_ShowCN(0,0,13);	//上0
	OLED_ShowCN(16,0,14);	//传16
	OLED_ShowCN(32,0,15);	//时32
	OLED_ShowCN(48,0,16);	//间48
	OLED_ShowCN(64,0,17);	//间
	OLED_ShowCN(80,0,18);	//隔

	OLED_ShowStr(0,3,"15 Minute",2);

	while(a==0)			{a=Key_Read();delay_ms(100);}
		
	//按ok
	if(a==1)  
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
	//按返回
	else if(a==3)		
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
}


//暂时不用
////函数3
//void Function3(void)
//{
//	u8 a;
//	
//	OLED_ShowCN(0,0,13);	//上0
//	OLED_ShowCN(16,0,14);	//传16
//	OLED_ShowCN(32,0,15);	//时32
//	OLED_ShowCN(48,0,16);	//间48
//	OLED_ShowCN(64,0,17);	//间
//	OLED_ShowCN(80,0,18);	//隔

//	OLED_ShowCN(8,4,7);		//清
//	OLED_ShowCN(24,4,8);	//除

//	OLED_ShowCN(56,4,11);	//信
//	OLED_ShowCN(72,4,12);	//息
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
//	//按ok
//	if(a==1)  
//	{
//		Run_BEEP();		OLED_CLS();		a=0;
//		MenuCancelOption();
//	}
//	//按返回
//	else if(a==3)		
//	{
//		Run_BEEP();		OLED_CLS();		a=0;
//		MenuCancelOption();
//	}
//}


//函数4
void Function4(void)
{
	u8 a;
	OLED_ShowCN(0,0,19);		//采
	OLED_ShowCN(16,0,20);		//样
	OLED_ShowCN(32,0,15);		//时
	OLED_ShowCN(48,0,16);		//间
	OLED_ShowCN(64,0,17);		//间
	OLED_ShowCN(80,0,18);		//隔
	
	OLED_ShowStr(0,3,"1 Minute",2);
		
	while(a==0)			{a=Key_Read();delay_ms(100);}
	
	//按ok
	if(a==1)  
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
	//按返回
	else if(a==3)		
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
}



//函数5
void Function5(void)
{
	u8 a;
	
	OLED_ShowCN(0,0,13);		//上
	OLED_ShowCN(16,0,14);		//传
	OLED_ShowCN(32,0,43);		//测
	OLED_ShowCN(48,0,51);		//试
	

	
	while(a==0)			{a=Key_Read();delay_ms(100);}
	
	//按ok
	if(a==3)  
	{
		Run_BEEP();		OLED_CLS();		a=0;
		
		Show_SendingPage();
		
		//进行测试函数
		Data_Call();
		
		Show_OkPage();
		
		MenuCancelOption();
	}
	//按返回
	else if(a==1)		
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
}


//函数6 添加测温模块
void Function6(void)
{
	u8 i=0,a;
	
	u32 Tmp_Num_t; //临时变量 存储温度编码
	
	OLED_ShowCN(0,0,52);		//添
	OLED_ShowCN(16,0,53);		//加
	OLED_ShowCN(32,0,43);		//测
	OLED_ShowCN(48,0,44);		//温
	OLED_ShowCN(64,0,23);		//模
	OLED_ShowCN(80,0,24);		//块
	
	//2显示内容
	for(i=0;i<9;i++)
	{
		OLED_ShowChar(i*8+8,4,Tmp_Num[i]+48);		
	}
	
	//3清除显示 & 显示起始位置
	for(i=0;i<120;i++)		
	{
		OLED_ShowStr(i,6," ",2);		
	}
	OLED_ShowStr((SelectIndexNum*8)+8,6,"^",1);		
	
	while(a==0)			{a=Key_Read();delay_ms(100);}

	//判断按键值为具体操作健值
	while(a==2||a==4||a==5||a==6)
	{
		//左键
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
		//右键
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
		//上键
		else if(a==2)
		{
			Run_BEEP();
			a=0;	
			
			//限定数字区间 & 选中数组值
			if(Tmp_Num[SelectIndexNum]==9)				Tmp_Num[SelectIndexNum]=0;			
			else										Tmp_Num[SelectIndexNum]++;
			
			//显示数值
			OLED_ShowChar((SelectIndexNum*8+8),4,Tmp_Num[SelectIndexNum]+48);		
			
			while(a==0)			{a=Key_Read();delay_ms(100);}
		}
		//下键
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
	
	//按ok
	if(a==3)  
	{
		Run_BEEP();		OLED_CLS();		a=0;
		
		Tmp_Num_t=Tmp_Num[0]*100000000+Tmp_Num[1]*10000000+Tmp_Num[2]*1000000+Tmp_Num[3]*100000+Tmp_Num[4]*10000+Tmp_Num[5]*1000+Tmp_Num[6]*100+Tmp_Num[7]*10+Tmp_Num[8];
		
		Sensor_ID_Set(1,Tmp_Num_t);
		
		Show_OkPage();

		MenuCancelOption();
	}
	//按返回
	else if(a==1)		
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
}


//函数6左移
void Tmp_NumLeftOption(void)
{
	u8 i=0;
	
	//如果是最左边的数字 就定位到最右边
	if(SelectIndexNum == LevelNum[0])		SelectIndexNum=LevelNum[1];
	else									SelectIndexNum--;
	
	//清除显示
	for(i=0;i<128;i++)			OLED_ShowStr(i,6," ",1);		
	
	//指示符号位置
	OLED_ShowStr((SelectIndexNum*8+8),6,"^",1);		
}


//函数6右移
void Tmp_NumRightOption(void)
{
	u8 i=0;
	
	//如果是最右边的数字 就定位到最左边 反之亦然
	if(SelectIndexNum == LevelNum[1])		SelectIndexNum=LevelNum[0];
	else									SelectIndexNum++;
			
	//指示符号位置
	for(i=0;i<128;i++)			OLED_ShowStr(i,6," ",1);		
			
	//指示符号位置
	OLED_ShowStr((SelectIndexNum*8+8),6,"^",1);		
}



////函数6
//void Function6(void)
//{
//	u8 a;
//	
//	OLED_ShowCN(0,0,0);		//设
//	OLED_ShowCN(16,0,1);	//置

//	OLED_ShowCN(8,4,7);		//清
//	OLED_ShowCN(24,4,8);	//除

//	OLED_ShowCN(56,4,11);	//信
//	OLED_ShowCN(72,4,12);	//息
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
//			Run_BEEP();		a=0;	//按键复位
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
//	//按ok
//	if(a==1)  
//	{
//		Run_BEEP();		OLED_CLS();		a=0;
//		MenuCancelOption();
//	}
//	//按返回
//	else if(a==3)		
//	{
//		Run_BEEP();		OLED_CLS();		a=0;
//		MenuCancelOption();
//	}
//}


//函数7
void Function7(void)
{
	u8 i, a=0;
	
	u32 Vib_Num_t; //临时变量 存储温度编码

	OLED_ShowCN(0,0,52);		//添
	OLED_ShowCN(16,0,53);		//加
	OLED_ShowCN(32,0,21);		//水
	OLED_ShowCN(48,0,22);		//位
	OLED_ShowCN(64,0,23);		//模
	OLED_ShowCN(80,0,24);		//块
	

	//2显示内容
	for(i=0;i<9;i++)
	{
		OLED_ShowChar(i*8+8,4,Vib_Num[i]+48);		
	}
	
	//3清除显示 & 显示起始位置
	for(i=0;i<120;i++)		
	{
		OLED_ShowStr(i,6," ",2);		
	}
	OLED_ShowStr((SelectIndexNum*8)+8,6,"^",1);		
	
	while(a==0)			{a=Key_Read();delay_ms(100);}
	
	//判断按键值为具体操作健值
	while(a==2||a==4||a==5||a==6)
	{
		//左键
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
		//右键
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
		//上键
		else if(a==2)
		{
			Run_BEEP();
			a=0;	
			
			//限定数字区间 & 选中数组值
			if(Vib_Num[SelectIndexNum]==9)				Vib_Num[SelectIndexNum]=0;			
			else									Vib_Num[SelectIndexNum]++;
			
			//显示数值
			OLED_ShowChar((SelectIndexNum*8+8),4,Vib_Num[SelectIndexNum]+48);		
			
			while(a==0)			{a=Key_Read();delay_ms(100);}
		}
		//下键
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

	//按ok
	if(a==3)  
	{
		Run_BEEP();		OLED_CLS();		a=0;
		
		Vib_Num_t=Vib_Num[0]*100000000+Vib_Num[1]*10000000+Vib_Num[2]*1000000+Vib_Num[3]*100000+Vib_Num[4]*10000+Vib_Num[5]*1000+Vib_Num[6]*100+Vib_Num[7]*10+Vib_Num[8];

		Sensor_ID_Set(2,Vib_Num_t);
		
		Show_OkPage();
		
		MenuCancelOption();
	}
	
	//按返回
	else if(a==1)		
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
}


//函数7左移
void Vib_NumLeftOption(void)
{
	u8 i=0;
	
	//如果是最左边的数字 就定位到最右边
	if(SelectIndexNum == LevelNum[0])		SelectIndexNum=LevelNum[1];
	else									SelectIndexNum--;
	
	//清除显示
	for(i=0;i<128;i++)			OLED_ShowStr(i,6," ",1);		
	
	//指示符号位置
	OLED_ShowStr((SelectIndexNum*8+8),6,"^",1);		
}


//函数7右移
void Vib_NumRightOption(void)
{
	u8 i=0;
	
	//如果是最右边的数字 就定位到最左边 反之亦然
	if(SelectIndexNum == LevelNum[1])		SelectIndexNum=LevelNum[0];
	else									SelectIndexNum++;
			
	//指示符号位置
	for(i=0;i<128;i++)			OLED_ShowStr(i,6," ",1);		
			
	//指示符号位置
	OLED_ShowStr((SelectIndexNum*8+8),6,"^",1);		
}


//函数8
void Function8(void)
{
	u8 a=0;
	
	OLED_ShowCN(0,0,25);		//设
	OLED_ShowCN(16,0,26);		//备
	OLED_ShowCN(32,0,11);		//信
	OLED_ShowCN(48,0,12);		//息

	OLED_ShowStr(0,3,"NUMBER: V1.0.0",1);//测试6*8字符,可显示7行,每行不超过20字符
	OLED_ShowStr(0,5,"DATA:   20190910",1);//测试6*8字符,可显示7行,每行不超过20字符
	
	while(a==0)			{a=Key_Read();delay_ms(100);}
	
	//按ok
	if(a!=0)  
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
}


//void Function8(void)
//{
//	u8 a=0;
//	OLED_ShowCN(0,0,0);			//设
//	OLED_ShowCN(16,0,1);		//置
//	OLED_ShowCN(32,0,23);		//模
//	OLED_ShowCN(48,0,24);		//块
//	OLED_ShowCN(64,0,2);		//编
//	OLED_ShowCN(80,0,3);		//号
//	while(a==0)			{a=Key_Read();delay_ms(100);}
//	//按ok
//	if(a!=0)  
//	{
//		Run_BEEP();		OLED_CLS();		a=0;
//		MenuCancelOption();
//	}
//}


//函数9
void Function9(void)
{
	u8 a=0;
	
	OLED_ShowCN(0,0,47);		//状
	OLED_ShowCN(16,0,48);		//态
	OLED_ShowCN(32,0,11);		//信
	OLED_ShowCN(48,0,12);		//息

	OLED_ShowStr(0,3,"3 online",2);//测试6*8字符,可显示7行,每行不超过20字符
	
//	//进行测试函数
//	void fun(void);
	
	while(a==0)			{a=Key_Read();delay_ms(100);}
	
	//按ok
	if(a!=0)  
	{
		Run_BEEP();		OLED_CLS();		a=0;
		MenuCancelOption();
	}
}















