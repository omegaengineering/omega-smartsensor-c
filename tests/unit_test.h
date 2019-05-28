
//
// Created by dev on 5/28/19.
//

#ifndef OMEGA_SMARTSENSOR_C_UNIT_TEST_H
#define OMEGA_SMARTSENSOR_C_UNIT_TEST_H

typedef struct {
    uint16_t    modbus_addr;
    uint16_t    i2c_addr;
    uint16_t    access;
    uint16_t    size;
} _register_t;

extern int get_def_table_size();
extern _register_t* get_def_table();

#endif //OMEGA_SMARTSENSOR_C_UNIT_TEST_H
