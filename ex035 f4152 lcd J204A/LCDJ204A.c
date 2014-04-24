#include "LCDJ204A.h"

//***********************************************************************
//	显示屏命令写入函数
//***********************************************************************


void LCD_write_com(const unsigned char com)
{
	RS_CLR;
	RW_CLR;
	EN_SET;
	DataPort = com;                 // 命令写入端口
	// delay_ms(2);					// 根据HD44780手册，和实际电路推测LCD时钟的最差情况
	delay_us(100);					// 本程序的延时基本都为最差情况
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
	DataPort = data;                // 数据写入端口
	// delay_ms(2);
	delay_us(100);
	EN_CLR;
}
//***********************************************************************
//	显示屏清空显示
//***********************************************************************

void LCD_clear(void)
{
	LCD_write_com(0x01);            // 清屏幕显示
	delay_ms(5);
}
//***********************************************************************
//	显示屏字符串写入函数
//***********************************************************************

void LCD_write_str(unsigned char col, unsigned char row, const char *s)
{

    switch ( row )
    {
    	case 0:
    		LCD_write_com(0x80 + col);        			// 第一行显示
    		break;
    	case 1:
    		LCD_write_com(0x80 + 0x40 + col);        	// 第二行显示
    		break;
    	case 2:
    		LCD_write_com(0x80 + 20 + col);        		// 第三行显示
    		break;
    	case 3:
    		LCD_write_com(0x80 + 0x40 + 20 + col);      // 第四行显示
    		break;
    	default:
    		break;
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

void LCD_write_char(unsigned char col, unsigned char row, const char data)
{
	
    switch ( row )
    {
    	case 0:
    		LCD_write_com(0x80 + col);        			// 第一行显示
    		break;
    	case 1:
    		LCD_write_com(0x80 + 0x40 + col);        	// 第二行显示
    		break;
    	case 2:
    		LCD_write_com(0x80 + 20 + col);        		// 第三行显示
    		break;
    	case 3:
    		LCD_write_com(0x80 + 0x40 + 20 + col);      // 第四行显示
    		break;
    	default:
    		break;
    }

    LCD_write_data( data);
}
//***********************************************************************
//	显示屏初始化函数
//***********************************************************************

void LCD_init(void)
{
	delay_ms(100);				// 等待启动，1602启动慢
    LCD_write_com(0x38);		// 显示模式设置
    delay_ms(5);
    LCD_write_com(0x08);		// 显示关闭
    delay_ms(5);
    LCD_write_com(0x01);		// 显示清屏
    delay_ms(5);
    LCD_write_com(0x06);		// 显示光标移动设置
    delay_ms(5);
    LCD_write_com(0x0C);		// 显示开及光标设置
    delay_ms(5);
	LCD_write_com(0x01);		// 显示清屏
	delay_ms(5);
}
