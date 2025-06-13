/*!********************************************************************************************
  @file     freertos_mutex.c

  @copyright
            Copyright (c) 2019, Omega Engineering Inc.

            Permission is hereby granted, free of charge, to any person obtaining
            a copy of this software and associated documentation files (the
            'Software'), to deal in the Software without restriction, including
            without limitation the rights to use, copy, modify, merge, publish,
            distribute, sublicense, and/or sell copies of the Software, and to
            permit persons to whom the Software is furnished to do so, subject to
            the following conditions:

            The above copyright notice and this permission notice shall be
            included in all copies or substantial portions of the Software.

            THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
            EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
            MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
            IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
            CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
            TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
            SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

  @author   Binh Dinh
  @date     August 5th, 2019
  @details


***********************************************************************************************/
#include <stdint.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include "common/errors.h"
#include "platform/mutex.h"
#include "platform/memory.h"

struct _s19_mutex {
    SemaphoreHandle_t mutex;
};

int s19_mutex_create(s19_mutex_t **mutex)
{
    *mutex = s19_mem_malloc(sizeof(struct _s19_mutex));
    if (*mutex)
    {
        (*mutex)->mutex = (s19_mutex_t*) xSemaphoreCreateMutex();
    }
    return E_OK;
}

int s19_mutex_lock(s19_mutex_t *mutex)
{
//    return xSemaphoreTakeFromISR( mutex->mutex, NULL ) ?
//           E_OK : E_TRY_AGAIN;
	  return xSemaphoreTake( mutex->mutex,  portMAX_DELAY) ?
	           E_OK : E_TRY_AGAIN;
}

int s19_mutex_unlock(s19_mutex_t *mutex)
{
//    return xSemaphoreGiveFromISR( mutex->mutex, NULL ) ? E_OK : E_TRY_AGAIN;
    return xSemaphoreGive( mutex->mutex ) ? E_OK : E_TRY_AGAIN;
}

int s19_mutex_destroy(s19_mutex_t *mutex)
{
    vSemaphoreDelete(mutex->mutex);
    return E_OK;
}
