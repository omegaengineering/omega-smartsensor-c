//
// Created by dev on 5/23/19.
//

#ifndef OMEGA_SMARTSENSOR_C_LOG_H
#define OMEGA_SMARTSENSOR_C_LOG_H

#include <stdio.h>

#ifndef NDEBUG

#define s19_log_create(name)    static const char log_module[] = name
#define s19_log_info(fmt,...)   do { printf("[%5s][INFO]: " fmt,log_module, ##__VA_ARGS__); fflush(stdout); } while(0)
#define s19_log_dbg(fmt,...)    do { printf("[%5s][ DBG]: " fmt,log_module, ##__VA_ARGS__); fflush(stdout); } while(0)
#define s19_log_warn(fmt,...)   do { printf("[%5s][WARN]: " fmt,log_module, ##__VA_ARGS__); fflush(stdout); } while(0)

#else
#define s19_log_create(name)
#define s19_log_info(fmt,...)
#define s19_log_dbg(fmt,...)
#define s19_log_warn(fmt,...)

#endif //NDEBUG


#endif //OMEGA_SMARTSENSOR_C_LOG_H
