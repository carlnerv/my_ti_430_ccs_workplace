#include "LCD1602.h"

//***********************************************************************
//	显示屏命令写入函数
//***********************************************************************


void LCD_write_com(const unsigned char com)
{
	RS_CLR;
	RW_CLR;
	EN_SET;
	DataPort = com;                 //命令写入端口
	// delay_ms(2);
	delay_us(100);
	EN_CLR;
}
//***********************************************************************
//	显示屏数据写入函数
//***********************************************************************

void LCD_write_data(const char data)
{
	RS_SET;
	RW_CLR;
	EN_SET;
	DataPort = data;                //数据写入端口
	// delay_ms(2);
	delay_us(100);
	EN_CLR;
}
//***********************************************************************
//	显示屏清空显示
//***********************************************************************

void LCD_clear(void)
{
	LCD_write_com(0x01);            //清屏幕显示
	delay_ms(5);
	// delay_us(5);
}
//***********************************************************************
//	显示屏字符串写入函数
//***********************************************************************

void LCD_write_str(unsigned char x,unsigned char y,const char *s)
{

    if (y == 0)
    {
    	LCD_write_com(0x80 + x);        //第一行显示
    }
    else
    {
    	LCD_write_com(0xC0 + x);        //第二行显示
    }

    while (*s)
    {
    	LCD_write_data( *s);
    	s ++;
    }
}
//***********************************************************************
//	显示屏单字符写入函数
//***********************************************************************

void LCD_write_char(unsigned char x,unsigned char y,const char data)
{
	
    if (y == 0)
    {
    	LCD_write_com(0x80 + x);        //第一行显示
    }
    else
    {
    	LCD_write_com(0xC0 + x);        //第二行显示
    }

    LCD_write_data( data);
}
//***********************************************************************
//	显示屏初始化函数
//***********************************************************************

void LCD_init(void)
{
	delay_ms(100);				// 等待启动，1602启动慢
    LCD_write_com(0x38);		//显示模式设置
    delay_ms(5);
    LCD_write_com(0x08);		//显示关闭
    delay_ms(5);
    LCD_write_com(0x01);		//显示清屏
    delay_ms(5);
    LCD_write_com(0x06);		//显示光标移动设置
    delay_ms(5);
    LCD_write_com(0x0C);		//显示开及光标设置
    delay_ms(5);
    // LCD_write_str(0,0,"init01");
	// LCD_write_str(0,1,"init02");
	LCD_write_com(0x01);		//显示清屏
	delay_ms(5);
}
