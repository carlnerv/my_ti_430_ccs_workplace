#ifndef _LCDJ204A_H_
#define _LCDJ204A_H_

#include <msp430.h>

/*	比较精准的延时
 * 	使用时修改CPU_F即可，单位Hz
 *
 * 	如果不用延时，用定时器配合中断，是不是可以伪并行处理？
 */

#define CPU_F ((double)1000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

#define RS_CLR	P3OUT &= ~BIT5			//RS置低
#define RS_SET	P3OUT |= BIT5			//RS置高

#define RW_CLR	P3OUT &= ~BIT6			//RW置低
#define RW_SET	P3OUT |= BIT6			//RW置高

#define EN_CLR	P3OUT &= ~BIT7			//E置低
#define EN_SET	P3OUT |= BIT7			//E置高

#define DataPort      P2OUT				//P2口为数据口

void LCD_write_com(const unsigned char com);
void LCD_write_data(const char data);
void LCD_clear(void);
void LCD_init(void);
void LCD_write_str(unsigned char col, unsigned char row, const char *s);
void LCD_write_char(unsigned char col, unsigned char row, const char data);

/* 
	1602屏，(col,row)
	====================================
	==(0,0)(1,0)(2,0)(3,0)......(15,0)==
	==(0,1)(1,1)(2,1)(3,1)......(15,1)==
	====================================
	J204A	(col,row)
	=======================================
	==(0,0)(1,0)(2,0)(3,0).........(19,0)==
	==(0,1)(1,1)(2,1)(3,1).........(19,1)==
	==(0,2)(1,2)(2,2)(3,2).........(19,2)==
	==(0,3)(1,3)(2,3)(3,3).........(19,3)==
	=======================================
	or
	=======================================
	==(0,0)(1,0)(2,0)(3,0).........(19,0)==
	==(0,1)(1,1)(2,1)(3,1).........(19,1)==
	==(20,1)(21,1)(22,1)(23,1).........(39,1)==
	==(20,2)(21,2)(22,2)(23,2).........(39,2)==
	=======================================
	写入32个空格 ~= LCD_clear
	LCD_write_str(0,0,"                ");
	LCD_write_str(0,1,"                ");

// *******************************************************************
	以下摘自 单片机控制2004A液晶屏之模块化编程 刺客_阿瑞 CSDN
	http://blog.csdn.net/yagnruinihao/article/details/19999551

	经过测试J204A(也称2004A,每行20字符，显示4行)液晶屏和1602液晶屏的
    使用方法几乎是一样的，不过也是有区别的，在1602液晶屏中，用来寄存待显
    示字符代码的DDRAM共有80个字节，而J204A能够一次性显示的字符是80字
    节，这里不是巧合，即J204A只是将LCD1602的DDRAM的80个字符全部显示出
    来而已。

    第一行首地址  :0x80
    第二行首地址  :0x80+0x40
    第三行首地址  :0x80+20
    第四行首地址  :0x80+0x40+20

    可以注意到，第一行和第三行、第二行和第四行的地址是分别连续的。 经过
    实验验证，可以得出这样的结论:J204A是将LCD1602的DDRAM的0x00~0x13地址
    的数据映射显示在J204A的第一行，LCD1602的DDRAM的0x14~0x27地址的数据映
    射显示在第三行 ； LCD1602的DDRAM的0x40~0x53地址的数据映射显示在J204A
    的第二行，将LCD1602的DDRAM的0x54~0x67地址的数据映射显示在J204A的第四行。

// ----------------------------------------------------------------------------
    J204A的程序和LCD1602的程序很相似，这里的程序是由LCD1602程序修改而来。
// ----------------------------------------------------------------------------


 */


#endif