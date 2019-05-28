//
// Created by dev on 5/13/19.
//

#ifndef OMEGA_SMARTSENSOR_C_ERRORS_H
#define OMEGA_SMARTSENSOR_C_ERRORS_H

typedef enum {
    E_OK                = 0x00000,
    E_BUS_UNAVAILABLE   = 0xf0000,
    E_BUS_OPERATION,
    E_API_ERROR,
    E_BUFFER_MEM_SIZE,
    E_INVALID_PARAM,
    E_NO_MEM,
    E_NOT_SUPPORTED
} error_t;

#endif //OMEGA_SMARTSENSOR_C_ERRORS_H
