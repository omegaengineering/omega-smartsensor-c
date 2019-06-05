/*!********************************************************************************************
  @file     linux_timer.c

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
#include <zconf.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include "common/errors.h"
#include "common/log.h"
#include "platform/timer.h"
#include "platform/memory.h"

#define MAX_TIMER_TRIGGER       1
#define BASE_CHECK_PERIOD_SEC   1

struct _s19_timer_entry{
    uint32_t period;
    uint32_t last_triggered;
    timer_trigger_callback callback;
    void*   user_ctx;
    bool     taken;
};

struct _s19_timer {
    pthread_t           thread_handle;
    s19_timer_entry_t   timer_list[MAX_TIMER_TRIGGER];
    bool                do_exit;
    pthread_mutex_t     do_exit_mtx;
    pthread_cond_t      do_exit_cond;
};

s19_log_create("Timer", LOG_LEVEL_DEBUG);

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
    int ret;
    s19_timer_t * timer = (s19_timer_t*) data;
    s19_log_dbg("Timer thread started\n");
    while (!timer->do_exit)
    {
        struct timespec ts;

        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += BASE_CHECK_PERIOD_SEC;

        pthread_mutex_lock(&timer->do_exit_mtx);
        ret = pthread_cond_timedwait(&timer->do_exit_cond, &timer->do_exit_mtx, &ts);
        pthread_mutex_unlock(&timer->do_exit_mtx);

        if (ret == ETIMEDOUT)
            timer_check(timer);
        else
            break;
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

    if ((ret = pthread_cond_init(&new_timer->do_exit_cond, NULL)) < 0)
        goto ERROR;
    if ((ret = pthread_mutex_init(&new_timer->do_exit_mtx, NULL)) < 0)
        goto ERROR;

    memset(new_timer, 0, sizeof(struct _s19_timer));
    pthread_attr_t attr;
    if ((ret = pthread_attr_init(&attr)) < 0)
        goto ERROR;
    if ((ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE)) < 0)
        goto ERROR;
    if ((ret = pthread_create(&new_timer->thread_handle, &attr, timer_thread, new_timer)) < 0)
        goto ERROR;
    *timer = new_timer;
    return E_OK;

ERROR:
    s19_mem_free(new_timer);
    return ret;
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
        pthread_mutex_lock(&timer->do_exit_mtx);
        timer->do_exit = true;
        pthread_cond_signal(&timer->do_exit_cond);
        pthread_mutex_unlock(&timer->do_exit_mtx);
        pthread_join(timer->thread_handle, NULL);
        s19_mem_free(timer);
    }
    return E_OK;
}

void s19_sleep_msec(uint32_t msec)
{
    usleep(msec);
}
