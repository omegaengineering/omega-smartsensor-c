/*!********************************************************************************************
  @file     contiki_mutex.c

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
  @date     June 5th, 2019
  @details


***********************************************************************************************/

#include "common/errors.h"
#include "platform/mutex.h"
#include "platform/memory.h"
#include "contiki.h"
#include "sys/mutex.h"

struct _s19_mutex {
    mutex_t lock;
    uint8_t used;
};

static struct _s19_mutex instances[MAX_PLATFORM_MUTEX];

int s19_mutex_create(s19_mutex_t **mutex)
{
    for (int i = 0; i < MAX_PLATFORM_MUTEX; i++)
    {
        if (!instances[i].used)
        {
            instances[i].used = true;
            *mutex = &instances[i];
            return E_OK;
        }
    }
    return E_NO_MEM;
}

int s19_mutex_lock(s19_mutex_t *mutex)
{
    return mutex_try_lock(&mutex->lock) == MUTEX_STATUS_LOCKED ?
                E_OK : E_TRY_AGAIN;
}

int s19_mutex_unlock(s19_mutex_t *mutex)
{
    mutex_unlock(&mutex->lock);
    return E_OK;
}

int s19_mutex_destroy(s19_mutex_t *mutex)
{
    mutex->used = false;
    return E_OK;
}