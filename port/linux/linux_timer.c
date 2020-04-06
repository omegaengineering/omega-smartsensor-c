#include <string.h>
#include <unistd.h>
#include <sys/timerfd.h>
#include "linux_platform.h"


int linux_timer_start(linux_timer_t* timer, int period_ms)
{
    if (!timer->running) {
        struct itimerspec new_value;
        timer->period = period_ms;
        if ((timer->fd = timerfd_create(CLOCK_REALTIME, 0)) >= 0)
        {
            // one shot
            new_value.it_value.tv_sec = period_ms / 1000;
            new_value.it_value.tv_nsec = (period_ms % 1000) * 1000000;
            // interval
            new_value.it_interval.tv_sec = period_ms / 1000;
            new_value.it_interval.tv_nsec = (period_ms % 1000) * 1000000;
            timerfd_settime(timer->fd, 0, &new_value, NULL);
            timer->running = 1;
            return E_OK;
        }
    }
    return -1;
}

int linux_timer_stop(linux_timer_t* timer)
{
    if (timer->running) {
        close(timer->fd);
        timer->running = 0;
    }
    return E_OK;
}

int linux_timer_is_running(linux_timer_t* timer)
{
    return timer->running;
}

int linux_timer_get_fd(linux_timer_t* timer)
{
    return timer->fd;
}

