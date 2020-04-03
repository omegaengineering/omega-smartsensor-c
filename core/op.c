#include "op.h"

#define BUFFER_SIZE 1024

/// buffer to store operations, NOT a circular buffer
static uint8_t buffer[BUFFER_SIZE];

static uint8_t op_cnt;
static uint8_t op_cur;
static uint8_t *op_in = buffer;
static uint8_t *op_out = buffer;

static int is_empty()
{
    return op_in == op_out;
}

static int is_full(op_e op, uint8_t* data, uint16_t data_len)
{

}

int op_push(op_e op, uint8_t* data, uint16_t data_len)
{
    if (op_cur)
    return -1;
}

///
int op_push_next(op_e op, uint8_t* data, uint16_t data_len)
{

}
