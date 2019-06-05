/*!********************************************************************************************
  @file     mutex.h

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
            provide abstraction over platform timers

***********************************************************************************************/

#ifndef OMEGA_SMARTSENSOR_C_TIMER_H
#define OMEGA_SMARTSENSOR_C_TIMER_H

#include <stdint.h>

struct _s19_timer;
typedef struct _s19_timer s19_timer_t;

struct _s19_timer_entry;
typedef struct _s19_timer_entry s19_timer_entry_t;

typedef void (*timer_trigger_callback)(void * user_data);

int s19_timer_create(s19_timer_t ** timer);
int s19_timer_entry_add(s19_timer_t *timer, s19_timer_entry_t **trigger);
int s19_timer_entry_set_period(s19_timer_entry_t *trigger, uint32_t period);
int s19_timer_entry_set_ctx(s19_timer_entry_t *trigger, void * ctx);
int s19_timer_entry_set_callback(s19_timer_entry_t *trigger, timer_trigger_callback callback);
int s19_timer_entry_remove(s19_timer_entry_t *trigger);
int s19_timer_destroy(s19_timer_t * timer);


void s19_sleep_msec(uint32_t msec);

#endif //OMEGA_SMARTSENSOR_C_TIMER_H
