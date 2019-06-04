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