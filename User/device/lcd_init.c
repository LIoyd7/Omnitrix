#include "lcd_init.h"
#include "bsp/bsp_delay.h"
#include "spi.h"
#include "tim.h"

/******************************************************************************
  函数说明: LCD 引脚初始化
  输入参数: 无
  返回值  : 无
******************************************************************************/
void LCD_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	// 初始化 RES (PB6), CS (PB4)
	GPIO_InitStructure.Pin = RES_PIN | CS_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	// 初始化 DC (PA15)
	GPIO_InitStructure.Pin = DC_PIN;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	// 初始状态: RES高, CS高, DC高
	HAL_GPIO_WritePin(RES_PORT, RES_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_SET);
}

/******************************************************************************
  函数说明: LCD 数据写入 (硬件 SPI)
  输入参数: dat 要写入的数据
  返回值  : 无
  说明    : 在传输前后自动控制 CS 片选
******************************************************************************/
void LCD_Writ_Bus(uint8_t dat)
{
	LCD_CS_Clr();						  // 拉低 CS 开始传输
	HAL_SPI_Transmit(&hspi1, &dat, 1, 1); // 发送一个字节
	LCD_CS_Set();						  // 拉高 CS 结束传输
}

/******************************************************************************
  函数说明: LCD 写 8 位数据
  输入参数: dat 写入的数据
  返回值  : 无
******************************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
	LCD_Writ_Bus(dat);
}

/******************************************************************************
  函数说明: LCD 写 16 位数据 (大端: 高字节在前)
  输入参数: dat 写入的数据
  返回值  : 无
******************************************************************************/
void LCD_WR_DATA(uint16_t dat)
{
	LCD_Writ_Bus(dat >> 8); // 高字节
	LCD_Writ_Bus(dat);		// 低字节
}

/******************************************************************************
  函数说明: LCD 写寄存器
  输入参数: dat 写入的数据
  返回值  : 无
******************************************************************************/
void LCD_WR_REG(uint8_t dat)
{
	LCD_DC_Clr(); // 写寄存器
	LCD_Writ_Bus(dat);
	LCD_DC_Set();
}

/******************************************************************************
  函数说明: 设置 LCD 显示区域
  输入参数: x1,x2 起始和结束列
			y1,y2 起始和结束行
  返回值  : 无
******************************************************************************/
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	LCD_WR_REG(0x2a); // 列地址设置
	LCD_WR_DATA(x1);
	LCD_WR_DATA(x2);
	LCD_WR_REG(0x2b); // 行地址设置
	LCD_WR_DATA(y1);
	LCD_WR_DATA(y2);
	LCD_WR_REG(0x2c); // 写内存
}

/******************************************************************************
  函数说明: 设置 LCD 背光亮度
  输入参数: dc, 范围 5%~100%
  返回值  : 无
******************************************************************************/
void LCD_Set_Light(uint8_t dc)
{
	if (dc >= 5 && dc <= 100)
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, dc * PWM_PERIOD / 100);
}

/******************************************************************************
  函数说明: 关闭 LCD 背光
  输入参数: 无
  返回值  : 无
******************************************************************************/
void LCD_Close_Light(void)
{
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 0);
}

/******************************************************************************
  函数说明: 打开 LCD 背光
  输入参数: 无
  返回值  : 无
******************************************************************************/
void LCD_Open_Light(void)
{
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}

/******************************************************************************
  函数说明: GC9A01 进入休眠模式
  输入参数: 无
  返回值  : 无
******************************************************************************/
void LCD_SleepIn(void)
{
	LCD_WR_REG(0x10);
	delay_ms(100);
}

/******************************************************************************
  函数说明: GC9A01 退出休眠模式
  输入参数: 无
  返回值  : 无
******************************************************************************/
void LCD_SleepOut(void)
{
	LCD_WR_REG(0x11);
	delay_ms(100);
}

/******************************************************************************
  函数说明: LCD 初始化 (GC9A01)
  输入参数: 无
  返回值  : 无
******************************************************************************/
void LCD_Init(void)
{
	LCD_GPIO_Init(); // 初始化 GPIO

	LCD_RES_Clr(); // 硬件复位
	delay_ms(100);
	LCD_RES_Set();
	delay_ms(100);

	// ==== GC9A01 初始化序列 ====
	LCD_WR_REG(0xEF);
	LCD_WR_REG(0xEB);
	LCD_WR_DATA8(0x14);

	LCD_WR_REG(0xFE);
	LCD_WR_REG(0xEF);

	LCD_WR_REG(0xEB);
	LCD_WR_DATA8(0x14);

	LCD_WR_REG(0x84);
	LCD_WR_DATA8(0x40);

	LCD_WR_REG(0x85);
	LCD_WR_DATA8(0xFF);

	LCD_WR_REG(0x86);
	LCD_WR_DATA8(0xFF);

	LCD_WR_REG(0x87);
	LCD_WR_DATA8(0xFF);

	LCD_WR_REG(0x88);
	LCD_WR_DATA8(0x0A);

	LCD_WR_REG(0x89);
	LCD_WR_DATA8(0x21);

	LCD_WR_REG(0x8A);
	LCD_WR_DATA8(0x00);

	LCD_WR_REG(0x8B);
	LCD_WR_DATA8(0x80);

	LCD_WR_REG(0x8C);
	LCD_WR_DATA8(0x01);

	LCD_WR_REG(0x8D);
	LCD_WR_DATA8(0x01);

	LCD_WR_REG(0x8E);
	LCD_WR_DATA8(0xFF);

	LCD_WR_REG(0x8F);
	LCD_WR_DATA8(0xFF);

	LCD_WR_REG(0xB6);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x20);

	// 设置显示方向 (MADCTL)
	LCD_WR_REG(0x36);
	if (USE_HORIZONTAL == 0)
		LCD_WR_DATA8(0x08);
	else if (USE_HORIZONTAL == 1)
		LCD_WR_DATA8(0xC8);
	else if (USE_HORIZONTAL == 2)
		LCD_WR_DATA8(0x68);
	else
		LCD_WR_DATA8(0xA8);

	// 颜色模式: 16位色 (65K)
	LCD_WR_REG(0x3A);
	LCD_WR_DATA8(0x05);

	LCD_WR_REG(0x90);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x08);

	LCD_WR_REG(0xBD);
	LCD_WR_DATA8(0x06);

	LCD_WR_REG(0xBC);
	LCD_WR_DATA8(0x00);

	LCD_WR_REG(0xFF);
	LCD_WR_DATA8(0x60);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x04);

	LCD_WR_REG(0xC3);
	LCD_WR_DATA8(0x13);

	LCD_WR_REG(0xC4);
	LCD_WR_DATA8(0x13);

	LCD_WR_REG(0xC9);
	LCD_WR_DATA8(0x22);

	LCD_WR_REG(0xBE);
	LCD_WR_DATA8(0x11);

	LCD_WR_REG(0xE1);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x0E);

	LCD_WR_REG(0xDF);
	LCD_WR_DATA8(0x21);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x02);

	// Gamma 正极
	LCD_WR_REG(0xF0);
	LCD_WR_DATA8(0x45);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x26);
	LCD_WR_DATA8(0x2A);

	LCD_WR_REG(0xF1);
	LCD_WR_DATA8(0x43);
	LCD_WR_DATA8(0x70);
	LCD_WR_DATA8(0x72);
	LCD_WR_DATA8(0x36);
	LCD_WR_DATA8(0x37);
	LCD_WR_DATA8(0x6F);

	// Gamma 负极
	LCD_WR_REG(0xF2);
	LCD_WR_DATA8(0x45);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x26);
	LCD_WR_DATA8(0x2A);

	LCD_WR_REG(0xF3);
	LCD_WR_DATA8(0x43);
	LCD_WR_DATA8(0x70);
	LCD_WR_DATA8(0x72);
	LCD_WR_DATA8(0x36);
	LCD_WR_DATA8(0x37);
	LCD_WR_DATA8(0x6F);

	LCD_WR_REG(0xED);
	LCD_WR_DATA8(0x1B);
	LCD_WR_DATA8(0x0B);

	LCD_WR_REG(0xAE);
	LCD_WR_DATA8(0x77);

	LCD_WR_REG(0xCD);
	LCD_WR_DATA8(0x63);

	LCD_WR_REG(0x70);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0E);
	LCD_WR_DATA8(0x0F);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x03);

	LCD_WR_REG(0xE8);
	LCD_WR_DATA8(0x34);

	LCD_WR_REG(0x62);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x71);
	LCD_WR_DATA8(0xED);
	LCD_WR_DATA8(0x70);
	LCD_WR_DATA8(0x70);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x0F);
	LCD_WR_DATA8(0x71);
	LCD_WR_DATA8(0xEF);
	LCD_WR_DATA8(0x70);
	LCD_WR_DATA8(0x70);

	LCD_WR_REG(0x63);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x71);
	LCD_WR_DATA8(0xF1);
	LCD_WR_DATA8(0x70);
	LCD_WR_DATA8(0x70);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x71);
	LCD_WR_DATA8(0xF3);
	LCD_WR_DATA8(0x70);
	LCD_WR_DATA8(0x70);

	LCD_WR_REG(0x64);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x29);
	LCD_WR_DATA8(0xF1);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0xF1);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x07);

	LCD_WR_REG(0x66);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0xCD);
	LCD_WR_DATA8(0x67);
	LCD_WR_DATA8(0x45);
	LCD_WR_DATA8(0x45);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);

	LCD_WR_REG(0x67);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x54);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x32);
	LCD_WR_DATA8(0x98);

	LCD_WR_REG(0x74);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x85);
	LCD_WR_DATA8(0x80);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x4E);
	LCD_WR_DATA8(0x00);

	LCD_WR_REG(0x98);
	LCD_WR_DATA8(0x3E);
	LCD_WR_DATA8(0x07);

	// TE (Tearing Effect Line) 关闭
	LCD_WR_REG(0x35);

	// 反色显示关闭
	LCD_WR_REG(0x21);

	// 睡眠退出
	LCD_WR_REG(0x11);
	delay_ms(120);

	// 显示开启
	LCD_WR_REG(0x29);
	delay_ms(20);
}
