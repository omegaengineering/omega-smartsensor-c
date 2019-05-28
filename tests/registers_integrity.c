#include <stdio.h>
#include <assert.h>
#include "smartsensor_private.h"
#include "unit_test.h"

int main()
{
    int size = get_def_table_size();
    printf("Table size = %d\n", size);
    int count = LAST_REGISTER;
    assert(size == count * sizeof(_register_t));
    _register_t * table = get_def_table();
    for(int i = 0; i < LAST_REGISTER; i++)
    {
        assert(table[i].size != 0);
    }
}

