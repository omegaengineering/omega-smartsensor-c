#include "common/common.h"
#include "common/log.h"

void print_buffer(const data_buffer_t * buffer)
{
    for(int i = 0; i < buffer->data_len; i++)
    {
        printf("%02X ", buffer->data[i]);
    }
    printf("\n");
    fflush(stdout);
}

void sprint_buffer(uint8_t * buffer, const data_buffer_t * data)
{

}