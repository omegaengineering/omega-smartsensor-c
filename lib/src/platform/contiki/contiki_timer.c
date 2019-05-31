#include "common/errors.h"
#include "common/log.h"
#include "platform/timer.h"
#include "platform/memory.h"

struct _s19_timer {

};

struct _s19_timer_entry {

};

int s19_timer_create(s19_timer_t ** timer)
{
    return E_OK;
}

int s19_timer_entry_add(s19_timer_t *timer, s19_timer_entry_t **trigger)
{
    return E_OK;
}
int s19_timer_entry_set_period(s19_timer_entry_t *trigger, uint32_t period)
{
    return E_OK;
}
int s19_timer_entry_set_ctx(s19_timer_entry_t *trigger, void * ctx)
{
    return E_OK;
}
int s19_timer_entry_set_callback(s19_timer_entry_t *trigger, timer_trigger_callback callback)
{
    return E_OK;
}
int s19_timer_entry_remove(s19_timer_entry_t *trigger)
{
    return E_OK;
}
int s19_timer_destroy(s19_timer_t * timer)
{
    return E_OK;
}

void s19_sleep_msec(uint32_t msec)
{

}