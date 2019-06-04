//
// Created by dev on 5/20/19.
//

#ifndef OMEGA_SMARTSENSOR_C_MUTEX_H
#define OMEGA_SMARTSENSOR_C_MUTEX_H

/// @brief: provide abstraction over platform mutexes

#define MAX_PLATFORM_MUTEX  3

typedef struct _s19_mutex s19_mutex_t;

int s19_mutex_create(s19_mutex_t **mutex);
int s19_mutex_lock(s19_mutex_t *mutex);
int s19_mutex_unlock(s19_mutex_t *mutex);
int s19_mutex_destroy(s19_mutex_t *mutex);

#endif //OMEGA_SMARTSENSOR_C_MUTEX_H
