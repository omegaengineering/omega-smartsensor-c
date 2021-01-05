#ifndef SMARTSENSOR_PORT_STM32_H
#define SMARTSENSOR_PORT_STM32_H

#include "port.h"
#include "stm32f4xx_hal.h"

typedef struct {
    I2C_HandleTypeDef   hi2c;
    uint16_t            dev_addr;
} stm32_i2c;

int i2c_init(stm32_i2c* i2c, I2C_TypeDef* i2cInstance);
void i2c_set_slave(stm32_i2c* i2c, uint16_t dev_addr);
int i2c_write(stm32_i2c* i2c, const uint8_t* buffer, uint16_t buffer_sz);
int i2c_read(stm32_i2c* i2c, uint8_t* buffer, uint16_t buffer_sz);
int i2c_deinit(stm32_i2c* i2c);


int data_pin_init(uint16_t pin);



#endif //SMARTSENSOR_PORT_STM32_H
