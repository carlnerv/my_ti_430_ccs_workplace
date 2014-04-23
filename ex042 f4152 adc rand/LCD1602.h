#ifndef _LCD1602_H_
#define _LCD1602_H_

#include <msp430.h>
#include "my430lib.h"


// #define	LCD_Port	_pc
// #define	LCD_PortC	_pcc
// #define	DI			0x01
// #define	CP			0x02

// void LCD_Init(void);
// void SendAByte(unsigned char dat);
// void LCD_Display(unsigned int dat);

#define RS_CLR	P3OUT &= ~BIT5        //RS置低
#define RS_SET	P3OUT |= BIT5         //RS置高

#define RW_CLR	P3OUT &= ~BIT6        //RW置低
#define RW_SET	P3OUT |= BIT6         //RW置高

#define EN_CLR	P3OUT &= ~BIT7        //E置低
#define EN_SET	P3OUT |= BIT7         //E置高

#define DataPort      P2OUT                 //P4口为数据口

void LCD_write_com(const unsigned char com);
void LCD_write_data(const char data);
void LCD_clear(void);
void LCD_init(void);
void LCD_write_str(unsigned char x,unsigned char y,const char *s);
void LCD_write_char(unsigned char x,unsigned char y,const char data);

/* 
	1602屏，(x,y)
	====================================
	==(0,0)(1,0)(2,0)(3,0)......(15,0)==
	==(0,1)(1,1)(2,1)(3,1)......(15,1)==
	====================================
	
	写入32个空格 ~= LCD_clear
	LCD_write_str(0,0,"                ");
	LCD_write_str(0,1,"                ");
 */


#endif
