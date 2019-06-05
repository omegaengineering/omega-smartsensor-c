/*!********************************************************************************************
  @file     linux_mutex.c

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

#include <pthread.h>
#include <errno.h>
#include "platform/mutex.h"
#include "platform/memory.h"
#include "common/errors.h"

struct _s19_mutex {
    pthread_mutex_t lock;
};

int s19_mutex_create(s19_mutex_t **mutex)
{
    s19_mutex_t * new_mutex = s19_mem_malloc(sizeof(s19_mutex_t));
    if (new_mutex == NULL)
        return E_NO_MEM;
    int ret = pthread_mutex_init(&new_mutex->lock, NULL);
    if (ret < 0)
    {
        s19_mem_free(new_mutex);
        return E_API_ERROR;
    }
    *mutex = new_mutex;
    return E_OK;
}

int s19_mutex_lock(s19_mutex_t *mutex)
{
    int status;
    return (status = pthread_mutex_lock(&mutex->lock)) == 0 ? E_OK : status;
}

int s19_mutex_unlock(s19_mutex_t *mutex)
{
    int status;
    return (status = pthread_mutex_unlock(&mutex->lock)) == 0 ? E_OK : status;
}

int s19_mutex_destroy(s19_mutex_t *mutex)
{
    int ret;
    if (mutex)
    {
        ret = pthread_mutex_destroy(&mutex->lock);
        s19_mem_free(mutex);
        return ret;
    }
    return E_OK;
}
