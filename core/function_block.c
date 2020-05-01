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