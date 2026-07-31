#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "bsp/struct_typedef.h"

// GC9A01 圆形屏: 固定 240x240
#define USE_HORIZONTAL 0

#define LCD_W 240
#define LCD_H 240

// TIM1 PWM 背光参数 (Prescaler=99, Period=300)
#define PWM_PERIOD 300

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
void LCD_Writ_Bus(uint8_t dat);                                           // 硬件 SPI 写 1 字节 (不控制 CS)
void LCD_WR_DATA8(uint8_t dat);                                           // 写入 8 位数据
void LCD_WR_DATA(uint16_t dat);                                           // 写入 16 位数据
void LCD_WR_REG(uint8_t dat);                                             // 写寄存器 (控制 DC, 不控制 CS)
void LCD_CS_Begin(void);                                                  // 拉低 CS, 开始 SPI 事务
void LCD_CS_End(void);                                                    // 拉高 CS, 结束 SPI 事务
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2); // 设置显示区域 (CS 保持低)
void LCD_Init(void);                                                      // LCD 初始化
void LCD_Set_Light(uint8_t dc);                                           // 设置背光亮度 (5%~100%)
void LCD_Close_Light(void);                                               // 关闭背光
void LCD_SleepIn(void);                                                   // 进入休眠
void LCD_SleepOut(void);                                                  // 退出休眠
void LCD_Open_Light(void);                                                // 打开背光
void LCD_SPI_Set16Bit(void);                                              // 切换 SPI 到 16 位模式
void LCD_SPI_Set8Bit(void);                                               // 切换 SPI 到 8 位模式

#endif
