//
// Created by dev on 5/15/19.
//

#include "platform/memory.h"

void* s19_mem_malloc(size_t size)
{
    return malloc(size);
}

void s19_mem_free(void *ptr)
{
    free(ptr);
}
