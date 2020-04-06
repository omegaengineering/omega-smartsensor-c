#ifndef SMARTSENSOR_SMARTSENSOR_ERRORS_H
#define SMARTSENSOR_SMARTSENSOR_ERRORS_H

typedef enum {
    E_UNAVAILABLE = -1,
    E_OK                = 0,
    E_BUS_OPERATION,
    E_BUS_TYPE,
    E_BUFFER_MEM_SIZE,
    E_INVALID_PARAM,
    E_NO_MEM,
    E_NOT_SUPPORTED,
    E_TRY_AGAIN,
    E_FULL,
    E_EMPTY,
    E_CONTINUE,
} error_t;

#endif //SMARTSENSOR_SMARTSENSOR_ERRORS_H
