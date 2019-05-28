//
// Created by dev on 5/13/19.
//

#ifndef OMEGA_SMARTSENSOR_C_COMMON_H
#define OMEGA_SMARTSENSOR_C_COMMON_H

#include <stdint.h>

typedef struct {
    uint8_t * const data;
    const uint16_t data_len;
} data_buffer_t;


#endif //OMEGA_SMARTSENSOR_C_COMMON_H
