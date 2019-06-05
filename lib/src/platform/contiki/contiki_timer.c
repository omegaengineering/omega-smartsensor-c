/*!********************************************************************************************
  @file     contiki_timer.c

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