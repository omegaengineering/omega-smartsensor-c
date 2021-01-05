#ifndef SMARTSENSOR_STM32_FREERTOS_H
#define SMARTSENSOR_STM32_FREERTOS_H

#include "smartsensor.h"
#include "stm32f4xx.h"

struct _port_cfg {
    I2C_TypeDef*     i2cInstance;
};



#endif //SMARTSENSOR_STM32_FREERTOS_H
