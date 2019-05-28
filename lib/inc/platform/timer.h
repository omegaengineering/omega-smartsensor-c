//
// Created by dev on 5/23/19.
//

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
