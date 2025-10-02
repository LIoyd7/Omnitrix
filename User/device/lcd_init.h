#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "bsp/struct_typedef.h"

#define USE_HORIZONTAL 0 // 屏幕显示方向: 0/1为竖屏, 2/3为横屏

#if USE_HORIZONTAL == 0 || USE_HORIZONTAL == 1
#define LCD_W 240
#define LCD_H 280
#else
#define LCD_W 280
#define LCD_H 240
#endif

//----------------- LCD 引脚定义 ----------------
#define SCLK_PORT GPIOB
#define SCLK_PIN GPIO_PIN_3

#define SDA_PORT GPIOB
#define SDA_PIN GPIO_PIN_5

#define RES_PORT GPIOB
#define RES_PIN GPIO_PIN_6

#define DC_PORT GPIOA
#define DC_PIN GPIO_PIN_15

#define CS_PORT GPIOB
#define CS_PIN GPIO_PIN_4

#define BLK_PORT GPIOA
#define BLK_PIN GPIO_PIN_8

#define LCD_SCLK_Clr() HAL_GPIO_WritePin(SCLK_PORT, SCLK_PIN, GPIO_PIN_RESET) // SCL = SCLK
#define LCD_SCLK_Set() HAL_GPIO_WritePin(SCLK_PORT, SCLK_PIN, GPIO_PIN_SET)

#define LCD_MOSI_Clr() HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_RESET) // SDA = MOSI
#define LCD_MOSI_Set() HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET)

#define LCD_RES_Clr() HAL_GPIO_WritePin(RES_PORT, RES_PIN, GPIO_PIN_RESET) // RES
#define LCD_RES_Set() HAL_GPIO_WritePin(RES_PORT, RES_PIN, GPIO_PIN_SET)

#define LCD_DC_Clr() HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_RESET) // DC
#define LCD_DC_Set() HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_SET)

#define LCD_CS_Clr() HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET) // CS
#define LCD_CS_Set() HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET)

#define LCD_BLK_Clr() HAL_GPIO_WritePin(BLK_PORT, BLK_PIN, GPIO_PIN_RESET) // BLK
#define LCD_BLK_Set() HAL_GPIO_WritePin(BLK_PORT, BLK_PIN, GPIO_PIN_SET)

//----------------- LCD 函数声明 ----------------
void LCD_GPIO_Init(void);                                                 // 初始化 GPIO
void LCD_Writ_Bus(uint8_t dat);                                           // 模拟 SPI 写数据
void LCD_WR_DATA8(uint8_t dat);                                           // 写入 8 位数据
void LCD_WR_DATA(uint16_t dat);                                           // 写入 16 位数据
void LCD_WR_REG(uint8_t dat);                                             // 写寄存器
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2); // 设置显示区域
void LCD_Init(void);                                                      // LCD 初始化
void LCD_Set_Light(uint8_t dc);                                           // 设置背光亮度
void LCD_Close_Light(void);                                               // 关闭背光
void LCD_ST7789_SleepIn(void);                                            // 进入休眠
void LCD_ST7789_SleepOut(void);                                           // 退出休眠
void LCD_Open_Light(void);                                                // 打开背光

#endif
