#include <string.h>
#include "log.h"
#include "function_block.h"

int fb_enable(sensor_t* sensor)
{
    int ret;
    uint16_t control;
    if ((ret = sensor_read(sensor, SYSTEM_CONTROL, &control, sizeof(control))) != 0)
        return ret;
    if (!(control & ENABLE_FUNCTION_BLOCK)) {
        control |= ENABLE_FUNCTION_BLOCK;
        if ((ret = sensor_write(sensor, SYSTEM_CONTROL, &control, sizeof(control))) != 0)
            return ret;
    }
    return ret;
}

int fb_disable(sensor_t* sensor)
{
    int ret;
    uint16_t control;
    if ((ret = sensor_read(sensor, SYSTEM_CONTROL, &control, sizeof(control))) != 0)
        return ret;
    control &= ~ENABLE_FUNCTION_BLOCK;
    if ((ret = sensor_write(sensor, SYSTEM_CONTROL, &control, sizeof(control))) != 0)
        return ret;
    return ret;
}

void fb_init_head(fb_block_t* head, uint8_t index)
{
    if (head) {
        fb_block_t* temp = head;
        temp->block_idx = index++;
        while (temp->next) {
            temp = temp->next;
            temp->block_idx = index++;
        }
    }
}

void fb_chain(fb_block_t* head, fb_block_t* next_block)
{
    if (head) {
        fb_block_t* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        next_block->block_idx = temp->block_idx+1;
        temp->next = next_block;
        temp = temp->next;
        while (temp->next) {
            temp->next->block_idx = temp->block_idx + 1;
        }
    }
}

void fb_terminate(fb_block_t* head, fb_block_t* eob)
{
    eob->block_data = FB__EOB;
    eob->next = NULL;
    fb_chain(head, eob);
}


int fb_commit(sensor_t* sensor, fb_block_t* head)
{
    int ret;
    fb_block_t* temp = head;
    while (temp) {
        if ((ret = fb_program_write(sensor, temp->block_idx, &temp->block_data, sizeof(temp->block_data))) != E_OK)
            return ret;
        temp = temp->next;
    }
    return ret;
}

int fb_program_erase(sensor_t* sensor, uint8_t index)
{
    uint32_t EOB_data = FB__EOB;
    return fb_program_write(sensor, index, &EOB_data, sizeof(EOB_data));
}

int fb_program_erase_all(sensor_t* sensor)
{
    // just write first block to EOB is enough, no need to write to 32 blocks
    return fb_program_erase(sensor, 0);
}

int fb_program_read(sensor_t* sensor, uint8_t index, void* buffer, uint32_t buf_len)
{
    if (buf_len != sizeof(uint32_t) || index >= FB__PROGRAM_MAX)
        return E_INVALID_PARAM;
    return sensor_indexed_read(sensor, FUNCTION_BLOCK_PROGRAM, index, buffer, buf_len);
}

int fb_program_write(sensor_t* sensor, uint8_t index, void* buffer, uint32_t buf_len)
{
    if (buf_len != sizeof(uint32_t) || index >= FB__PROGRAM_MAX)
        return E_INVALID_PARAM;
    return sensor_indexed_write(sensor, FUNCTION_BLOCK_PROGRAM, index, buffer, buf_len);
}

int fb_param_write(sensor_t* sensor, uint8_t index, void* buffer, uint32_t buf_len)
{
    if (buf_len != sizeof(uint32_t) || index >= FB__PARAM_MAX)
        return E_INVALID_PARAM;
    return sensor_indexed_write(sensor, FUNCTION_BLOCK_PARAM, index, buffer, buf_len);
}

#if DEBUG

void fb_print(fb_block_t* head)
{
    fb_block_t* temp = head;
    while (temp) {
        s_log("Block %d, data %08X\n", temp->block_idx, temp->block_data);
        temp = temp->next;
    }
}

#endif