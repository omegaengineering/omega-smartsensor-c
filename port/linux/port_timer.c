#include <unistd.h>
#include <pthread.h>
#include <poll.h>
#include <stdio.h>
#include <sys/timerfd.h>
#include "port.h"

#define N_TIMERS    2

typedef struct {
    uint8_t             set;
    int                 fd;
    uint32_t            period;
    port_timer_callback callback;
} portTimer_t;

typedef struct {

} portData_t;


static portTimer_t  timers[N_TIMERS];
static pthread_t    timer_thread;
static uint8_t      timer_started;
static uint8_t      timer_do_exit;

static int timers_set_poll(struct pollfd* polls, int n)
{
    int nActive = 0;
    // lock
    for (int i = 0; i < N_TIMERS && i < n; i++) {
        if (timers[i].set) {
            polls[nActive].fd = timers[i].fd;
            polls[nActive++].events = POLLIN;
        }
    }
    // unlock
    return nActive;
}

static portTimer_t* timers_get_by_fd(int fd)
{
    for (int i = 0; i < N_TIMERS; i++) {
        if (timers[i].fd == fd)
            return &timers[i];
    }
    return NULL;
}

void* timer_thread_process(void* arg)
{
    struct pollfd polls[N_TIMERS];
    int fd;

    while (!timer_do_exit)
    {
        int nActive = 0;
        nActive = timers_set_poll(polls, N_TIMERS);

        fd = poll(polls, nActive, 100);

        if (fd > 0) {
            for (int i = 0; i < nActive; i++) {
                if (polls[i].revents & POLLIN) {
                    uint64_t u64;
                    int s;
                    s = read(polls[i].fd, &u64, sizeof(uint64_t));

                    if (s != sizeof(uint64_t))
                        continue;

                    portTimer_t* timer = timers_get_by_fd(polls[i].fd);
                    if (timer && timer->set && timer->callback)
                        timer->callback(0);
                }
            }
        }
    }
    return NULL;
}

static int timer_thread_start()
{
    int ret = 0;
    if (!timer_started) {
        timer_do_exit = 0;
        ret = pthread_create(&timer_thread, NULL, timer_thread_process, NULL);
        if (ret) return ret;
        timer_started = 1;
    }
    return ret;
}

static int timer_thread_stop()
{
    timer_do_exit = 1;
    if (timer_started) {
        if (timer_started) {
            pthread_join(timer_thread, NULL);
            timer_started = 0;
        }
    }
    return 0;
}

int port_timer_init()
{
    return timer_thread_start();
}

void port_timer_close(int instance)
{
    timer_thread_stop();
}


int port_timer_register(int instance, int period, port_timer_callback callback)
{
    for (int i = 0; i < N_TIMERS; i++) {
        if (!timers[i].set) {
            timers[i].fd = timerfd_create(CLOCK_REALTIME, 0);
            timers[i].period = period;
            timers[i].callback = callback;
            timers[i].set = 1;
            break;
        }
    }
    return 0;
}

int port_timer_deregister(int instance, port_timer_callback callback)
{
    for (int i = 0; i < N_TIMERS; i++) {
        if (!timers[i].set && timers[i].callback == callback) {
            if (timers[i].fd >= 0)
                close(timers[i].fd);
            timers[i].period = 0;
            timers[i].callback = NULL;
            timers[i].set = 0;
            break;
        }
    }
    return 0;
}

void port_sleep_ms(uint32_t msec)
{
    usleep(msec);
}

