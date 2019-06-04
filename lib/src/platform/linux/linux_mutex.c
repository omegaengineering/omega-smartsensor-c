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
