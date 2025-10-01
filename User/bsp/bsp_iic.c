#include "bsp_iic.h"
#include "bsp_delay.h"

/**
 * @brief SDA 设置为输入模式
 * @param None
 * @retval None
 */
void SDA_Input_Mode(iic_bus_t *bus)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = bus->IIC_SDA_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(bus->IIC_SDA_PORT, &GPIO_InitStructure);
}

/**
 * @brief SDA 设置为输出模式
 * @param None
 * @retval None
 */
void SDA_Output_Mode(iic_bus_t *bus)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = bus->IIC_SDA_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(bus->IIC_SDA_PORT, &GPIO_InitStructure);
}

/**
 * @brief SDA 输出一位
 * @param val 输出的电平（0/1）
 * @retval None
 */
void SDA_Output(iic_bus_t *bus, uint16_t val)
{
    if (val)
    {
        bus->IIC_SDA_PORT->BSRR |= bus->IIC_SDA_PIN;
    }
    else
    {
        bus->IIC_SDA_PORT->BSRR = (uint32_t)bus->IIC_SDA_PIN << 16U;
    }
}

/**
 * @brief SCL 输出一位
 * @param val 输出的电平（0/1）
 * @retval None
 */
void SCL_Output(iic_bus_t *bus, uint16_t val)
{
    if (val)
    {
        bus->IIC_SCL_PORT->BSRR |= bus->IIC_SCL_PIN;
    }
    else
    {
        bus->IIC_SCL_PORT->BSRR = (uint32_t)bus->IIC_SCL_PIN << 16U;
    }
}

/**
 * @brief 读取 SDA 输入一位
 * @param None
 * @retval 1:高电平 0:低电平
 */
uint8_t SDA_Input(iic_bus_t *bus)
{
    if (HAL_GPIO_ReadPin(bus->IIC_SDA_PORT, bus->IIC_SDA_PIN) == GPIO_PIN_SET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief IIC 起始信号
 */
void IICStart(iic_bus_t *bus)
{
    SDA_Output(bus, 1);
    delay_us(2);
    SCL_Output(bus, 1);
    delay_us(1);
    SDA_Output(bus, 0);
    delay_us(1);
    SCL_Output(bus, 0);
    delay_us(1);
}

/**
 * @brief IIC 停止信号
 */
void IICStop(iic_bus_t *bus)
{
    SCL_Output(bus, 0);
    delay_us(2);
    SDA_Output(bus, 0);
    delay_us(1);
    SCL_Output(bus, 1);
    delay_us(1);
    SDA_Output(bus, 1);
    delay_us(1);
}

/**
 * @brief IIC 等待应答信号
 * @retval SUCCESS:收到应答 ERROR:超时未收到
 */
unsigned char IICWaitAck(iic_bus_t *bus)
{
    unsigned short cErrTime = 5;
    SDA_Input_Mode(bus);
    SCL_Output(bus, 1);
    while (SDA_Input(bus))
    {
        cErrTime--;
        delay_us(1);
        if (0 == cErrTime)
        {
            SDA_Output_Mode(bus);
            IICStop(bus);
            return ERROR;
        }
    }
    SDA_Output_Mode(bus);
    SCL_Output(bus, 0);
    delay_us(2);
    return SUCCESS;
}

/**
 * @brief IIC 发送应答
 */
void IICSendAck(iic_bus_t *bus)
{
    SDA_Output(bus, 0);
    delay_us(1);
    SCL_Output(bus, 1);
    delay_us(1);
    SCL_Output(bus, 0);
    delay_us(1);
}

/**
 * @brief IIC 发送非应答
 */
void IICSendNotAck(iic_bus_t *bus)
{
    SDA_Output(bus, 1);
    delay_us(1);
    SCL_Output(bus, 1);
    delay_us(1);
    SCL_Output(bus, 0);
    delay_us(2);
}

/**
 * @brief IIC 发送一个字节
 */
void IICSendByte(iic_bus_t *bus, unsigned char cSendByte)
{
    unsigned char i = 8;
    while (i--)
    {
        SCL_Output(bus, 0);
        delay_us(2);
        SDA_Output(bus, cSendByte & 0x80);
        delay_us(1);
        cSendByte <<= 1;
        delay_us(1);
        SCL_Output(bus, 1);
        delay_us(1);
    }
    SCL_Output(bus, 0);
    delay_us(2);
}

/**
 * @brief IIC 接收一个字节
 */
unsigned char IICReceiveByte(iic_bus_t *bus)
{
    unsigned char i = 8;
    unsigned char cR_Byte = 0;
    SDA_Input_Mode(bus);
    while (i--)
    {
        cR_Byte <<= 1;
        SCL_Output(bus, 0);
        delay_us(2);
        SCL_Output(bus, 1);
        delay_us(1);
        cR_Byte |= SDA_Input(bus);
    }
    SCL_Output(bus, 0);
    SDA_Output_Mode(bus);
    return cR_Byte;
}

/**
 * @brief 写单字节
 */
uint8_t IIC_Write_One_Byte(iic_bus_t *bus, uint8_t daddr, uint8_t reg, uint8_t data)
{
    IICStart(bus);

    IICSendByte(bus, daddr << 1);
    if (IICWaitAck(bus)) // 等待应答
    {
        IICStop(bus);
        return 1;
    }
    IICSendByte(bus, reg);
    IICWaitAck(bus);
    IICSendByte(bus, data);
    IICWaitAck(bus);
    IICStop(bus);
    delay_us(1);
    return 0;
}

/**
 * @brief 写多个字节
 */
uint8_t IIC_Write_Multi_Byte(iic_bus_t *bus, uint8_t daddr, uint8_t reg, uint8_t length, uint8_t buff[])
{
    unsigned char i;
    IICStart(bus);

    IICSendByte(bus, daddr << 1);
    if (IICWaitAck(bus))
    {
        IICStop(bus);
        return 1;
    }
    IICSendByte(bus, reg);
    IICWaitAck(bus);
    for (i = 0; i < length; i++)
    {
        IICSendByte(bus, buff[i]);
        IICWaitAck(bus);
    }
    IICStop(bus);
    delay_us(1);
    return 0;
}

/**
 * @brief 读单字节
 */
unsigned char IIC_Read_One_Byte(iic_bus_t *bus, uint8_t daddr, uint8_t reg)
{
    unsigned char dat;
    IICStart(bus);
    IICSendByte(bus, daddr << 1);
    IICWaitAck(bus);
    IICSendByte(bus, reg);
    IICWaitAck(bus);

    IICStart(bus);
    IICSendByte(bus, (daddr << 1) + 1);
    IICWaitAck(bus);
    dat = IICReceiveByte(bus);
    IICSendNotAck(bus);
    IICStop(bus);
    return dat;
}

/**
 * @brief 读多个字节
 */
uint8_t IIC_Read_Multi_Byte(iic_bus_t *bus, uint8_t daddr, uint8_t reg, uint8_t length, uint8_t buff[])
{
    unsigned char i;
    IICStart(bus);
    IICSendByte(bus, daddr << 1);
    if (IICWaitAck(bus))
    {
        IICStop(bus);
        return 1;
    }
    IICSendByte(bus, reg);
    IICWaitAck(bus);

    IICStart(bus);
    IICSendByte(bus, (daddr << 1) + 1);
    IICWaitAck(bus);
    for (i = 0; i < length; i++)
    {
        buff[i] = IICReceiveByte(bus);
        if (i < length - 1)
        {
            IICSendAck(bus);
        }
    }
    IICSendNotAck(bus);
    IICStop(bus);
    return 0;
}
