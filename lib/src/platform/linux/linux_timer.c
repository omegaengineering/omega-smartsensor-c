//
// Created by dev on 5/23/19.
//

#include <pthread.h>
#include <zconf.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <stdio.h>
#include "common/errors.h"
#include "common/log.h"
#include "platform/timer.h"
#include "platform/memory.h"

#define MAX_TIMER_TRIGGER   1

struct _s19_timer_entry{
    uint32_t period;
    uint32_t last_triggered;
    timer_trigger_callback callback;
    void*   user_ctx;
    bool     taken;
};

struct _s19_timer {
    pthread_t            thread_handle;
    s19_timer_entry_t    timer_list[MAX_TIMER_TRIGGER];
    uint8_t              shutdown_req;
};

s19_log_create("Timer", LOG_LEVEL_INFO);

static uint32_t s19_time_now_sec()
{
    struct timeval ltime;
    gettimeofday(&ltime, NULL);
    return (int) ltime.tv_sec;
}

static long long s19_time_now_msec()
{
    struct timeval ltime;
    gettimeofday(&ltime, NULL);
    return ltime.tv_sec * 1000 + ltime.tv_usec / 1000;
}

static void timer_check(s19_timer_t * timer)
{
    uint32_t time_now = s19_time_now_sec();
    for(int i = 0; i < MAX_TIMER_TRIGGER; i++)
    {
        s19_timer_entry_t * entry = &timer->timer_list[i];
        if (entry->callback &&
                time_now - entry->last_triggered >= entry->period)
        {
            entry->callback(entry->user_ctx);
            entry->last_triggered = time_now;
        }
    }
}

static void* timer_thread(void * data)
{
    s19_timer_t * timer = (s19_timer_t*) data;
    while (!timer->shutdown_req)
    {
        timer_check(timer);
        sleep(1);
    }
    s19_log_dbg("Timer thread exited\n");
}


/// each new timer will launch a thread
int s19_timer_create(s19_timer_t ** timer)
{
    int ret;
    s19_timer_t * new_timer = s19_mem_malloc(sizeof(struct _s19_timer));
    if (new_timer == NULL)
        return E_NO_MEM;

    memset(new_timer, 0, sizeof(struct _s19_timer));
    pthread_attr_t attr;
    if ((ret = pthread_attr_init(&attr)) < 0)
        return ret;
    if ((ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE)) < 0)
        return ret;
    if ((ret = pthread_create(&new_timer->thread_handle, &attr, timer_thread, new_timer)) < 0)
        return ret;
    *timer = new_timer;
    return E_OK;
}

int s19_timer_entry_add(s19_timer_t *timer, s19_timer_entry_t **trigger)
{
    for(int i = 0; i < MAX_TIMER_TRIGGER; i++)
    {
        s19_timer_entry_t * entry = &timer->timer_list[i];
        if (!entry->taken)
        {
            entry->taken = true;
            *trigger = entry;
            return E_OK;
        }
    }
    return E_NO_MEM;
}

int s19_timer_entry_set_period(s19_timer_entry_t *trigger, uint32_t period)
{
    trigger->period = period;
    return E_OK;
}
int s19_timer_entry_set_ctx(s19_timer_entry_t *trigger, void * ctx)
{
    trigger->user_ctx = ctx;
    return E_OK;
}
int s19_timer_entry_set_callback(s19_timer_entry_t *trigger, timer_trigger_callback callback)
{
    trigger->callback = callback;
    return E_OK;
}
int s19_timer_entry_remove(s19_timer_entry_t *trigger)
{
    memset(trigger, 0, sizeof(s19_timer_entry_t));
    return E_OK;
}
int s19_timer_destroy(s19_timer_t * timer)
{
    if (timer)
    {
        timer->shutdown_req = true;
//        pthread_cancel(timer->thread_handle);
        pthread_join(timer->thread_handle, NULL);
        s19_mem_free(timer);
    }
    return E_OK;
}

void s19_sleep_msec(uint32_t msec)
{
    usleep(msec);
}
