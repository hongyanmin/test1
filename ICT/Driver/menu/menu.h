#ifndef __MENU_H
#define __MENU_H

#include "sys.h"
#include "key.h"
#include "oled.h"
#include "switch.h"
#include "usart.h"
#include "protocol.h"
#include "show.h"



#define OPTIONMETE 	9           //��ǰѡ�����������һ���ر���ѡ��


void MenuReflash (void);

void Display_OLEDMenu (u8 index,u8 line);
void Display_OLEDLine(u8 index,u8 line);

void MenuUpOneOption (void);
void MenuDownOneOption (void);
void MenuEnterOption (void);
void MenuCancelOption (void);

void Function1(void);
void NumLeftOption(void);
void NumRightOption(void);

void Function2(void);
void ServerLeftOption(void);
void ServerRightOption(void);

void Function3(void);

void Function4(void);
void UpTimeLeftOption(void);
void UpTimeRightOption(void);

void Function5(void);
void SampTimeLeftOption(void);
void SampTimeRightOption(void);

//����¶�ģ�麯��
void Function6(void);
void Tmp_NumLeftOption(void);
void Tmp_NumRightOption(void);


//�����ģ�麯��
void Function7(void);
void Vib_NumLeftOption(void);
void Vib_NumRightOption(void);

void Function8(void);
void Function9(void);



#endif

