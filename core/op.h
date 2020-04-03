#ifndef SMARTSENSOR_OP_H
#define SMARTSENSOR_OP_H

#include <stdint.h>

typedef enum {
    OP_SEND,
    OP_RECV,
} op_e;

typedef struct {
    uint16_t    data_len;
    uint8_t     data[];
} op_send_t;

typedef struct {
    uint16_t    data_len;
    uint8_t*    data;
} op_recv_t;

typedef struct {
    op_e            op;
    union {
        op_send_t   send_data;
        op_recv_t   recv_data;
    };
} op_t;

int op_push(op_e op, uint8_t* data, uint16_t data_len);
int op_push_next(op_e op, uint8_t* data, uint16_t data_len);

#endif //SMARTSENSOR_OP_H
