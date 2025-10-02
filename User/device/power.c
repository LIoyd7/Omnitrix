#include "power.h"
#include "adc.h"
#include "bsp/bsp_delay.h"

#define INTERNAL_RES 0.128
#define CHARGING_CUR 1


float BatCheck()
{
    uint16_t dat;
    float BatVoltage;
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 5);
    dat = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    BatVoltage = dat * 2 * 3.3 / 4096;
    return BatVoltage;
}

float BatCheck_8times()
{
    uint32_t dat = 0;
    uint8_t i;
    float BatVoltage;
    for (i = 0; i < 8; i++)
    {
        HAL_ADC_Start(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, 5);
        dat += HAL_ADC_GetValue(&hadc1);
        HAL_ADC_Stop(&hadc1);
        delay_ms(1);
    }
    dat = dat >> 3;
    BatVoltage = dat * 2 * 3.3 / 4096;
    return BatVoltage;
}

uint8_t PowerCalculate()
{
    uint8_t power = 0; // 默认 0%
    float voltage;
    voltage = BatCheck_8times();

    // if (ChargeCheck())
    // {
    //     voltage -= INTERNAL_RES * CHARGING_CUR;
    // }

    if ((voltage >= 4.2))
    {
        power = 100;
    }
    else if (voltage >= 4.06 && voltage < 4.2)
    {
        power = 90;
    }
    else if (voltage >= 3.98 && voltage < 4.06)
    {
        power = 80;
    }
    else if (voltage >= 3.92 && voltage < 3.98)
    {
        power = 70;
    }
    else if (voltage >= 3.87 && voltage < 3.92)
    {
        power = 60;
    }
    else if (voltage >= 3.82 && voltage < 3.87)
    {
        power = 50;
    }
    else if (voltage >= 3.79 && voltage < 3.82)
    {
        power = 40;
    }
    else if (voltage >= 3.77 && voltage < 3.79)
    {
        power = 30;
    }
    else if (voltage >= 3.74 && voltage < 3.77)
    {
        power = 20;
    }
    else if (voltage >= 3.68 && voltage < 3.74)
    {
        power = 10;
    }
    else if (voltage >= 3.45 && voltage < 3.68)
    {
        power = 5;
    }
    return power;
}

