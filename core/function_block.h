#ifndef SMARTSENSOR_FUNCTION_BLOCK_H
#define SMARTSENSOR_FUNCTION_BLOCK_H

#include "smartsensor.h"

#define FB__PROGRAM_MAX     32
#define FB__PARAM_MAX       32
#define FB__EOB             0xffffff7e

typedef enum fb_type {
    FUNCTION_BLOCK_ALARM        = 0x00,
    FUNCTION_BLOCK_MATH,
    FUNCTION_BLOCK_MINMAX,
    FUNCTION_BLOCK_CONTROL,
    FUNCTION_BLOCK_TRANSFER,
    FUNCTION_BLOCK_END          = 0x0f
} fb_type_t;

/*! template function block structure */
typedef struct fb_control {
    uint8_t     block_enable: 1;
    uint8_t     block_type  : 4;
    uint8_t     block_mode  : 2;
    uint8_t     status      : 1;
    uint8_t     variable_select_index : 5;
    uint8_t     ext_addr : 3;
    uint8_t     reserved1   : 8;
    uint8_t     reserved2   : 8;
} fb_data_t;

typedef struct fb_block {
    uint8_t     block_idx;
    uint32_t    block_data;
    struct fb_block* next;
} fb_block_t;

/*! enable function block processing. Do this after finished with editing function block */
int fb_enable(sensor_t* sensor);
/*! disable function block processing. Do this before editing function blocks */
int fb_disable(sensor_t* sensor);

/*! init func block at index */
void fb_init_head(fb_block_t* head, uint8_t index);
/*! chain block to the tail of list */
void fb_chain(fb_block_t* head, fb_block_t* next_block);
/*! terminate list with EOB marker */
void fb_terminate(fb_block_t* head, fb_block_t* eob);
/*! write func blocks down to smartsensor */
int  fb_commit(sensor_t* sensor, fb_block_t* head);

int fb_program_read(sensor_t* sensor, uint8_t index, void* buffer, uint32_t buf_len);
int fb_program_write(sensor_t* sensor, uint8_t index, void* buffer, uint32_t buf_len);
int fb_program_erase(sensor_t* sensor, uint8_t index);
int fb_program_erase_all(sensor_t* sensor);

int fb_param_write(sensor_t* sensor, uint8_t index, void* buffer, uint32_t buf_len);

#if DEBUG
/*! print out the function block chain for inspection */
void fb_print(fb_block_t* head);
#else
#define fb_print
#endif


#endif //SMARTSENSOR_FUNCTION_BLOCK_H
