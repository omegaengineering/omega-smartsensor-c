#ifndef SMARTSENSOR_SMARTSENSOR_ERRORS_H
#define SMARTSENSOR_SMARTSENSOR_ERRORS_H

#define GET_SENSOR_ERR(e)   (e & 0xffff)
#define GET_PORT_ERR(e)     (e >> 16)
#define SET_PORT_ERR(n)     ((n << 16) + E_PORT_ERR)

typedef enum {
    E_OK                = 0,
    E_UNAVAILABLE,
    E_BUS_OPERATION,
    E_BUS_TYPE,
    E_BUFFER_MEM_SIZE,
    E_INVALID_PARAM,
    E_INVALID_ADDR,
    E_NO_MEM,
    E_NOT_SUPPORTED,
    E_TRY_AGAIN,
    E_FULL,
    E_EMPTY,
    E_CONTINUE,
    E_PORT_UNAVAILABLE,
    E_PORT_ERR = 0x8000,
} error_t;

#endif //SMARTSENSOR_SMARTSENSOR_ERRORS_H
