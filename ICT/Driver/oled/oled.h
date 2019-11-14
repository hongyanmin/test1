#ifndef __OLED_H
#define	__OLED_H

#include "sys.h"  


//PB5 -- key
#define OLED_KEY_PORT 		GPIOB
#define OLED_KEY_PIN 			GPIO_Pin_5
#define OLED_KEY_CLK			RCC_AHBPeriph_GPIOB
#define OLED_KEY_SOURCE		GPIO_PinSource5

//PB6 -- SCL
#define OLED_SCL_PORT 		GPIOB
#define OLED_SCL_PIN 			GPIO_Pin_6
#define OLED_SCL_CLK			RCC_AHBPeriph_GPIOB
#define OLED_SCL_SOURCE		GPIO_PinSource6
//PB7 -- SDA
#define OLED_SDA_PORT 		GPIOB
#define OLED_SDA_PIN 			GPIO_Pin_7
#define OLED_SDA_CLK			RCC_AHBPeriph_GPIOB
#define OLED_SDA_SOURCE		GPIO_PinSource7

#define OLED_ADDRESS	0x78 

void OLED_Init(void);


void I2C_OLED_KEY(int status);
void I2C_WriteByte(uint8_t addr,uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);


//void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr);
//void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char size,unsigned char mode);
//void OLED_DrawPoint(unsigned char x,unsigned char y,unsigned char t);
void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char chr);
void OLED_ShowChar_Small(unsigned char x, unsigned char y, unsigned char chr);
//void OLED_ShowChar_Small(u8 x,u8 y,u8 chr,u8 size,u8 mode);
//void OLED_DrawPoint(u8 x,u8 y,u8 t);


void OLED_Page1(void);
void OLED_Page2(void);
void OLED_Page3(void);
void OLED_Line1(unsigned char LINE);
void OLED_Line2(unsigned char LINE);
void OLED_Line3(unsigned char LINE);
void OLED_Line4(unsigned char LINE);
void OLED_Line5(unsigned char LINE);
void OLED_Line6(unsigned char LINE);
void OLED_Line7(unsigned char LINE);
void OLED_Line8(unsigned char LINE);
void OLED_Line9(unsigned char LINE);



#endif

