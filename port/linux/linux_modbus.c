#include <stdint.h>
#include <string.h>
#include <modbus/modbus-rtu.h>
#include <modbus/modbus.h>
#include "smartsensor.h"
#include "linux_private.h"

#define MODBUS_BUFFER_SIZE      128      /**< buffer for manipulating user data to modbus endianess */
#define MODBUS_BAUD             115200
#define MODBUS_PARITY           'N'
#define MODBUS_DATA_BITS        8
#define MODBUS_STOP_BITS        1


typedef struct {
    modbus_t*       mb;
    uint16_t        dev_addr;
    uint8_t         buffer[MODBUS_BUFFER_SIZE];
} linux_modbus_t;

int linux_modbus_open(linux_modbus_t* mb, const char* device)
{
    int ret;
    mb->mb = modbus_new_rtu(device,
                        MODBUS_BAUD,
                        MODBUS_PARITY,
                        MODBUS_DATA_BITS,
                        MODBUS_STOP_BITS);
    if (mb->mb == NULL)
        return E_BUS_OPERATION;
    if ((ret = modbus_connect(mb->mb)) < 0)
        return E_BUS_OPERATION;
    return E_OK;
}

static void linux_modbus_set_slave(linux_modbus_t* mb, uint16_t dev_addr)
{
    mb->dev_addr = dev_addr;
}

int linux_modbus_read(linux_modbus_t* mb, uint16_t reg_addr, void* buffer, uint16_t buffer_sz)
{
    int ret;
    uint16_t rx_size = buffer_sz % 2 ? buffer_sz + 1: buffer_sz;

    if (rx_size > sizeof(mb->buffer))
        return E_NO_MEM;

    ret = modbus_read_registers(mb->mb, reg_addr, rx_size/2, (uint16_t *) mb->buffer);
    if (ret != rx_size / 2) {
        return E_BUS_OPERATION;
    }

    for(int idx = 0; idx <= rx_size/2; idx += 2) {
        uint8_t t = mb->buffer[idx];
        mb->buffer[idx] = mb->buffer[idx+1];
        mb->buffer[idx+1] = t;
    }
    memcpy(buffer, mb->buffer, buffer_sz);
    return E_OK;
}

int linux_modbus_write(linux_modbus_t* mb, uint16_t reg_addr, void* buffer, uint16_t buffer_sz)
{
    int ret;
    uint16_t tx_size = buffer_sz % 2 ? buffer_sz + 1: buffer_sz;

    if (tx_size > sizeof(mb->buffer))
        return E_NO_MEM;

    memcpy(mb->buffer, buffer, buffer_sz);
    for (int idx = 0; idx <= tx_size/2; idx +=2 ) {
        uint8_t t = mb->buffer[idx];
        mb->buffer[idx] = mb->buffer[idx + 1];
        mb->buffer[idx + 1] = t;
    }

    ret = modbus_write_registers(mb->mb, reg_addr, tx_size /2, (uint16_t *) mb->buffer);
    if (ret != tx_size / 2) {
        return E_BUS_OPERATION;
    }
    return E_OK;
}

int linux_modbus_close(linux_modbus_t* mb)
{
    if (mb) {
        modbus_close(mb->mb);
        modbus_free(mb->mb);
    }
    return E_OK;
}