#ifndef __LCD_H
#define __LCD_H

#include "bsp/bsp_delay.h"

//----------------- LCD 基本绘图函数 ----------------
void LCD_Fill(u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color); // 填充指定区域颜色
void LCD_Color_Fill(u16 xsta, u16 ysta, u16 xend, u16 yend, u16 *color);
void LCD_DrawPoint(u16 x, u16 y, u16 color);                       // 在指定坐标画一个点
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);      // 画线
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color); // 画矩形
void Draw_Circle(u16 x0, u16 y0, u8 r, u16 color);                 // 画圆

//----------------- LCD 中文显示函数 ----------------
void LCD_ShowChinese(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode);      // 显示任意中文
void LCD_ShowChinese12x12(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode); // 显示12x12中文
void LCD_ShowChinese16x16(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode); // 显示16x16中文
void LCD_ShowChinese24x24(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode); // 显示24x24中文
void LCD_ShowChinese32x32(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode); // 显示32x32中文

//----------------- LCD 字符显示函数 ----------------
void LCD_ShowChar(u16 x, u16 y, u8 num, u16 fc, u16 bc, u8 sizey, u8 mode);        // 显示单个字符
void LCD_ShowString(u16 x, u16 y, const u8 *p, u16 fc, u16 bc, u8 sizey, u8 mode); // 显示字符串
u32 mypow(u8 m, u8 n);                                                             // 计算 m 的 n 次方
void LCD_ShowIntNum(u16 x, u16 y, u16 num, u8 len, u16 fc, u16 bc, u8 sizey);      // 显示整数
void LCD_ShowFloatNum1(u16 x, u16 y, float num, u8 len, u16 fc, u16 bc, u8 sizey); // 显示浮点数

//----------------- LCD 图片显示函数 ----------------
void LCD_ShowPicture(u16 x, u16 y, u16 length, u16 width, const u8 pic[]); // 显示图片

//----------------- LCD 常用颜色 ----------------
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0xF81F
#define GRED 0xFFE0
#define GBLUE 0x07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0xBC40      // 棕色
#define BRRED 0xFC07      // 浅红
#define GRAY 0x8430       // 灰色
#define DARKBLUE 0x01CF   // 深蓝
#define LIGHTBLUE 0x7D7C  // 淡蓝
#define GRAYBLUE 0x5458   // 灰蓝
#define LIGHTGREEN 0x841F // 淡绿
#define LGRAY 0xC618      // 淡灰(PANNEL)，背景灰
#define LGRAYBLUE 0xA651  // 淡蓝灰，中间层灰
#define LBBLUE 0x2B12     // 淡蓝灰，选中项目颜色

#endif
