#include "stm32f4xx.h"
#include "port_stm32.h"

#define I2C_TIMEOUT         100u    /// do-not-use HAL_MAX_DELAY as potentially cause bus lockup


int i2c_init(stm32_i2c* i2c, I2C_TypeDef* i2cInstance)
{
    HAL_StatusTypeDef ret;
    i2c->hi2c.Instance = i2cInstance;
    i2c->hi2c.Init.ClockSpeed = 100000; // 100 khz
    i2c->hi2c.Init.DutyCycle = I2C_DUTYCYCLE_2;
    i2c->hi2c.Init.OwnAddress1 = 0;
    i2c->hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    i2c->hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    i2c->hi2c.Init.OwnAddress2 = 0;
    i2c->hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    i2c->hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if ((ret = HAL_I2C_Init(&i2c->hi2c)) == HAL_OK)
        return E_OK;
    else
        return -1;
}

void i2c_set_slave(stm32_i2c* i2c, uint16_t dev_addr)
{
    i2c->dev_addr = dev_addr;
}

int i2c_write(stm32_i2c* i2c, const uint8_t* buffer, uint16_t buffer_sz)
{
    HAL_StatusTypeDef ret;
    if ((ret = HAL_I2C_Master_Transmit(&i2c->hi2c, i2c->dev_addr << 1U, (uint8_t*) buffer, buffer_sz, I2C_TIMEOUT)) == HAL_OK)
        return E_OK;
    else
        return -1;
}


int i2c_read(stm32_i2c* i2c, uint8_t* buffer, uint16_t buffer_sz)
{
    HAL_StatusTypeDef ret;
    if ((ret = HAL_I2C_Master_Receive(&i2c->hi2c, i2c->dev_addr << 1U, (uint8_t*) buffer, buffer_sz, I2C_TIMEOUT)) == HAL_OK)
        return E_OK;
    else
        return -1;
}

int i2c_deinit(stm32_i2c* i2c)
{
    HAL_StatusTypeDef ret;
    if ((ret = HAL_I2C_DeInit(&i2c->hi2c)) == HAL_OK)
        return E_OK;
    else
        return -1;
}
